import os
import shutil
from collections import namedtuple
from copy import deepcopy

import configsuite
import numpy as np
import pandas as pd
import pytest

from res.enkf import ResConfig, EnKFMain
from res.enkf.jobs.scaling import (
    scaling_job,
    job_config,
    measured_data,
    scaled_matrix,
    validator,
)
from tests.conftest import source_root

_TEST_DATA_DIR = os.path.join(source_root(), "test-data")


@pytest.fixture()
def setup_tmpdir(tmpdir):
    cwd = os.getcwd()
    tmpdir.chdir()
    yield
    os.chdir(cwd)


def get_config(index_list_calc=None, index_list_update=None):
    schema = job_config.build_schema()
    default_values = job_config.get_default_values()
    config = {
        "UPDATE_KEYS": {"keys": [{"key": "POLY_OBS"}]},
        "CALCULATE_KEYS": {"keys": [{"key": "POLY_OBS"}]},
    }

    if index_list_update:
        config["UPDATE_KEYS"]["keys"][0].update({"index": index_list_update})
    if index_list_calc:
        config["CALCULATE_KEYS"]["keys"][0].update({"index": index_list_calc})

    return configsuite.ConfigSuite(config, schema, layers=(default_values,)).snapshot


@pytest.mark.usefixtures("setup_ert")
def test_create_observation_vectors(setup_ert):

    valid_config_data = {
        "CALCULATE_KEYS": {"keys": [{"key": "WPR_DIFF_1"}]},
        "UPDATE_KEYS": {"keys": [{"key": "WPR_DIFF_1"}]},
    }
    config = configsuite.ConfigSuite(valid_config_data, job_config.build_schema())

    res_config = setup_ert
    ert = EnKFMain(res_config)
    obs = ert.getObservations()

    new_events = scaling_job._create_active_lists(obs, config.snapshot.UPDATE_KEYS.keys)

    keys = [event.key for event in new_events]

    assert "WPR_DIFF_1" in keys
    assert "SNAKE_OIL_WPR_DIFF" not in keys


@pytest.mark.parametrize(
    "valid_input",
    [
        "0,1,2-5",
        [0, 1, "2-5"],
        [0, 1, 2, 3, 4, 5],
        [0, 1, "2-3", "4-5"],
        "0-5",
        "0-1,2,3-5",
        ["0,1,2-5"],
    ],
)
def test_to_int_list(valid_input):
    expected_result = list(range(6))
    assert job_config._to_int_list(valid_input) == expected_result


@pytest.mark.parametrize(
    "test_input,expected_result", [(-1, False), (0, True), (1, True)]
)
def test_min_value(test_input, expected_result):
    assert job_config._min_value(test_input).__bool__() == expected_result


def test_expand_input():

    expected_result = {
        "UPDATE_KEYS": {"keys": [{"key": "key_4"}, {"key": "key_5"}, {"key": "key_6"}]},
        "CALCULATE_KEYS": {
            "keys": [{"key": "key_1"}, {"key": "key_2"}, {"key": "key_3"}]
        },
    }

    valid_config = deepcopy(expected_result)

    assert job_config._expand_input(deepcopy(expected_result)) == expected_result

    copy_of_valid_config = deepcopy(valid_config)
    copy_of_valid_config.pop("UPDATE_KEYS")

    expected_result = {
        "UPDATE_KEYS": {"keys": [{"key": "key_1"}, {"key": "key_2"}, {"key": "key_3"}]},
        "CALCULATE_KEYS": {
            "keys": [{"key": "key_1"}, {"key": "key_2"}, {"key": "key_3"}]
        },
    }

    assert job_config._expand_input(copy_of_valid_config) == expected_result


