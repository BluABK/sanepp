from flask import Flask, jsonify, render_template, request, Response
from flask_restful import Api

# Sane legacy
from sane_legacy.handlers.log_handler import create_logger
from resources import remote
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


"""
Local: Internal or local operations.
"""


@app.route('/api/v1/local/add_subscription')
def add_subscription():
    """
    Add a YouTube subscription (Local).
    Takes a channel ID or username as argument.

    :return: Returns the youtube#channel resource response.
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

    channel_title = channel['items'][0]['snippet']['localized']['title']
    channel_id = channel['items'][0]['id']

    logger.info("Adding subscription (Local): {} / {}".format(channel_id, channel_title))

    return jsonify(channel)


"""
Remote: Requests to the YouTube API with some extra functionality added on.
"""


@app.route('/api/v1/remote/subscriptions')
def youtube_subscriptions():
    """
    Returns subscriptions from YouTube API.

    :return: A list of youtube#subscription resources with <contentDetails> added on.
    """
    logger.info("Getting subscriptions from DB.")

    # Get an authenticated OAuth2 resource (since this is authenticated user scope)
    youtube_oauth = load_youtube_resource_oauth()

    try:
        subscriptions = remote.get_subscriptions(youtube_oauth)
    except ValueError as exc_ve:
        return jsonify(str(exc_ve))

    return jsonify(subscriptions)


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
