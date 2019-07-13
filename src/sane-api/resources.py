import datetime
import threading

from flask_restful import Resource
from flask import jsonify, render_template

from database.write_operations import DBUpdateVideo
from sane_legacy.youtube.youtube_requests import get_stored_subscriptions
from youtube.youtube_dl_handler import YoutubeDownload


class APIHomepage(Resource):
    def __init__(self):
        pass

    def get(self):
        """
        This function just responds to the browser ULR
        localhost:5000/

        :return:        the rendered template 'home.html'
        """
        return render_template('home.html')


class YouTubeSubFeed(Resource):
    def __init__(self):
        pass

    def get(self, channel_id):
        pass


class YouTubeVideo(Resource):
    def __init__(self):
        pass

    def get(self, video_id):
        # query =
        pass


class YouTubeChannel(Resource):
    def __init__(self):
        pass

    def get(self, channel_id):
        pass


class YouTubeSubscriptions(Resource):
    def __init__(self):
        pass

    def get(self):
        """
        Returns stored (DB) subscriptions.
        :return: A list of dictified Channel objects
        """
        result = get_stored_subscriptions()

        return jsonify(result)


class YouTubeDownload(Resource):
    def __init__(self):
        pass

    def get(self, video_id, db_update_listeners=None, youtube_dl_finished_listener=None, wait=False):
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
