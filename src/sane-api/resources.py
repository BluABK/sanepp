from flask_restful import Resource
from flask import jsonify

from sane_legacy.youtube.youtube_requests import get_stored_subscriptions


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
