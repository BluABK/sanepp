"""
Passthrough kwargs directly to the YouTube API at https://www.googleapis.com/youtube/v3/
"""

from resources.youtube_auth import load_key


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


def youtube_api_channels_list(**kwargs):
    """
    Get a youtube#channelListResponse,
    :param kwargs:
    :return: youtube#channelListResponse
    """
    youtube_key = load_key()
    kwargs = remove_empty_kwargs(**kwargs)

    response = youtube_key.channels().list(**kwargs).execute()

    return response
