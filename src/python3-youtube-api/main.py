from handlers.log_handler import create_logger
from youtube.update_videos import refresh_uploads

logger = create_logger(__name__)

cached_subs = True
global_debug = False
global_info = False
info = True
debug = True
print_statistics = True


# Auth OAuth2 with YouTube API

# Create controller object

def run_with_cli():
    logger.info('Running with CLI')
    logger.error("CLI UI Not yet implemented")
    exit(0)


def cli_refresh_and_print_subfeed():
    logger.info('Running with print/console')
    new_videos = refresh_uploads()
    for vid in new_videos:
        print(vid)

