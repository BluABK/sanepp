from sane_legacy.handlers.log_handler import create_logger

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
