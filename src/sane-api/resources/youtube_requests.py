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
