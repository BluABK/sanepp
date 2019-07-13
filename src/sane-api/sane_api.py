import datetime
import threading
from flask import Flask, jsonify, render_template
from flask_restful import Resource, Api
from sqlalchemy import create_engine
from json import dumps

# Sane legacy
#   Database
from database.orm import init_db
from database.write_operations import DBUpdateVideo
#   YouTube
from sane_legacy.youtube.youtube_requests import get_stored_subscriptions
from youtube.youtube_dl_handler import YoutubeDownload

# Create the application instance
app = Flask(__name__, template_folder="templates")
api = Api(app)


@app.route('/')
def home():
    """
    This function just responds to the browser ULR
    localhost:5000/

    :return:        the rendered template 'home.html'
    """
    return render_template('home.html')


@app.route('/subfeed')
def youtube_subfeed(channel_id):
    pass


@app.route('/video')
def youtube_video(video_id):
    pass


@app.route('/channel')
def youtube_channel(channel_id):
    pass


@app.route('/subscriptions')
def youtube_subscriptions():
    """
    Returns stored (DB) subscriptions.
    :return: A list of dictified Channel objects
    """
    result = get_stored_subscriptions()

    return jsonify(result)


@app.route('/download', methods=['GET', 'PUT', 'CREATE'])
def youtube_download(video_id, db_update_listeners=None, youtube_dl_finished_listener=None, wait=False):
    # Vital parts unimplemented, return error for now.

    raise Exception("Not Implemented")

    # FIXME: get a video obj from id
    video = None

    # Set properties related to downloading.
    video.downloaded = True
    video.date_downloaded = datetime.datetime.utcnow()

    # Update Video information in Database.
    DBUpdateVideo(video, update_existing=True,
                  finished_listeners=db_update_listeners).start()

    # Create a threading event and set it if ready.
    event = threading.Event()

    if not wait:
        event.set()

    # FIXME: Find a way to handle "emitting" progress, finished and failed downloads.
    # Create a signal to track download progress. (was: PyQt signals/slots)
    # download_progress_signal = DownloadProgressSignals(video, event)
    download_progress_signal = None

    # Create a download instance.
    YoutubeDownload(video, event, download_progress_listener=download_progress_signal,
                    finished_listeners=youtube_dl_finished_listener).start()

    # return the signal
    return download_progress_signal


# If we're running in stand alone mode, run the application
if __name__ == '__main__':
    debug = True
    port = "5002"

    # Initialize database
    init_db()

    app.run(port=port, debug=debug)
