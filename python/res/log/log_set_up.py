import logging
import os

def set_up_log(log_file, level):
    log_formatter = logging.Formatter(
        '%(levelname)s: %(asctime)s %(name)s %(message)s',
        "%m/%d - %H:%M:%S"
    )

    log_path = os.path.split(log_file)[0]
    try:
        os.makedirs(log_path)
    except OSError:
        if not os.path.isdir(log_path):
            raise

    if not isinstance(level, int):
        DeprecationWarning("Use of {} is deprecated, use integer ({}) instead".format(level, int(level)))
        level = int(level)

    root_logger = logging.getLogger()
    root_logger.setLevel(logging.DEBUG)
    root_logger.addHandler(_set_up_file_logging(level, log_file, log_formatter))
    root_logger.addHandler(_set_up_console(log_formatter))


def _set_up_console(log_formatter):
    console_handler = logging.StreamHandler()
    console_handler.setLevel(logging.ERROR)
    console_handler.setFormatter(log_formatter)
    return console_handler


def _set_up_file_logging(level, log_file, log_formatter):
    file_handler = logging.FileHandler(log_file)
    file_handler.setLevel(level)
    file_handler.setFormatter(log_formatter)
    return file_handler
