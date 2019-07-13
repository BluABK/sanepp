import datetime
import threading
from flask import Flask, jsonify, render_template, request, redirect
from flask_restful import Resource, Api
from sqlalchemy import create_engine
from json import dumps

# Sane legacy
#   Database
from database.orm import init_db
from database.write_operations import DBUpdateVideo
#   YouTube
from resources.youtube_api import youtube_api_channels_list
from resources.youtube_requests import get_stored_subscriptions
from resources.youtube_auth import load_key
from youtube.youtube_dl_handler import YoutubeDownload

# Create the application instance
app = Flask(__name__, template_folder="templates")
api = Api(app)

API_VERSION = 'v1'

"""
API Internal
"""


@app.route('/')
def home():
    """
    This function just responds to the browser URL
    localhost:<port>/

    :return:        the rendered template 'home.html'
    """
    return render_template('home.html')


"""
Local: Database lookups and other internal
"""


@app.route('/api/v1/local/video')
def youtube_video(id):
    pass  # FIXME: Implement


@app.route('/api/v1/local/subscriptions')
def youtube_subscriptions():
    """
    Returns stored (DB) subscriptions.
    :return: A list of dictified Channel objects
    """
    result = get_stored_subscriptions()

    return jsonify(result)


"""
Remote: Requests to the YouTube API with some extra functionality added on.
"""


@app.route('/api/v1/remote/subfeed')
def youtube_subfeed():
    """
    !! NB: This is *NOT* the regular YouTube subscription feed !!

    This function gets a list of every channel the user is subscribed to
    and combines the "Uploaded videos" playlists of all the channel into
    a single one.

    :return: JSONified list of subscribed channels' uploaded videos.
    """
    pass  # FIXME: Implement


@app.route('/video')
def youtube_video_remote(id):
    pass  # FIXME: Implement


@app.route('/api/v1/remote/channel')
def youtube_channel_remote():
    """
    Takes either a channel id or username and passes it as kwargs to YouTube API pass-through.
    :return: A youtube#channel json
    """
    youtube_auth = load_key()
    if 'id' in request.args:
        channel = youtube_api_channels_list(youtube_auth, part='contentDetails,snippet', id=request.args['id'])
    elif 'username' in request.args:
        channel = youtube_api_channels_list(youtube_auth, part='contentDetails,snippet',
                                            forUsername=request.args['username'])
    else:
        return jsonify("Error: no id or username field provided. Please specify one.")

    # Get ID of uploads playlist # FIXME: Why though?
    # TODO: store channel_id in channel, making one less extra request
    return jsonify(channel['items'][0])  # Send full response since id is outside of snippet


"""
# FIXME: find a name/category for youtube-dl. 
"""


@app.route('/api/v1/download', methods=['GET', 'PUT', 'CREATE'])
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


"""
YouTube: Pass-through kwargs directly to the YouTube API at https://www.googleapis.com/youtube/v3/
"""


@app.route('/api/v1/youtube/channels/list')
def youtube_api_channels_list_passthrough():
    """
    Passes on any kwargs directly to the YouTube API pass-through.
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Pass on kwargs and return (JSONified) result
    return jsonify(youtube_api_channels_list(youtube_auth, **request.args))


# If we're running in stand alone mode, run the application
if __name__ == '__main__':
    # Initialize database
    init_db()

    # Run API
    app.run(port="5002", debug=True)
