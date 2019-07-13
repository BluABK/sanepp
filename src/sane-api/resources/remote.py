from handlers.log_handler import create_logger

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


def remote_search_uploaded_videos(youtube_key, channel_id, req_limit):
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

        playlistitems_list_request = youtube_key.playlistItems().list_next(
            playlistitems_list_request, playlistitems_list_response)

    return videos
