import sys
import pytest

from res.enkf.jobs.scaling import validator

if sys.version_info >= (3, 3):
    from unittest.mock import Mock
else:
    from mock import Mock


@pytest.mark.parametrize(
    "test_input,expected_result",
    [
        (["POLY_OBS"], 0),
        (["NOT_A_KEY"], 1),
        (["POLY_OBS", "NOT_A_KEY"], 1),
        (["POLY_NOT", "NOT_A_KEY"], 2),
    ],
)
def test_has_keys(test_input, expected_result):
    obs = ["POLY_OBS"]
    msg = "fail_message"
    assert len(validator._has_keys(obs, test_input, msg)) == expected_result


@pytest.mark.parametrize(
    "input_list,result_list", [(["a"], []), (["a", "c"], []), (["a", "b", "c"], [])]
)
def test_is_subset_valid(input_list, result_list):
    example_list = ["a", "b", "c"]

    assert validator._is_subset(example_list, input_list) == result_list


@pytest.mark.parametrize(
    "input_list,list_length", [(["d"], 1), (["d", "e"], 2), (["a", "b", "d"], 1)]
)
def test_is_subset_invalid(input_list, list_length):
    example_list = ["a", "b", "c"]

    assert len(validator._is_subset(example_list, input_list)) == list_length


@pytest.mark.parametrize(
    "calc_key,app_key,obs_list,data_list,expected_result",
    [
        ("KEY_1", "KEY_1", ["KEY_1"], ["KEY_1"], True),
        ("KEY_1", "KEY_1", ["KEY_1", "KEY_2"], ["KEY_1"], True),
        ("KEY_1", "KEY_1", ["KEY_1", "KEY_2"], ["KEY_2"], False),
        ("not_in_list", "KEY_1", ["KEY_1"], ["KEY_1"], False),
        ("KEY_1", "not_in_list", ["KEY_1"], ["KEY_1"], False),
        ("KEY_1", "KEY_1", [], ["KEY_1"], False),
        ("KEY_1", "KEY_1", ["KEY_1"], [], False),
    ],
)
def test_valid_job(calc_key, app_key, obs_list, data_list, expected_result):

    mock_entry_1 = Mock()
    mock_entry_1.key = calc_key

    mock_entry_2 = Mock()
    mock_entry_2.key = app_key

    user_config = Mock()
    user_config.snapshot.CALCULATE_KEYS.keys = [mock_entry_1]
    user_config.snapshot.UPDATE_KEYS.keys = [mock_entry_2]

    assert validator.valid_job(user_config, obs_list, data_list) is expected_result
