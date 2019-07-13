from database.engine_statements import get_channel_by_id_stmt, update_channel_from_remote
from database.models import Channel
from database.orm import db_session
from database.write_operations import engine_execute_first, engine_execute, delete_sub_not_in_list
from handlers.log_handler import create_logger
from youtube_api import youtube_api_channels_list

# Create logger instance
logger = create_logger(__name__)


def get_remote_subscriptions(youtube_oauth):
    """
    Get a list of the authenticated user's subscriptions.
    :param youtube_oauth:
    :return: [subs]
    """
    logger.info("Getting subscriptions from remote (cached OAuth).")
    if youtube_oauth is None:
        logger.critical("YouTube API OAuth object was NoneType, aborting!")
        return None
    subscription_list_request = youtube_oauth.subscriptions().list(part='snippet', mine=True,
                                                                   maxResults=50)
    subs = []
    # Retrieve the list of subscribed channels for authenticated user's channel.
    channel_ids = []
    while subscription_list_request:
        subscription_list_response = subscription_list_request.execute()

        # Grab information about each subscription page
        for page in subscription_list_response['items']:
            # Get channel
            channel_response = youtube_api_channels_list(youtube_oauth, part='contentDetails',
                                                         id=page['snippet']['resourceId']['channelId'])

            # Get ID of uploads playlist
            channel_uploads_playlist_id = channel_response['items'][0]['contentDetails']['relatedPlaylists']['uploads']
            channel = Channel(page['snippet'], channel_uploads_playlist_id)
            db_channel = engine_execute_first(get_channel_by_id_stmt(channel))
            if db_channel:
                engine_execute(update_channel_from_remote(channel))
                subs.append(channel)
            else:
                # TODO: change to sqlalchemy core stmt
                create_logger(__name__ + ".subscriptions").info(
                    "Added channel {} - {}".format(channel.title, channel.id))
                db_session.add(channel)
                subs.append(channel)
            channel_ids.append(channel.id)
        subscription_list_request = youtube_oauth.playlistItems().list_next(
            subscription_list_request, subscription_list_response)
    delete_sub_not_in_list(channel_ids)
    db_session.commit()
    return subs
