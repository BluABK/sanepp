import ast
import copy
import os
from configparser import ConfigParser, NoSectionError, NoOptionError
from sane_legacy.absolute_paths import CONFIG_PATH, SAMPLE_PATH

default_parser = ConfigParser()
default_parser.read(CONFIG_PATH)

DEFAULTS = {
    'Debug': {
        'debug': 'False',
    },
    'Logging': {
        'use_socket_log': 'False',
        'log_level': '1',
        'logging_port': '19996'
    },
    'Authentication': {
        'oauth2_local_server_port': '10600'
    },
    'OAuth2Scopes': {
        'youtube': 'True',
        'youtube.force-ssl': 'True',
        'youtube.readonly': 'True',
        'youtube.upload': 'True',
        'youtubepartner': 'True',
        'youtubepartner-channel-audit': 'True'
    }
}


def create_config_file(file_path, template: dict):
    """
    Creates a config file in file_path using the given template dict.
    :param file_path:
    :param template:
    :return:
    """
    cfg_parser = ConfigParser()

    if file_path == SAMPLE_PATH:
        # Special handling required for config.ini.sample
        for cfg_section in DEFAULTS:
            if cfg_section == 'Database':
                # Mask database path so it's not included in sample ini file
                modified_section = copy.deepcopy(DEFAULTS[cfg_section])
                modified_section['url'] = 'sqlite:///<path>/permanents.db'
                cfg_parser[cfg_section] = modified_section
            else:
                cfg_parser[cfg_section] = DEFAULTS[cfg_section]
    else:
        cfg_parser.read_dict(template)

    with open(file_path, 'w') as config_file:
        cfg_parser.write(config_file)


def read_config(section, option, literal_eval=True):
    """
    Reads a configuration file (INI format)
    :param section:
    :param option:
    :param literal_eval: eval config literally, instead of string
    :param custom_ini: if set, use given custom config
    :return:
    """
    defaults = DEFAULTS
    parser = default_parser

    if literal_eval:
        try:
            value = parser.get(section, option)
        except (NoSectionError, NoOptionError):
            value = defaults[section][option]
        if value:
            try:
                return ast.literal_eval(value)
            except ValueError:
                return value
        else:
            return ast.literal_eval(defaults[section][option])
    else:
        try:
            value = parser.get(section, option)
        except (NoSectionError, NoOptionError):
            value = defaults[section][option]
        if value:
            try:
                return value
            except ValueError:
                return value
        else:
            return defaults[section][option]


def read_entire_config():
    """
    Reads the entire config file into a nested dict-list-dict
    :return:
    """
    _parser = default_parser

    config = {}
    for section in _parser.sections():
        config[section] = []
        section_option = {}
        for option in _parser.options(section):
            value = read_config(section, option)
            section_option[option] = value
            config[section].append(section_option)

    return config


def has_section(section):
    """
    Checks if a section exists in config
    :return:
    """
    _parser = default_parser

    return _parser.has_section(section)


def get_sections():
    """
    Returns config sections
    :return:
    """
    _parser = default_parser

    return _parser.sections()


def get_options(section, custom_ini=None):
    """
    Returns a list of ALL options (valid or not) in a given config section.

    A valid option is defined as one that exists and is not a blank string.
    :return:
    """
    _parser = default_parser
    if custom_ini is not None:
        raise ValueError("Custom config '{}' is not defined in handler!!".format(custom_ini))
    if _parser.has_section(section):
        return_options = _parser.options(section)
    else:
        return_options = DEFAULTS[section].keys()

    return return_options


def get_valid_options(config_section):
    """
    Returns a list of ONLY VALID options in a given config section.

    A valid option is defined as one that exists and is not a blank string.
    :param config_section:
    :return:
    """
    valid_options = []
    for proxy_option in get_options(config_section):
        # Retrieve the config option (and strip string characters).
        current_option = read_config(config_section, proxy_option, literal_eval=False)

        # Check that the current option is valid.
        if current_option is not "" and current_option is not None:
            # Append the option to a list of valid options.
            valid_options.append(current_option)

    return valid_options


def get_size(incl_sections=False):
    """
    Sums up sections and options
    :return:
    """
    _parser = default_parser

    size = 0
    for section in _parser.sections():
        if incl_sections:
            size += 1
        for _ in _parser.options(section):
            size += 1

    return size


def set_config(section, option, value):
    """
    Sets the given option's value
    :param section:
    :param option:
    :param value:
    :return:
    """
    if value is None:
        value = ""  # Keep a uniform format in the config
    _parser = default_parser

    try:
        _parser.set(section, option, value)
    except NoSectionError:
        _parser.add_section(section)
        _parser.set(section, option, value)
    with open(CONFIG_PATH, 'w') as config:
        _parser.write(config)


# Create sample config if none exists
if not os.path.exists(SAMPLE_PATH):
    create_config_file(SAMPLE_PATH, DEFAULTS)
