import sys
import pytest
import os
import shutil

from res.enkf import ResConfig

from tests.conftest import source_root

if sys.version_info >= (3, 3):
    from unittest.mock import Mock, patch
else:
    from mock import Mock, patch

_TEST_DATA_DIR = os.path.join(source_root(), "test-data")


@pytest.fixture()
def facade():
    obs_mock = Mock()
    obs_mock.getDataKey.return_value = "test_data_key"
    obs_mock.getStepList.return_value.asList.return_value = [1]

    facade = Mock()
    facade.impl.return_value = Mock()
    facade.ens_size.return_value = 3
    facade.obs.return_value = {"some_key": obs_mock}

    facade.current_case_name.return_value = "test_case"

    return facade


@pytest.fixture()
def setup_ert(tmpdir):
    cwd = os.getcwd()
    tmpdir.chdir()
    test_data_dir = os.path.join(_TEST_DATA_DIR, "local", "snake_oil")
    shutil.copytree(test_data_dir, "test_data")
    os.chdir(os.path.join("test_data"))

    res_config = ResConfig("snake_oil.ert")
    yield res_config

    os.chdir(cwd)
