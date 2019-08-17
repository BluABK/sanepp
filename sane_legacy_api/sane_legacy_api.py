import datetime
import threading
from flask import Flask, jsonify, render_template, request, Response
from flask_restful import Api

# Sane legacy
from sane_legacy.handlers.log_handler import create_logger
from resources import remote
from resources.youtube_auth import load_key, load_youtube_resource_oauth
from sane_legacy.youtube.youtube_dl_handler import YoutubeDownload

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


"""
# FIXME: find a name/category for youtube-dl. 
"""


# noinspection PyUnreachableCode
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

Functions that are not actually supported on their end won't be implemented, see:
https://developers.google.com/youtube/v3/getting-started#supported-operations
"""


def remove_empty_kwargs(**kwargs):
    """
    Remove keyword arguments that are not set.
    :param kwargs:
    :return:
    """
    good_kwargs = {}
    if kwargs is not None:
        for key, value in kwargs.items():
            if value:
                good_kwargs[key] = value

    return good_kwargs


""" Activities """


@app.route('/api/v1/youtube/activities/list')
def youtube_api_activities_list():
    """
    Passes on any kwargs directly to the YouTube API: activities.list().

    https://developers.google.com/youtube/v3/docs/activities/list
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.activities().list(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


@app.route('/api/v1/youtube/activities/insert')
def youtube_api_activities_insert():
    """
    Passes on any kwargs directly to the YouTube API: activities.insert().

    https://developers.google.com/youtube/v3/docs/activities/insert
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.activities().insert(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


""" Captions """


@app.route('/api/v1/youtube/captions/list')
def youtube_api_captions_list():
    """
    Passes on any kwargs directly to the YouTube API: captions.list().

    https://developers.google.com/youtube/v3/docs/captions/list
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.captions().list(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


@app.route('/api/v1/youtube/captions/insert')
def youtube_api_captions_insert():
    """
    Passes on any kwargs directly to the YouTube API: captions.insert().

    https://developers.google.com/youtube/v3/docs/captions/insert
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.captions().insert(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


@app.route('/api/v1/youtube/captions/update')
def youtube_api_captions_update():
    """
    Passes on any kwargs directly to the YouTube API: captions.update().

    https://developers.google.com/youtube/v3/docs/captions/update
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.captions().update(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


@app.route('/api/v1/youtube/captions/download')
def youtube_api_captions_download():
    """
    Passes on any kwargs directly to the YouTube API: captions.download().

    https://developers.google.com/youtube/v3/docs/captions/download
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.captions().download(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


@app.route('/api/v1/youtube/captions/delete')
def youtube_api_captions_delete():
    """
    Passes on any kwargs directly to the YouTube API: captions.delete().

    https://developers.google.com/youtube/v3/docs/captions/delete
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.captions().delete(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


""" ChannelBanners """


@app.route('/api/v1/youtube/channelBanners/insert')
def youtube_api_channel_banners_insert():
    """
    Passes on any kwargs directly to the YouTube API: channelBanners.insert().

    https://developers.google.com/youtube/v3/docs/channelBanners/insert
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.channelBanners().insert(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


""" Channels """


@app.route('/api/v1/youtube/channels/list')
def youtube_api_channels_list():
    """
    Passes on any kwargs directly to the YouTube API: Channels.list().

    :return: youtube#channelListResponse JSON

    https://developers.google.com/youtube/v3/docs/channels/list
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.channels().list(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


""" ChannelSections """


@app.route('/api/v1/youtube/channelSections/list')
def youtube_api_channel_section_list():
    """
    Passes on any kwargs directly to the YouTube API: channelSections.list().

    https://developers.google.com/youtube/v3/docs/channelSections/list
    :return:
    """
    # Get an authenticated API OAuth2 object
    youtube_auth = load_youtube_resource_oauth()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.channelSections().list(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


@app.route('/api/v1/youtube/channelSections/insert')
def youtube_api_channel_section_insert():
    """
    Passes on any kwargs directly to the YouTube API: channelSections.insert().

    https://developers.google.com/youtube/v3/docs/channelSections/insert
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.channelSections().insert(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


@app.route('/api/v1/youtube/channelSections/update')
def youtube_api_channel_section_update():
    """
    Passes on any kwargs directly to the YouTube API: channelSections.update().

    https://developers.google.com/youtube/v3/docs/channelSections/update
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.channelSections().update(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


@app.route('/api/v1/youtube/channelSections/delete')
def youtube_api_channel_section_delete():
    """
    Passes on any kwargs directly to the YouTube API: channelSections.delete().

    https://developers.google.com/youtube/v3/docs/channelSections/delete
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.channelSections().delete(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


""" Comments """


@app.route('/api/v1/youtube/comments/list')
def youtube_api_comment_list():
    """
    Passes on any kwargs directly to the YouTube API: comments.list().

    https://developers.google.com/youtube/v3/docs/comments/list
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.comments().list(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


@app.route('/api/v1/youtube/comments/insert')
def youtube_api_comment_insert():
    """
    Passes on any kwargs directly to the YouTube API: comments.insert().

    https://developers.google.com/youtube/v3/docs/comments/insert
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.comments().insert(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


@app.route('/api/v1/youtube/comments/update')
def youtube_api_comment_update():
    """
    Passes on any kwargs directly to the YouTube API: comments.update().

    https://developers.google.com/youtube/v3/docs/comments/update
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.comments().update(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


@app.route('/api/v1/youtube/comments/markAsSpam')
def youtube_api_comment_mark_as_spam():
    """
    Passes on any kwargs directly to the YouTube API: comments.markAsSpam().

    https://developers.google.com/youtube/v3/docs/comments/markAsSpam
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.comments().markAsSpam(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


@app.route('/api/v1/youtube/comments/setModerationStatus')
def youtube_api_comment_set_moderation_status():
    """
    Passes on any kwargs directly to the YouTube API: comments.setModerationStatus().

    https://developers.google.com/youtube/v3/docs/comments/setModerationStatus
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.comments().setModerationStatus(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


@app.route('/api/v1/youtube/comments/delete')
def youtube_api_comment_delete():
    """
    Passes on any kwargs directly to the YouTube API: comments.delete().

    https://developers.google.com/youtube/v3/docs/comments/delete
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.comments().delete(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


""" CommentThreads """


@app.route('/api/v1/youtube/commentThreads/list')
def youtube_api_comment_thread_list():
    """
    Passes on any kwargs directly to the YouTube API: commentThreads.list().

    https://developers.google.com/youtube/v3/docs/commentThreads/list
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.commentThreads().list(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


@app.route('/api/v1/youtube/commentThreads/insert')
def youtube_api_comment_thread_insert():
    """
    Passes on any kwargs directly to the YouTube API: commentThreads.insert().

    https://developers.google.com/youtube/v3/docs/commentThreads/insert
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.commentThreads().insert(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


@app.route('/api/v1/youtube/commentThreads/update')
def youtube_api_comment_thread_update():
    """
    Passes on any kwargs directly to the YouTube API: commentThreads.update().

    https://developers.google.com/youtube/v3/docs/commentThreads/update
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.commentThreads().update(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


""" GuideCategories """


@app.route('/api/v1/youtube/guideCategories/list')
def youtube_api_guide_categories_list():
    """
    Passes on any kwargs directly to the YouTube API: guideCategories.list().

    https://developers.google.com/youtube/v3/docs/guideCategories/list
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.guideCategories().list(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


""" i18nLanguages """


@app.route('/api/v1/youtube/i18nLanguages/list')
def youtube_api_i18n_languages_list():
    """
    Passes on any kwargs directly to the YouTube API: i18nLanguages.list().

    https://developers.google.com/youtube/v3/docs/i18nLanguages/list
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.i18nLanguages().list(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


""" i18nRegions """


@app.route('/api/v1/youtube/i18nRegions/list')
def youtube_api_i18n_regions_list():
    """
    Passes on any kwargs directly to the YouTube API: i18nRegions.list().

    https://developers.google.com/youtube/v3/docs/i18nRegions/list
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.i18nRegions().list(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


""" PlaylistItems """


@app.route('/api/v1/youtube/playlistItems/list')
def youtube_api_playlist_items_list():
    """
    Passes on any kwargs directly to the YouTube API: playlistItems.list().

    https://developers.google.com/youtube/v3/docs/playlistItems/list
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.playlistItems().list(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


@app.route('/api/v1/youtube/playlistItems/insert')
def youtube_api_playlist_items_insert():
    """
    Passes on any kwargs directly to the YouTube API: playlistItems.insert().

    https://developers.google.com/youtube/v3/docs/playlistItems/insert
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.playlistItems().insert(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


@app.route('/api/v1/youtube/playlistItems/update')
def youtube_api_playlist_items_update():
    """
    Passes on any kwargs directly to the YouTube API: playlistItems.update().

    https://developers.google.com/youtube/v3/docs/playlistItems/update
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.playlistItems().update(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


@app.route('/api/v1/youtube/playlistItems/delete')
def youtube_api_playlist_items_delete():
    """
    Passes on any kwargs directly to the YouTube API: playlistItems.delete().

    https://developers.google.com/youtube/v3/docs/playlistItems/delete
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.playlistItems().delete(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


""" Playlists """


@app.route('/api/v1/youtube/playlists/list')
def youtube_api_playlists_list():
    """
    Passes on any kwargs directly to the YouTube API: playlists.list().

    https://developers.google.com/youtube/v3/docs/playlists/list
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.playlists().list(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


@app.route('/api/v1/youtube/playlists/insert')
def youtube_api_playlists_insert():
    """
    Passes on any kwargs directly to the YouTube API: playlists.insert().

    https://developers.google.com/youtube/v3/docs/playlists/insert
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.playlists().insert(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


@app.route('/api/v1/youtube/playlists/update')
def youtube_api_playlists_update():
    """
    Passes on any kwargs directly to the YouTube API: playlists.update().

    https://developers.google.com/youtube/v3/docs/playlists/update
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.playlists().update(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


@app.route('/api/v1/youtube/playlists/delete')
def youtube_api_playlists_delete():
    """
    Passes on any kwargs directly to the YouTube API: playlists.delete().

    https://developers.google.com/youtube/v3/docs/playlists/delete
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.playlists().delete(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


""" Search """


@app.route('/api/v1/youtube/search/list')
def youtube_api_search():
    """
    Passes on any kwargs directly to the YouTube API: Search().

    https://developers.google.com/youtube/v3/docs/search/list
    :return:
    """
    # Get an authenticated API OAuth2 object
    youtube_auth = load_youtube_resource_oauth()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.search().list(**kwargs).execute()

    return jsonify(response)


""" Subscriptions """


@app.route('/api/v1/youtube/subscriptions/list')
def youtube_api_subscriptions_list():
    """
    Passes on any kwargs directly to the YouTube API: subscriptions.list().

    https://developers.google.com/youtube/v3/docs/subscriptions/list
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.subscriptions().list(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


""" Thumbnails """


@app.route('/api/v1/youtube/thumbnails/set')
def youtube_api_thumbnails_set():
    """
    Passes on any kwargs directly to the YouTube API: thumbnails.set().

    https://developers.google.com/youtube/v3/docs/thumbnails/set
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.thumbnails().set(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


""" VideoAbuseReportReasons """


@app.route('/api/v1/youtube/videoAbuseReportReasons/list')
def youtube_api_video_abuse_report_reasons_list():
    """
    Passes on any kwargs directly to the YouTube API: videoAbuseReportReasons.list().

    https://developers.google.com/youtube/v3/docs/videoAbuseReportReasons/list
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.videoAbuseReportReasons().list(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


""" VideoCategories """


@app.route('/api/v1/youtube/videoCategories/list')
def youtube_api_video_categories_insert():
    """
    Passes on any kwargs directly to the YouTube API: videoCategories.list().

    https://developers.google.com/youtube/v3/docs/videoCategories/list
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.videoCategories().list(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


""" Videos """


@app.route('/api/v1/youtube/videos/list')
def youtube_api_videos_list():
    """
    Passes on any kwargs directly to the YouTube API: videos.list().

    https://developers.google.com/youtube/v3/docs/videos/list
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_youtube_resource_oauth()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.videos().list(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


@app.route('/api/v1/youtube/videos/insert')
def youtube_api_videos_insert():
    """
    Passes on any kwargs directly to the YouTube API: videos.insert().

    https://developers.google.com/youtube/v3/docs/videos/insert
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.videos().insert(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


@app.route('/api/v1/youtube/videos/update')
def youtube_api_videos_update():
    """
    Passes on any kwargs directly to the YouTube API: videos.update().

    https://developers.google.com/youtube/v3/docs/videos/update
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.videos().update(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


@app.route('/api/v1/youtube/videos/rate')
def youtube_api_videos_rate():
    """
    Passes on any kwargs directly to the YouTube API: videos.rate().

    https://developers.google.com/youtube/v3/docs/videos/rate
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.videos().rate(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


@app.route('/api/v1/youtube/videos/getRating')
def youtube_api_videos_get_rating():
    """
    Passes on any kwargs directly to the YouTube API: videos.getRating().

    https://developers.google.com/youtube/v3/docs/videos/getRating
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.videos().getRating(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


@app.route('/api/v1/youtube/videos/reportAbuse')
def youtube_api_videos_report_abuse():
    """
    Passes on any kwargs directly to the YouTube API: videos.reportAbuse().

    https://developers.google.com/youtube/v3/docs/videos/reportAbuse
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.videos().reportAbuse(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


@app.route('/api/v1/youtube/videos/delete')
def youtube_api_videos_delete():
    """
    Passes on any kwargs directly to the YouTube API: videos.delete().

    https://developers.google.com/youtube/v3/docs/videos/delete
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.videos().delete(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


""" Watermarks """


@app.route('/api/v1/youtube/watermarks/set')
def youtube_api_watermarks_set():
    """
    Passes on any kwargs directly to the YouTube API: watermarks.set().

    https://developers.google.com/youtube/v3/docs/watermarks/set
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.watermarks().set(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


@app.route('/api/v1/youtube/watermarks/unset')
def youtube_api_watermarks_unset():
    """
    Passes on any kwargs directly to the YouTube API: watermarks.unset().

    https://developers.google.com/youtube/v3/docs/watermarks/unset
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.watermarks().unset(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


"""
Testing and mock calls.
"""


@app.route('/api/v1/test/static/json')
def testing_api_static_json():
    # return jsonify({'one': 1, 'two': 2, 'three': 3, "sub_dict": {'subitem1': 'cake', 'subitem2': 4.20}, 'the list':
    #         ['this', 'is', 'a', 'list']})
    return jsonify([{'one': '1', 'two': '2', 'three': '3', "sub_dict": {'subitem1': 'cake', 'subitem2': '4.20'},
                     'the list': ['this', 'is', 'a', 'list']}])
    # return jsonify(['a', 'b', 'c'])


@app.errorhandler(Exception)
def exception_handler(error):
    status_code = str(error)[11:14]

    return Response(repr(error), status=status_code)


# If we're running in stand alone mode, run the application
if __name__ == '__main__':
    # Debug OAuth2
    youtube_auth = load_youtube_resource_oauth()
    logger.debug(youtube_auth.__dict__)
    logger.debug(youtube_auth._http.__dict__)
    logger.debug(youtube_auth._http.credentials.__dict__)
    # Run API
    app.run(port="5002", debug=True)
