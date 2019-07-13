from database.detached_models.video_d import VideoD, GRAB_METHOD_SEARCH, GRAB_METHOD_LIST, \
    GRAB_METHOD_VIDEOS
from database.engine_statements import update_channel_from_remote, get_channel_by_id_stmt
from database.models import Channel
from database.orm import db_session
from database.write_operations import engine_execute_first, engine_execute
from handlers.log_handler import create_logger
from database.detached_models.video_d import VIDEO_KIND_VOD, VIDEO_KIND_LIVE, \
    VIDEO_KIND_LIVE_SCHEDULED
from sane_api import youtube_api_channels_list

YOUTUBE_URL = "https://www.youtube.com/"
YOUTUBE_PARM_VIDEO = "watch?v="
YOUTUBE_PARM_PLIST = "playlist?list ="
YT_VIDEO_URL = YOUTUBE_URL + YOUTUBE_PARM_VIDEO

# FIXME: module level logger not suggested: https://fangpenlin.com/posts/2012/08/26/good-logging-practice-in-python/
logger = create_logger(__name__)


def get_channel_uploads(youtube_key, channel_id, videos, req_limit):
    """
    Get a channel's "Uploaded videos" playlist, given channel ID.
    Carries videos and req_limit for use outside this scope.
    :param req_limit: carried from outside scope
    :param videos: carried from outside scope
    :param youtube_key:
    :param channel_id:
    :return: list_uploaded_videos(channel_uploads_playlist_id, debug=debug, limit=limit)
    """
    # Get channel
    channel = youtube_api_channels_list(part='contentDetails',
                                        id=channel_id)  # FIXME: stats unnecessary?

    # Get ID of uploads playlist
    # TODO: store channel_id in channel, making one less extra request
    channel_uploads_playlist_id = channel['items'][0]['contentDetails']['relatedPlaylists']['uploads']

    # Get playlistListResponse item of uploads playlist
    return list_uploaded_videos(youtube_key, videos, req_limit, channel_uploads_playlist_id)


def check_if_livestream(search_result):
    """
    Checks if a search_result is of type liveBroadcastContent.
    :param search_result:
    :return:
    """
    try:
        if 'liveBroadcastContent' in search_result['snippet']:
            live_str = search_result['snippet']['liveBroadcastContent']
            if live_str == 'live':
                logger.info("Livestream video: {}".format(search_result))
                return VIDEO_KIND_LIVE
            elif live_str == 'upcoming':
                logger.info("Livestream video (scheduled): {}".format(search_result))
                return VIDEO_KIND_LIVE_SCHEDULED
            elif live_str != 'none':
                # Catch future anomalies in value type
                logger.error("liveBroadcastContent in snippet, but val is '{}' NOT 'live' or 'none'!".format(live_str))
                logger.info(search_result)
                return VIDEO_KIND_VOD
    except Exception as anomaly:
        logger.critical("Anomaly detected while checking if search_result was liveBroadcastContent", exc_info=anomaly)
        logger.info(search_result)
        return None


def list_uploaded_videos(youtube_key, videos, uploads_playlist_id, req_limit):
    """
    Get a list of videos in a playlist
    :param req_limit:
    :param videos:
    :param youtube_key:
    :param uploads_playlist_id:
    :return: [list(dict): videos, dict: statistics]
    """
    # Retrieve the list of videos uploaded to the authenticated user's channel.
    playlistitems_list_request = youtube_key.playlistItems().list(
        maxResults=50, part='snippet', playlistId=uploads_playlist_id)

    searched_pages = 0
    while playlistitems_list_request:
        searched_pages += 1
        playlistitems_list_response = playlistitems_list_request.execute()

        # Grab information about each video.
        for search_result in playlistitems_list_response['items']:
            videos.append(VideoD.playlist_item_new_video_d(search_result, grab_methods=[GRAB_METHOD_LIST]))
        if searched_pages >= req_limit:
            break

        playlistitems_list_request = youtube_key.playlistItems().list_next(
            playlistitems_list_request, playlistitems_list_response)


def list_uploaded_videos_page(youtube_key, videos, uploads_playlist_id, playlistitems_list_request=None):
    if not playlistitems_list_request:
        playlistitems_list_request = youtube_key.playlistItems().list(
            maxResults=50, part='snippet', playlistId=uploads_playlist_id)

    playlistitems_list_response = playlistitems_list_request.execute()

    # Grab information about each video.
    for search_result in playlistitems_list_response['items']:
        videos.append(VideoD.playlist_item_new_video_d(search_result))

    return youtube_key.playlistItems().list_next(
        playlistitems_list_request, playlistitems_list_response)


