from handlers.log_handler import create_logger

YOUTUBE_URL = "https://www.youtube.com/"
YOUTUBE_PARM_VIDEO = "watch?v="
YOUTUBE_PARM_PLIST = "playlist?list ="
YT_VIDEO_URL = YOUTUBE_URL + YOUTUBE_PARM_VIDEO

# Create logger instance
logger = create_logger(__name__)


def get_subscriptions(youtube_oauth):
    """
    Get a list of the authenticated user's subscriptions.

    Also do a YouTube API Channels.list() for each subscription and attach the
    <contentDetails> resource which holds the uploaded videos playlist ID.

    :param youtube_oauth:
    :return: [subs]
    """
    subs = []

    logger.info("Getting subscriptions from remote (cached OAuth).")

    if youtube_oauth is None:
        logger.critical("YouTube API OAuth object was NoneType, aborting!")
        raise ValueError("Operation Aborted: YouTube API OAuth object was NoneType!")
    subscription_list_request = youtube_oauth.subscriptions().list(part='snippet', mine=True, maxResults=50)

    # Retrieve the list of subscribed channels for authenticated user's channel.
    while subscription_list_request:
        subscription_list_response = subscription_list_request.execute()

        # Grab information about each subscription/page.
        for page in subscription_list_response['items']:
            channel_id = page['snippet']['resourceId']['channelId']

            # Get channel from YouTube API: Channels.list()
            channel_response = youtube_oauth.channels().list(part='contentDetails',
                                                             id=channel_id).execute()

            # Attach ID of uploads playlist and more for more convenient access.
            page['contentDetails'] = channel_response['items'][0]['contentDetails']

            # Append the subscription.
            subs.append(page)

        # Iterate through the response pages
        subscription_list_request = youtube_oauth.playlistItems().list_next(
            subscription_list_request, subscription_list_response)

    return subs


def search_uploaded_videos(youtube_key, channel_id, req_limit):  # FIXME: Not used
    """
    Get a list of videos through the API search()
    Quota cost: 100 units / response
    :param req_limit:
    :param channel_id:
    :param youtube_key:

    :return: [list(dict): videos]
    """
    search_pages = 0
    videos = []

    # Retrieve the list of videos uploaded to the user's channel.
    playlistitems_list_request = youtube_key.search().list(
        maxResults=50, part='snippet', channelId=channel_id, order='date')

    # Iterate through the result pages
    while playlistitems_list_request:
        search_pages += 1
        playlistitems_list_response = playlistitems_list_request.execute()

        # Grab information about each video.
        for search_result in playlistitems_list_response['items']:
            if search_result['id']['kind'] == 'youtube#video':
                    videos.append(search_result)
        if search_pages >= req_limit:
            break

        # Iterate through the response pages
        playlistitems_list_request = youtube_key.playlistItems().list_next(
            playlistitems_list_request, playlistitems_list_response)

    return videos


def videos_list(youtube_key, video_ids, req_limit, part='snippet', edgecase=False):  # FIXME: Not used
    """
    Get a list of videos by calling YouTube API Videos.list().
    Quota cost: 2-3 units / part / request

    :param part:
    :param video_ids: a list of ids to request video from
    :param req_limit:
    :param youtube_key:
    :return: [list(dict): videos]
    """
    # Retrieve the list of videos uploaded to the authenticated user's channel.
    videos = []
    search_pages = 0
    string_video_ids = ','.join(map(str, video_ids))

    playlistitems_list_request = youtube_key.videos().list(
        maxResults=50, part=part, id=string_video_ids)

    # Iterate through the result pages
    while playlistitems_list_request:
        search_pages += 1
        playlistitems_list_response = playlistitems_list_request.execute()

        if edgecase:
            videos.extend(playlistitems_list_response['items'])
        else:
            # Grab information about each video.
            for search_result in playlistitems_list_response['items']:
                videos.append(search_result)

        if search_pages >= req_limit:
            break

        # Iterate through the response pages
        playlistitems_list_request = youtube_key.playlistItems().list_next(playlistitems_list_request,
                                                                           playlistitems_list_response)
    return videos


def playlistitems_list_uploaded_videos(youtube_key, uploads_playlist_id, req_limit):  # FIXME: Not used
    """
    Get a list of videos in a playlist by calling YouTube API: PlaylistItems.list().

    :param req_limit:
    :param youtube_key:
    :param uploads_playlist_id:
    :return: [list(dict): videos]
    """
    searched_pages = 0
    videos = []

    # Retrieve the list of videos uploaded to the authenticated user's channel.
    playlistitems_list_request = youtube_key.playlistItems().list(maxResults=50, part='snippet',
                                                                  playlistId=uploads_playlist_id)
    # Iterate through the result pages
    while playlistitems_list_request:
        searched_pages += 1

        playlistitems_list_response = playlistitems_list_request.execute()

        # Grab information about each video.
        for search_result in playlistitems_list_response['items']:
            videos.append(search_result)

        if searched_pages >= req_limit:
            break

        # Iterate through the response pages
        playlistitems_list_request = youtube_key.playlistItems().list_next(
            playlistitems_list_request, playlistitems_list_response)

    return videos


def get_channel_uploads(youtube_key, channel_id, req_limit):  # FIXME: Not used
    """
    Get a channel's "Uploaded videos" playlist, given channel ID.

    Calls YouTube API: Channels.list(), then passes the response onto PlaylistItems.list().

    :param req_limit:   Request limit.
    :param youtube_key:
    :param channel_id:
    :return: [list(dict): videos]
    """
    # Get channel
    channel = youtube_key.channels().list(part='contentDetails', id=channel_id).execute()

    # Get ID of uploads playlist
    channel_uploads_playlist_id = channel['items'][0]['contentDetails']['relatedPlaylists']['uploads']

    # Get playlistListResponse item of uploads playlist
    videos = playlistitems_list_uploaded_videos(youtube_key, req_limit, channel_uploads_playlist_id)

    return videos
