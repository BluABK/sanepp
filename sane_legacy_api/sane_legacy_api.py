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


@app.route('/api/v1/remote/channel')
def youtube_channel_remote():
    """
    Takes either a channel <id> or <username> and passes it as kwargs to YouTube API pass-through.

    :return: A youtube#channel JSON
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    if 'id' in request.args:
        channel = youtube_auth.channels().list(part='contentDetails,snippet', id=request.args['id']).execute()

    elif 'username' in request.args:
        channel = youtube_auth.channels().list(part='contentDetails,snippet',
                                               forUsername=request.args['username']).execute()

    else:
        return jsonify("Error: no id or username field provided. Please specify one.")

    return jsonify(channel['items'][0])  # Send full relevant response since id is outside of snippet

@app.errorhandler(Exception)
def exception_handler(error):
    status_code = str(error)[11:14]

    return Response(repr(error), status=status_code)


# If we're running in stand alone mode, run the application
if __name__ == '__main__':
    # Debug OAuth2
    youtube_auth = load_youtube_resource_oauth()

    print(youtube_auth._http.credentials._refresh_token)
    # Run API
    app.run(port="5002", debug=True)
