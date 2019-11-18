import sys
import pandas as pd
import pytest

from res.enkf.jobs.scaling.measured_data import MeasuredData
from res.enkf.jobs.scaling import measured_data_utils

if sys.version_info >= (3, 3):
    from unittest.mock import Mock
else:
    from mock import Mock

@pytest.mark.usefixtures("facade")
@pytest.mark.parametrize("obs_type", [("GEN_OBS"), ("SUMMARY_OBS"), ("BLOCK_OBS")])
def test_get_data(obs_type, monkeypatch, facade):

    facade.impl_type_name_for_obs_key.return_value = obs_type

    mocked_loader = Mock(return_value=pd.DataFrame(data=[[1, 2, 3]]))
    factory = Mock(return_value=mocked_loader)

    monkeypatch.setattr(measured_data_utils, "data_loader_factory", factory)

    md = MeasuredData(facade, ["test_key"], index_lists=[[1, 2]])

    factory.assert_called_once_with(obs_type)
    mocked_loader.assert_called_once_with(facade, "test_key", "test_case")
    assert (
        md.data
        == pd.concat({"test_key": pd.DataFrame(data=[[2, 3]], columns=[1, 2])}, axis=1)
    ).all
