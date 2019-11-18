import os
import shutil

import configsuite
import numpy as np
import pytest
import yaml

from res.enkf import ResConfig, EnKFMain
from res.enkf.jobs.scaling import scaling_job, job_config, validator
from res.enkf.jobs.scaling.ert_facade.facade import ErtFacade
from tests.conftest import source_root

_TEST_DATA_DIR = os.path.join(source_root(), "test-data")


def assert_obs_vector(vector, val_1, index_list=None, val_2=None):
    if index_list is None:
        index_list = []
    for node in vector:
        for index in range(len(node)):
            if index in index_list:
                assert node.getStdScaling(index) == val_2
            else:
                assert node.getStdScaling(index) == val_1


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
def test_old_enkf_scaling_job(setup_ert):

    res_config = setup_ert
    ert = EnKFMain(res_config)

    obs = ert.getObservations()
    obs_vector = obs["WPR_DIFF_1"]

    assert_obs_vector(obs_vector, 1.0)

    job = ert.getWorkflowList().getJob("STD_SCALE_CORRELATED_OBS")
    job.run(ert, ["WPR_DIFF_1"])

    assert_obs_vector(obs_vector, np.sqrt(4.0 / 2.0))


@pytest.mark.skip("Util abort")
@pytest.mark.usefixtures("setup_ert")
def test_installed_python_version_of_enkf_scaling_job(setup_ert, monkeypatch):
    res_config = setup_ert
    ert = EnKFMain(res_config)

    obs = ert.getObservations()
    obs_vector = obs["WPR_DIFF_1"]

    assert_obs_vector(obs_vector, 1.0)

    job_config = {"CALCULATE_KEYS": {"keys": [{"key": "WPR_DIFF_1"}]}}

    with open("job_config.yml", "w") as fout:
        yaml.dump(job_config, fout)

    job = ert.getWorkflowList().getJob("CORRELATE_OBSERVATIONS")
    job.run(ert, ["job_config.yml"])

    assert_obs_vector(obs_vector, np.sqrt(4.0 / 2.0))

    job_config["CALCULATE_KEYS"]["keys"][0].update({"index": [400, 800, 1200]})
    with open("job_config.yml", "w") as fout:
        yaml.dump(job_config, fout)
    job.run(ert, ["job_config.yml"])

    assert_obs_vector(
        obs_vector,
        np.sqrt(4.0 / 2.0),
        index_list=job_config["CALCULATE_KEYS"]["keys"][0]["index"],
        val_2=np.sqrt(3.0 / 2.0),
    )


@pytest.mark.usefixtures("setup_tmpdir")
def test_compare_different_jobs():

    arguments = {"CALCULATE_KEYS": {"keys": [{"key": "WPR_DIFF_1"}]}}

    test_data_dir = os.path.join(_TEST_DATA_DIR, "local", "snake_oil")

    shutil.copytree(test_data_dir, "test_data")
    os.chdir(os.path.join("test_data"))

    res_config = ResConfig("snake_oil.ert")

    ert = EnKFMain(res_config)
    facade = ErtFacade(ert)
    obs = ert.getObservations()
    obs_vector = obs["WPR_DIFF_1"]

    assert_obs_vector(obs_vector, 1.0)

    job = ert.getWorkflowList().getJob("STD_SCALE_CORRELATED_OBS")
    job.run(ert, ["WPR_DIFF_1"])

    # Result of old job:
    assert_obs_vector(obs_vector, np.sqrt(4 / 2))

    scaling_job.scaling_job(facade, arguments)

    # Result of new job with no sub-indexing:
    assert_obs_vector(obs_vector, np.sqrt(4 / 2))


@pytest.mark.usefixtures("setup_tmpdir")
def test_main_entry_point_gen_data():

    arguments = {
        "CALCULATE_KEYS": {"keys": [{"key": "WPR_DIFF_1"}]},
        "UPDATE_KEYS": {"keys": [{"key": "WPR_DIFF_1", "index": [0, 1]}]},
    }

    test_data_dir = os.path.join(_TEST_DATA_DIR, "local", "snake_oil")

    shutil.copytree(test_data_dir, "test_data")
    os.chdir(os.path.join("test_data"))

    res_config = ResConfig("snake_oil.ert")

    ert = EnKFMain(res_config)
    facade = ErtFacade(ert)

    scaling_job.scaling_job(facade, arguments)

    obs = ert.getObservations()
    obs_vector = obs["WPR_DIFF_1"]

    assert_obs_vector(
        obs_vector, 1.0, arguments["UPDATE_KEYS"]["keys"][0]["index"], np.sqrt(4 / 2)
    )

    arguments["CALCULATE_KEYS"]["keys"][0].update({"index": [400, 800, 1200]})
    scaling_job.scaling_job(facade, arguments)
    assert_obs_vector(
        obs_vector,
        1.0,
        arguments["UPDATE_KEYS"]["keys"][0]["index"],
        np.sqrt(3.0 / 2.0),
    )


