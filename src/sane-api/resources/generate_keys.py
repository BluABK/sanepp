import threading

from handlers.log_handler import create_logger
from youtube_auth import youtube_auth_keys

logger = create_logger(__name__)


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
