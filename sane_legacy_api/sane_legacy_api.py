from flask import Flask, jsonify, render_template, request, Response
from flask_restful import Api

# Sane legacy
from sane_legacy.handlers.log_handler import create_logger
from resources.youtube_auth import load_key, load_youtube_resource_oauth

# Create logger instance
logger = create_logger(__name__)

# Create the application instance
app = Flask(__name__, template_folder="templates")
api = Api(app)

API_VERSION = 'v1'


@app.errorhandler(Exception)
def exception_handler(error):
    status_code = str(error)[11:14]

    return Response(repr(error), status=status_code)


# If we're running in stand alone mode, run the application
if __name__ == '__main__':
    # Load Oauth2
    youtube_auth = load_youtube_resource_oauth()
    # Dump the refresh token
    print("YouTube OAuth2 refresh token: {}".format(youtube_auth._http.credentials._refresh_token))

    # Run API
    # app.run(port="5002", debug=True)
