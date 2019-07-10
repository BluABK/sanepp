import os

import click
import datetime

from sqlalchemy import or_, and_

from absolute_paths import LOG_DIR
from database.orm import db_session, init_db
from database.video import Video
from handlers.config_handler import read_config
from handlers.log_handler import create_logger
from main import run_with_cli, cli_refresh_and_print_subfeed
from youtube.update_videos import load_keys
from youtube.youtube_requests import get_subscriptions
from cli import print_functions
import youtube as youtube

logger = create_logger(__name__)
logger.info("Initializing...")

OS_PATH = os.path.dirname(__file__)

PICKLE_PATH = os.path.join(OS_PATH, 'resources', 'pickles')
THUMBNAIL_PATH = os.path.join(OS_PATH, 'resources', 'thumbnails')

# Initialize database
init_db()

# Make sure dirs exists on startup
if not os.path.isdir(PICKLE_PATH):
    os.makedirs(PICKLE_PATH)

if not os.path.isdir(THUMBNAIL_PATH):
    os.makedirs(THUMBNAIL_PATH)

if not os.path.isdir(LOG_DIR):
    os.makedirs(LOG_DIR)

# Make sure files exists on startup
# if not os.path.isfile(HISTORY_FILE_PATH):
#     open(HISTORY_FILE_PATH, 'a').close()


@click.option(u'--no_gui', is_flag=True)
@click.option(u'--update-watch-prio', is_flag=True)
@click.option(u'--set-watched-day')
@click.option(u'--refresh_and_print_subfeed', is_flag=True)
@click.option(u'--print_subscriptions', is_flag=True)
@click.option(u'--print_downloaded_videos', is_flag=True)
@click.option(u'--print_watched_videos', is_flag=True)
@click.option(u'--print_discarded_videos', is_flag=True)
@click.option(u'--print_playlist_items', is_flag=False)
@click.option(u'--print_playlist_items_url_only', is_flag=True)
@click.command()
def cli(no_gui, update_watch_prio, set_watched_day, refresh_and_print_subfeed, print_subscriptions,
        print_watched_videos, print_discarded_videos, print_downloaded_videos, print_playlist_items,
        print_playlist_items_url_only):
    logger = create_logger(__name__)
    if update_watch_prio:
        videos = db_session.query(Video).all()
        watch_prio = read_config('Play', 'default_watch_prio')
        logger.debug("Setting watch_prio {}, for: {} videos".format(watch_prio, len(videos)))
        for video in videos:
            video.watch_prio = watch_prio
        db_session.commit()
        return

    if set_watched_day:
        videos = db_session.query(Video).filter(or_(Video.downloaded == True, (Video.vid_path.is_(None)))).all()
        for video in videos:
            vid_age = datetime.datetime.utcnow() - video.date_published
            if vid_age > datetime.timedelta(days=int(set_watched_day)):
                logger.debug("Setting watched, {} - {} - {}".format(vid_age, video.title, video.__dict__))
                video.watched = True
        db_session.commit()
        return
    if refresh_and_print_subfeed:
        cli_refresh_and_print_subfeed()
    if print_subscriptions:
        cached_subs = True
        subs = get_subscriptions(cached_subs)
        for channel in subs:
            if channel.subscribed_override:
                print(("[{}]    {} [Subscription override]".format(channel.id, channel.title)))
            else:
                print(("[{}]    {}".format(channel.id, channel.title)))
    if print_watched_videos:
        videos = db_session.query(Video).filter(and_(Video.watched is True, (Video.vid_path.isnot(None)))).all()
        print_functions.print_videos(videos, path_only=True)
    if print_discarded_videos:
        videos = db_session.query(Video).filter(and_(Video.discarded is True, (Video.vid_path.isnot(None)))).all()
        print_functions.print_videos(videos, path_only=True)
    if print_downloaded_videos:
        videos = db_session.query(Video).filter(and_(Video.downloaded is True, (Video.vid_path.isnot(None)))).all()
        print_functions.print_videos(videos, path_only=True)
    if print_playlist_items:
        youtube_auth_resource = load_keys(1)[0]
        playlist_video_items = []
        youtube.youtube_requests.list_uploaded_videos(youtube_auth_resource, playlist_video_items,
                                                      print_playlist_items, 50)
        for vid in playlist_video_items:
            if print_playlist_items_url_only:
                print(vid.url_video)
            else:
                print(vid)

    run_with_cli()


if __name__ == '__main__':
    cli()
