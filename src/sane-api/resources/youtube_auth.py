import pickle
import threading
import time

import httplib2
import json

import apiclient
from google_auth_oauthlib.flow import InstalledAppFlow
from googleapiclient.discovery import build
from oauthlib.oauth2 import MissingCodeError

from absolute_paths import KEYS_FILE, CLIENT_SECRET_FILE, YOUTUBE_RESOURCE_OAUTH_PICKLE, YOUTUBE_RESOURCE_KEYS_PICKLE
from handlers.config_handler import read_config
from handlers.log_handler import create_logger
from settings import mutable_settings

logger = create_logger(__name__)

SCOPES = ['https://www.googleapis.com/auth/youtube.readonly']

API_SERVICE_NAME = 'youtube'
API_VERSION = 'v3'


class GenerateKeys(threading.Thread):

    def __init__(self, keys):
        threading.Thread.__init__(self)
        self.keys = keys

    def run(self):
        self.keys.append(youtube_auth_keys())


def generate_keys(key_number):
    keys = []
    threads = []

    logger.info("Starting key generation threads.")
    for _ in range(key_number):
        t = GenerateKeys(keys)
        t.start()
        threads.append(t)

    logger.info("Waiting for key generation threads.")
    for t in threads:
        t.join()

    return keys


def get_api_key():
    with open(KEYS_FILE) as keys_json_data_file:
        keys = json.load(keys_json_data_file)
    return keys['api_key']


def build_request(http, *args, **kwargs):
    """
    The httplib2.Http() objects are not thread-safe. Create a new Http() object for every request and
    override the construction of it within the service object.

    https://developers.google.com/api-client-library/python/guide/thread_safety
    :param http:
    :param args:
    :param kwargs:
    :return:
    """
    new_http = httplib2.Http()
    return apiclient.http.HttpRequest(new_http, *args, **kwargs)


def youtube_auth_oauth():
    """
    Authorize the request using OAuth and store authorization credentials.

    OAuth is required for most higher level user actions like accessing user's subscriptions.
    :return:
    """
    logger.info("OAuth: Authorising API...")
    flow = InstalledAppFlow.from_client_secrets_file(CLIENT_SECRET_FILE, SCOPES)
    if mutable_settings.using_gui:
        try:
            credentials = flow.run_local_server(host='localhost',
                                                port=read_config('Authentication', 'oauth2_local_server_port',
                                                                 literal_eval=True),
                                                authorization_prompt_message='Please visit this URL: {url}',
                                                success_message='The auth flow is complete; you may close this window.',
                                                open_browser=True)
        except MissingCodeError as exc_mce:
            logger.exception("A MissingCodeError Exception occurred during OAuth2",
                             exc_info=exc_mce)
            return None

    else:
        credentials = flow.run_console()
    logger.info("OAuth: Instantiated flow (console)")
    # Note: If you try to send in requestBuilder here it will fail, but OAuth isn't threaded so it should be fine...
    return build(API_SERVICE_NAME, API_VERSION, credentials=credentials)


def youtube_auth_keys():
    """
    Authorize the request using an API Key and store authorization credentials.

    API Keys are easier to use than needing to OAuth for every single resource.
    :return:
    """
    logger.info("Keys: Authorising API...")
    # Send in requestBuilder to override the construction of non-thread safe http obj within the service object.
    return build(API_SERVICE_NAME, API_VERSION, developerKey=get_api_key(), requestBuilder=build_request)


def save_youtube_resource_oauth(youtube):
    """
    Saves the Pickled YouTube OAuth Authenticated Resource object to a .pkl file.
    :param youtube:
    :return:
    """
    logger.info("Dumping youtube pickle")
    dump_pickle(youtube, YOUTUBE_RESOURCE_OAUTH_PICKLE)


def dump_pickle(pickle_object, path):
    """
    Dumps a Pickle object to a .pkl file.
    :param pickle_object:
    :param path:
    :return:
    """
    logger.info("Dumping pickle obj {} to {}".format(pickle_object, path))
    with open(path, 'wb') as pickle_output:
        pickle.dump(pickle_object, pickle_output, pickle.HIGHEST_PROTOCOL)


