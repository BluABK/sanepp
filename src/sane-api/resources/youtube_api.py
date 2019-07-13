"""
Pass-through kwargs directly to the YouTube API at https://www.googleapis.com/youtube/v3/
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


def youtube_api_channels_list(youtube_auth, **kwargs):
    """
    Get a youtube#channelListResponse,
    :param youtube_auth: Authenticated API key or OAuth2 token
    :param kwargs:
    :return: youtube#channelListResponse
    """
    kwargs = remove_empty_kwargs(**kwargs)

    response = youtube_auth.channels().list(**kwargs).execute()

    return response


def youtube_api_search(youtube_auth, **kwargs):
    """
    Get a channel by searching for it
    :param youtube_key:
    :param query:
    :return: searchList response items
    """
    youtube_auth = load_key()
    response = youtube_auth.search().list(**kwargs).execute()
    # response.execute()

    # return search_response['items']
    return response
