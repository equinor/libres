def valid_job(user_config, obs_list, obs_with_data):
    """
    Validates the job, assumes that the configuration is valid
    """

    calc_keys = [entry.key for entry in user_config.snapshot.CALCULATE_KEYS.keys]
    application_keys = [entry.key for entry in user_config.snapshot.UPDATE_KEYS.keys]

    error_messages = []

    error_messages.extend(_is_subset(calc_keys, application_keys))
    error_messages.extend(_has_keys(obs_list, calc_keys, "Key: {} has no observations"))
    error_messages.extend(_has_keys(obs_with_data, calc_keys, "Key: {} has no data"))

    for error in error_messages:
        print(error)
    return len(error_messages) == 0


def _has_keys(main_list, sub_list, error_msg):
    """
    Checks that all sub_list are present and returns a list of error messages
    """
    return [error_msg.format(key) for key in sub_list if key not in main_list]


def _is_subset(main_list, sub_list):
    """
    Checks if all the keys in sub_list are present in main_list and returns list of error
    messages
    """
    error_msg = "Update key: {} missing from calculate keys: {}"
    missing_keys = set(sub_list).difference(set(main_list))
    return [error_msg.format(missing_key, main_list) for missing_key in missing_keys]


def valid_configuration(user_config):
    """
    Validates the configuration
    """
    for error in user_config.errors:
        print(error)
    return user_config.valid