def load_pickle(path, max_attempts=6):
    """
    Loads a pickle file from path.
    :param path:
    :param max_attempts:
    :return:
    """
    logger.info("Loading pickle from {}".format(path))
    attempt = 1
    while attempt <= max_attempts:
        try:
            with open(path, 'rb') as pickle_input:
                pickle_object = pickle.load(pickle_input)
            return pickle_object
        except MemoryError as mem_exc:
            logger.error("Loading attempt #{} of pickle failed, retrying {} more time(s)...".format(
                attempt, max_attempts - attempt))
            logger.exception("load_pickle exception: ", exc_info=mem_exc)
            logger.debug("pickle path: {}".format(path))
            if attempt <= 6:
                attempt += 1
                time.sleep(1)
                continue
            else:
                logger.fatal("Loading of pickle has utterly failed, terminating application!")
                exit(-3)


def load_youtube_resource_oauth():
    """
    Loads a Pickled YouTube OAuth Authenticated Resource object from file.
    :return:
    """
    exc_occured = False

    logger.info("Loading YouTube OAuth Authenticated Resource object pickle from {}".format(
        YOUTUBE_RESOURCE_OAUTH_PICKLE))

    try:
        youtube_oauth = load_pickle(YOUTUBE_RESOURCE_OAUTH_PICKLE)

    except FileNotFoundError as file404_exc:
        logger.warning("Loading of cached OAuth: File not found. Requesting new OAuth from user.", exc_info=file404_exc)
        youtube_oauth = youtube_auth_oauth()
        if youtube_oauth is None:
            logger.critical("Failed to authenticate YouTube API OAuth2!")
            return None

    except ModuleNotFoundError as mod404_exc:
        logger.warning("Loading of cached OAuth: Module not found. Requesting new OAuth from user.",
                       exc_info=mod404_exc)
        youtube_oauth = youtube_auth_oauth()
        if youtube_oauth is None:
            logger.critical("Failed to authenticate YouTube API OAuth2!")
            return None

    except Exception as exc:
        logger.warning("Loading of cached OAuth: Unexpected exception. Requesting new OAuth from user.", exc_info=exc)
        youtube_oauth = youtube_auth_oauth()
        if youtube_oauth is None:
            logger.critical("Failed to authenticate YouTube API OAuth2!")
            return None

    if exc_occured:
        # If an exception had to be handled, save the new re-authenticated OAuth2 resource to disk.
        save_youtube_resource_oauth(youtube_oauth)

    return youtube_oauth


def load_youtube_resource_keys():
    """
    Loads Pickled YouTube API-Key Authenticated Resource objects from file.
    :return:
    """
    logger.info("Loading YouTube API-Key Authenticated Resource objects pickle from {}".format(
        YOUTUBE_RESOURCE_KEYS_PICKLE))
    return load_pickle(YOUTUBE_RESOURCE_KEYS_PICKLE)


def save_youtube_resource_keys(youtube_list):
    """
    Saves Pickled YouTube API-Key Authenticated Resource objects to file.
    :param youtube_list:
    :return:
    """
    logger.info("Saving YouTube API-Key Authenticated Resource objects pickle to {}".format(
        YOUTUBE_RESOURCE_KEYS_PICKLE))
    dump_pickle(youtube_list, YOUTUBE_RESOURCE_KEYS_PICKLE)


def load_keys(number_of_keys):
    youtube_keys = []
    try:
        youtube_keys = load_youtube_resource_keys()
    except FileNotFoundError as file404_exc:
        logger.info("load_youtube_resource_keys() gave 404 error. Generating new youtube key builds.",
                    exc_info=file404_exc)
        youtube_keys.extend(generate_keys(number_of_keys))
        save_youtube_resource_keys(youtube_keys)
    except ModuleNotFoundError as mod404_exc:
        logger.info("load_youtube_resource_keys() gave ModuleNotFoundError error. Generating new youtube key builds.",
                    exc_info=mod404_exc)
        youtube_keys.extend(generate_keys(number_of_keys))
        save_youtube_resource_keys(youtube_keys)
    except Exception as exc:
        logger.info("load_youtube_resource_keys() gave Unexpected exception error. Generating new youtube key builds.",
                    exc_info=exc)
        youtube_keys.extend(generate_keys(number_of_keys))
        save_youtube_resource_keys(youtube_keys)

    diff = number_of_keys - len(youtube_keys)
    if diff > 0:
        logger.info("Generating diff youtube key builds.")
        youtube_keys.extend(generate_keys(diff))
        save_youtube_resource_keys(youtube_keys)
    return youtube_keys


def load_key():
    """
    Simplified function to load a single YouTube auth key.
    :return:
    """
    return load_keys(1)[0]