@pytest.mark.usefixtures("setup_tmpdir")
def test_main_entry_point_summary_data_calc():

    arguments = {
        "CALCULATE_KEYS": {"keys": [{"key": "WOPR_OP1_108"}, {"key": "WOPR_OP1_144"}]}
    }

    test_data_dir = os.path.join(_TEST_DATA_DIR, "local", "snake_oil")

    shutil.copytree(test_data_dir, "test_data")
    os.chdir(os.path.join("test_data"))

    res_config = ResConfig("snake_oil.ert")

    ert = EnKFMain(res_config)
    facade = ErtFacade(ert)
    obs = ert.getObservations()

    obs_vector = obs["WOPR_OP1_108"]

    for index, node in enumerate(obs_vector):
        assert node.getStdScaling(index) == 1.0

    scaling_job.scaling_job(facade, arguments)

    for index, node in enumerate(obs_vector):
        assert node.getStdScaling(index) == np.sqrt((2.0 * 6.0) / 2.0)

    arguments["CALCULATE_KEYS"]["keys"][0].update({"index": [1, 2, 3]})
    arguments["CALCULATE_KEYS"]["keys"][1].update({"index": [1, 2, 3]})

    with pytest.raises(ValueError):  # Will give an empty data set
        scaling_job.scaling_job(facade, arguments)

    arguments["CALCULATE_KEYS"]["keys"][0].update({"index": [8, 35, 71]})
    arguments["CALCULATE_KEYS"]["keys"][1].update({"index": [8, 35, 71]})
    scaling_job.scaling_job(facade, arguments)

    for index, node in enumerate(obs_vector):
        if index in arguments["CALCULATE_KEYS"]["keys"][0]["index"]:
            assert node.getStdScaling(index) == np.sqrt((2.0 * 6.0) / 1.0)
        else:
            assert node.getStdScaling(index) == np.sqrt((2.0 * 6.0) / 2.0)


@pytest.mark.equinor_test
@pytest.mark.usefixtures("setup_tmpdir")
def test_main_entry_point_summary_data_update():
    arguments = {
        "CALCULATE_KEYS": {"keys": [{"key": "WWCT:OP_1"}, {"key": "WWCT:OP_2"}]},
        "UPDATE_KEYS": {"keys": [{"key": "WWCT:OP_2", "index": [1, 2, 3, 4, 5]}]},
    }

    test_data_dir = os.path.join(_TEST_DATA_DIR, "Equinor", "config", "obs_testing")

    shutil.copytree(test_data_dir, "test_data")
    os.chdir(os.path.join("test_data"))

    res_config = ResConfig("config")
    ert = EnKFMain(res_config)
    facade = ErtFacade(ert)
    obs = ert.getObservations()
    obs_vector = obs["WWCT:OP_2"]

    scaling_job.scaling_job(facade, arguments)

    for index, node in enumerate(obs_vector):
        if index in arguments["UPDATE_KEYS"]["keys"][0]["index"]:
            assert node.getStdScaling(index) == np.sqrt(61.0 * 2.0)
        else:
            assert node.getStdScaling(index) == 1.0

    obs_vector = obs["WWCT:OP_1"]

    scaling_job.scaling_job(facade, arguments)

    for index, node in enumerate(obs_vector):
        assert node.getStdScaling(index) == 1.0


@pytest.mark.equinor_test
@pytest.mark.usefixtures("setup_tmpdir")
def test_main_entry_point_block_data_calc():
    arguments = {"CALCULATE_KEYS": {"keys": [{"key": "RFT3"}]}}

    test_data_dir = os.path.join(_TEST_DATA_DIR, "Equinor", "config", "with_RFT")

    shutil.copytree(test_data_dir, "test_data")
    os.chdir(os.path.join("test_data"))

    res_config = ResConfig("config")
    ert = EnKFMain(res_config)
    facade = ErtFacade(ert)
    obs = ert.getObservations()

    obs_vector = obs["RFT3"]

    for index, node in enumerate(obs_vector):
        assert node.getStdScaling(index) == 1.0

    scaling_job.scaling_job(facade, arguments)

    for index, node in enumerate(obs_vector):
        assert node.getStdScaling(index) == 2.0


@pytest.mark.equinor_test
@pytest.mark.usefixtures("setup_tmpdir")
def test_main_entry_point_block_and_summary_data_calc():
    arguments = {"CALCULATE_KEYS": {"keys": [{"key": "FOPT"}, {"key": "RFT3"}]}}

    test_data_dir = os.path.join(_TEST_DATA_DIR, "Equinor", "config", "with_RFT")

    shutil.copytree(test_data_dir, "test_data")
    os.chdir(os.path.join("test_data"))

    res_config = ResConfig("config")
    ert = EnKFMain(res_config)
    facade = ErtFacade(ert)
    obs = ert.getObservations()

    obs_vector = obs["RFT3"]

    for index, node in enumerate(obs_vector):
        assert node.getStdScaling(index) == 1.0

    scaling_job.scaling_job(facade, arguments)

    for index, node in enumerate(obs_vector):
        assert node.getStdScaling(index) == np.sqrt(65)


def test_valid_configuration():

    valid_config_data = {
        "CALCULATE_KEYS": {"keys": [{"key": "POLY_OBS"}]},
        "UPDATE_KEYS": {"keys": [{"key": "POLY_OBS"}]},
    }

    schema = job_config.build_schema()
    config = configsuite.ConfigSuite(valid_config_data, schema)

    assert validator.valid_configuration(config)
