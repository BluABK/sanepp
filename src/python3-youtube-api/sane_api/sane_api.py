from flask import Flask, request
from flask_restful import Resource, Api
from sqlalchemy import create_engine
from json import dumps
from flask import jsonify

from sane_api.resources import YouTubeSubFeed, YouTubeChannel, YouTubeDownload, YouTubeVideo, YouTubeSubscriptions


class SaneAPI:

    API_PORT = '5002'
    default_routes = [[YouTubeChannel, '/channel'],
                      [YouTubeVideo, '/video'],
                      [YouTubeDownload, '/download'],
                      [YouTubeSubFeed, '/subfeed'],
                      [YouTubeSubscriptions, '/subscriptions']]

    def __init__(self):
        self.routes = None

        self.app = Flask(__name__)
        self.api = Api(self.app)
        self.add_resources(self.default_routes)
        self.run()

    def add_resource(self, resource, route):
        self.api.add_resource(resource, route)

    def add_resources(self, resources_with_routes):
        for resource_with_route in resources_with_routes:
            self.add_resource(*resource_with_route)

    def run(self):
        self.app.run(port=self.API_PORT)