def list_uploaded_videos_videos(youtube_key, video_ids, req_limit, part='snippet'):
    """
    Get a list of videos through the API videos()
    Quota cost: 2-3 units / part / request
    :param part:
    :param video_ids: a list of ids to request video from
    :param req_limit:
    :param youtube_key:
    :return: [list(dict): videos, dict: statistics]
    """
    # Retrieve the list of videos uploaded to the authenticated user's channel.
    videos = []
    string_video_ids = ','.join(map(str, video_ids))

    playlistitems_list_request = youtube_key.videos().list(
        maxResults=50, part=part, id=string_video_ids)
    search_pages = 0
    while playlistitems_list_request:
        search_pages += 1
        playlistitems_list_response = playlistitems_list_request.execute()

        # Grab information about each video.
        for search_result in playlistitems_list_response['items']:
            videos.append(VideoD.videos_item_new_video_d(search_result, grab_methods=[GRAB_METHOD_VIDEOS]))
        if search_pages >= req_limit:
            break
        playlistitems_list_request = youtube_key.playlistItems().list_next(playlistitems_list_request,
                                                                           playlistitems_list_response)
    return videos


def get_videos_result(youtube_key, video_ids, req_limit, part='snippet'):
    """
    Get a list of videos through the API videos()
    Quota cost: 2-3 units / part / request
    :param part:
    :param video_ids: a list of ids to request video from
    :param req_limit:
    :param youtube_key:
    :return: [list(dict): videos, dict: statistics]
    """
    # Retrieve the list of videos uploaded to the authenticated user's channel.
    results = []
    string_video_ids = ','.join(map(str, video_ids))

    playlistitems_list_request = youtube_key.videos().list(
        maxResults=50, part=part, id=string_video_ids)
    search_pages = 0
    while playlistitems_list_request:
        search_pages += 1
        playlistitems_list_response = playlistitems_list_request.execute()
        results.extend(playlistitems_list_response['items'])
        # Grab information about each video.
        if search_pages >= req_limit:
            break
        playlistitems_list_request = youtube_key.playlistItems().list_next(playlistitems_list_request,
                                                                           playlistitems_list_response)
    return results


def list_uploaded_videos_search(youtube_key, channel_id, videos, req_limit):
    """
    Get a list of videos through the API search()
    Quota cost: 100 units / response
    :param videos:
    :param req_limit:
    :param channel_id:
    :param youtube_key:
    :return: [list(dict): videos, dict: statistics]
    """
    # Retrieve the list of videos uploaded to the authenticated user's channel.
    playlistitems_list_request = youtube_key.search().list(
        maxResults=50, part='snippet', channelId=channel_id, order='date')
    search_pages = 0
    while playlistitems_list_request:
        search_pages += 1
        playlistitems_list_response = playlistitems_list_request.execute()

        # Grab information about each video.
        for search_result in playlistitems_list_response['items']:
            if search_result['id']['kind'] == 'youtube#video':
                video_kind = check_if_livestream(search_result)
                if video_kind is not None:
                    videos.append(VideoD(search_result, grab_methods=[GRAB_METHOD_SEARCH], kind=video_kind))
                else:
                    videos.append(VideoD(search_result, grab_methods=[GRAB_METHOD_SEARCH]))
        if search_pages >= req_limit:
            break

        playlistitems_list_request = youtube_key.playlistItems().list_next(
            playlistitems_list_request, playlistitems_list_response)


# FIXME: Redundant?
# def get_subscriptions(cached_subs):
#     if cached_subs:
#         return get_stored_subscriptions()
#     else:
#         return facilitate_getting_remote_subscriptions()


def add_subscription_local(youtube_auth, channel_id, by_username=False):
    """
    Add a YouTube subscription (Local/DB).
    :param by_username:
    :param youtube_auth:
    :param channel_id:
    :return:
    """
    # FIXME: Somewhat duplicate code of get_remote_subscriptions, move to own function -- START
    # Get ID of uploads playlist
    # channel_uploads_playlist_id = get_channel_uploads_playlist_id(youtube_auth, channel_id)
    if by_username:
        channel_response = youtube_api_channels_list(youtube_auth, part='contentDetails,snippet', id=channel_id)
    else:
        channel_response = youtube_api_channels_list(youtube_auth, part='contentDetails,snippet',
                                                     forUsername=channel_id)
    channel_uploads_playlist_id = channel_response['contentDetails']['relatedPlaylists']['uploads']
    channel = Channel(channel_response, channel_uploads_playlist_id, channel_list_response=True)
    db_channel = engine_execute_first(get_channel_by_id_stmt(channel))
    if db_channel:
        engine_execute(update_channel_from_remote(channel))
    else:
        # TODO: change to sqlalchemy core stmt
        create_logger(__name__ + ".subscriptions").info(
            "Added channel {} - {}".format(channel.title, channel.id))
        db_session.add(channel)

    db_session.commit()
    # FIXME: Somewhat duplicate code of get_remote_subscriptions, move to own function -- END

    logger.info("Added subscription (Local/DB): {} / {}".format(channel_id, channel.title))


def add_subscription(youtube_auth, channel_id, by_username=False):
    """
    Add a YouTube Channel subscription.
    :param by_username:
    :param youtube_auth: api key or oauth
    :param channel_id:
    :return: returns response or raises exception
    """
    add_subscription_local(youtube_auth, channel_id, by_username=by_username)
    # add_subscription_remote(channel_id) # DEPRECATED, see its docstring
