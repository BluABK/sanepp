import os

# Backend
ROOT_PATH = os.path.dirname(__file__)
PROJECT_ROOT_PATH = os.path.join(ROOT_PATH, '..')
RESOURCES_PATH = os.path.join(ROOT_PATH, 'resources')
IMG_PATH = os.path.join(ROOT_PATH, 'images')

# Pickler
PICKLE_PATH = os.path.join(RESOURCES_PATH, 'pickles')
YOUTUBE_RESOURCE_OAUTH_PICKLE = os.path.join(PICKLE_PATH, 'youtube_oauth.pkl')
YOUTUBE_RESOURCE_KEYS_PICKLE = os.path.join(PICKLE_PATH, 'youtube_auth_keys.pkl')

# YouTube
CLIENT_SECRET_FILE = os.path.join(RESOURCES_PATH, 'client_secret.json')
CLIENT_SECRET_PUBLIC_FILE = os.path.join(RESOURCES_PATH, "client_secret_public.json")
KEYS_FILE = os.path.join(RESOURCES_PATH, 'keys.json')
KEYS_PULIC_FILE = os.path.join(RESOURCES_PATH, "keys_public.json")

THUMBNAILS_PATH = os.path.join(RESOURCES_PATH, 'thumbnails')
THUMBNAILS_RESIZED_PATH = os.path.join(THUMBNAILS_PATH, 'resized')
THUMBNAIL_NA_PATH = os.path.join(RESOURCES_PATH, 'thumbnail_na.png')
THUMBNAIL_404_PATH = os.path.join(RESOURCES_PATH, 'quality404.jpg')