def test_config_setup():

    valid_config_data = {
        "CALCULATE_KEYS": {"keys": [{"key": "first_key"}, {"key": "second_key"}]}
    }

    schema = job_config.build_schema()
    config = configsuite.ConfigSuite(valid_config_data, schema)
    assert config.valid

    valid_config_data = {
        "CALCULATE_KEYS": {"keys": [{"key": "first_key"}, {"key": "second_key"}]},
        "UPDATE_KEYS": {"keys": [{"index": [1, 2, 3], "key": "first_key"}]},
    }

    schema = job_config.build_schema()
    config = configsuite.ConfigSuite(valid_config_data, schema)
    assert config.valid

    invalid_too_short_index_list = {
        "UPDATE_KEYS": {"keys": [{"index": "1", "key": ["a_key"]}]}
    }

    config = configsuite.ConfigSuite(invalid_too_short_index_list, schema)
    assert not config.valid

    invalid_missing_required_keyword = {
        "CALCULATE_KEYS": {"keys": [{"key": "a_key"}]},
        "UPDATE_KEYS": {"index": "1-5"},
    }

    config = configsuite.ConfigSuite(invalid_missing_required_keyword, schema)
    assert not config.valid

    invalid_negative_index = {
        "CALCULATE_KEYS": {"keys": [{"key": "first_key"}, {"key": "second_key"}]},
        "UPDATE_KEYS": {"keys": [{"index": [-1, 2, 3], "key": "first_key"}]},
    }

    schema = job_config.build_schema()
    config = configsuite.ConfigSuite(invalid_negative_index, schema)
    assert not config.valid


def test_get_scaling_factor():
    new_event = namedtuple("named_dict", ["keys", "threshold"])
    event = new_event(["one_random_key"], 0.95)
    np.random.seed(123)
    input_matrix = np.random.rand(10, 10)

    matrix = scaled_matrix.DataMatrix(pd.DataFrame(data=input_matrix))

    assert matrix.get_scaling_factor(event) == np.sqrt(10 / 4.0)


@pytest.mark.parametrize(
    "threshold,expected_result", [(0.0, 1), (0.83, 2), (0.90, 3), (0.95, 4), (0.99, 6)]
)
def test_get_nr_primary_components(threshold, expected_result):
    np.random.seed(123)
    input_matrix = np.random.rand(10, 10)
    matrix = scaled_matrix.DataMatrix
    assert matrix._get_nr_primary_components(input_matrix, threshold) == expected_result


def test_std_normalization():
    input_matrix = pd.DataFrame(np.ones((3, 3)))
    input_matrix.loc["OBS"] = np.ones(3)
    input_matrix.loc["STD"] = np.ones(3) * 0.1
    expected_matrix = [[10.0, 10.0, 10.0], [10.0, 10.0, 10.0], [10.0, 10.0, 10.0]]
    matrix = scaled_matrix.DataMatrix(pd.concat({"A_KEY": input_matrix}, axis=1))
    result = matrix.std_normalization(["A_KEY"])
    assert (result.loc[[0, 1, 2]].values == expected_matrix).all()


def test_filter_on_column_index():
    matrix = np.random.rand(10, 10)

    index_lists = [[0, 1], [1, 2, 3], [1, 2, 3, 4, 5]]
    for index_list in index_lists:
        result = measured_data.MeasuredData.filter_on_column_index(
            pd.DataFrame(matrix), index_list
        )
        assert result.shape == (10, len(index_list))

    with pytest.raises(IndexError):
        measured_data.MeasuredData.filter_on_column_index(pd.DataFrame(matrix), [11])


@pytest.mark.parametrize(
    "matching_keys,entry,expected_result",
    [
        (["a_key"], {"key": "a_*"}, [{"key": "a_key"}]),
        (["a_key", "b_key"], {"key": "*key"}, [{"key": "a_key"}, {"key": "b_key"}]),
        (
            ["a_key"],
            {"key": "a_*", "index": [1, 2]},
            [{"key": "a_key", "index": [1, 2]}],
        ),
    ],
)
def test_wildcard_to_dict_list(matching_keys, entry, expected_result):
    assert scaling_job._wildcard_to_dict_list(matching_keys, entry) == expected_result


