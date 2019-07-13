from flask import Flask, request, render_template
from flask_restful import Resource, Api
from sqlalchemy import create_engine
from json import dumps
from flask import jsonify

from database.orm import init_db
from resources import APIHomepage, YouTubeSubFeed, YouTubeChannel, YouTubeDownload, YouTubeVideo, YouTubeSubscriptions


class SaneAPI:

    def __init__(self, debug=False, port='5002', run=True):
        self.debug = debug
        self.port = port
        self.routes = [[APIHomepage, '/'],
                       [YouTubeChannel, '/channel'],
                       [YouTubeVideo, '/video'],
                       [YouTubeDownload, '/download'],
                       [YouTubeSubFeed, '/subfeed'],
                       [YouTubeSubscriptions, '/subscriptions']]

        # Initialize database
        init_db()

        self.app = Flask(__name__, template_folder="templates")
        self.api = Api(self.app)
        if run:
            self.add_resources(self.routes)
            self.run()

    def add_resource(self, resource, route):
        self.api.add_resource(resource, route)

    def add_resources(self, resources_with_routes):
        for resource_with_route in resources_with_routes:
            self.add_resource(*resource_with_route)

    def run(self):
        self.app.run(port=self.port, debug=self.debug)