def test_find_and_expand_wildcards():
    expected_dict = {
        "ANOTHER_KEY": "something",
        "CALCULATE_KEYS": {
            "keys": [
                {"key": "WOPR_OP1_108"},
                {"key": "WOPR_OP1_144"},
                {"key": "WOPR_OP1_190"},
                {"key": "WOPR_OP1_9"},
                {"key": "WOPR_OP1_36"},
                {"key": "WOPR_OP1_72"},
                {"key": "FOPR"},
            ]
        },
        "UPDATE_KEYS": {
            "keys": [
                {"key": "WOPR_OP1_108"},
                {"key": "WOPR_OP1_144"},
                {"key": "WOPR_OP1_190"},
                {"key": "FOPR"},
            ]
        },
    }

    user_config = {
        "ANOTHER_KEY": "something",
        "CALCULATE_KEYS": {"keys": [{"key": "WOPR_*"}, {"key": "FOPR"}]},
        "UPDATE_KEYS": {"keys": [{"key": "WOPR_OP1_1*"}, {"key": "FOPR"}]},
    }

    observation_list = [
        "WOPR_OP1_108",
        "WOPR_OP1_144",
        "WOPR_OP1_190",
        "WOPR_OP1_9",
        "WOPR_OP1_36",
        "WOPR_OP1_72",
        "FOPR",
    ]

    result_dict = scaling_job._find_and_expand_wildcards(observation_list, user_config)

    assert result_dict == expected_dict


@pytest.mark.usefixtures("setup_tmpdir")
def test_add_observation_vectors():

    valid_config_data = {"UPDATE_KEYS": {"keys": [{"key": "WOPR_OP1_108"}]}}

    schema = job_config.build_schema()
    config = configsuite.ConfigSuite(valid_config_data, schema)

    test_data_dir = os.path.join(_TEST_DATA_DIR, "local", "snake_oil_field")

    shutil.copytree(test_data_dir, "test_data")
    os.chdir(os.path.join("test_data"))

    res_config = ResConfig("snake_oil.ert")

    ert = EnKFMain(res_config)

    obs = ert.getObservations()

    new_events = scaling_job._create_active_lists(obs, config.snapshot.UPDATE_KEYS.keys)

    keys = [event.key for event in new_events]

    assert "WOPR_OP1_108" in keys
    assert "WOPR_OP1_144" not in keys


@pytest.mark.usefixtures("setup_tmpdir")
def test_validate_failed_realizations():
    """
    Config has several failed realisations
    """
    test_data_dir = os.path.join(_TEST_DATA_DIR, "local", "custom_kw")
    shutil.copytree(test_data_dir, "test_data")
    os.chdir(os.path.join("test_data"))

    res_config = ResConfig("mini_fail_config")
    ert = EnKFMain(res_config)
    observations = ert.getObservations()

    result = scaling_job.keys_with_data(
        observations,
        ["GEN_PERLIN_1"],
        ert.getEnsembleSize(),
        ert.getEnkfFsManager().getCurrentFileSystem(),
    )
    assert result == ["GEN_PERLIN_1"]


@pytest.mark.usefixtures("setup_tmpdir")
def test_validate_no_realizations():
    """
    Ensamble has not run
    """
    test_data_dir = os.path.join(_TEST_DATA_DIR, "local", "poly_normal")
    shutil.copytree(test_data_dir, "test_data")
    os.chdir(os.path.join("test_data"))

    res_config = ResConfig("poly.ert")
    ert = EnKFMain(res_config)
    observations = ert.getObservations()

    result = scaling_job.keys_with_data(
        observations,
        ["POLY_OBS"],
        ert.getEnsembleSize(),
        ert.getEnkfFsManager().getCurrentFileSystem(),
    )
    assert result == []
