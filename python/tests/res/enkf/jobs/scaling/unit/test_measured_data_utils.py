import res.enkf.jobs.scaling.measured_data_utils as mdu
from tests.res.enkf.jobs.scaling.unit.mocked_block_observation import (
    MockedBlockObservation,
)
import sys
import pandas as pd
import pytest

if sys.version_info >= (3, 3):
    from unittest.mock import Mock, MagicMock, patch
else:
    from mock import Mock, MagicMock, patch


def create_expected_data():
    return pd.DataFrame(
        [[10.0, None, 10.0, 10.0], [1.0, None, 1.0, 1.0], [10.0, 10.0, 10.0, 10.0]],
        index=["OBS", "STD", 0],
    )


def create_summary_obs():
    return pd.DataFrame(
        [[10.0, None, 10.0, 10.0], [1.0, None, 1.0, 1.0]], index=["OBS", "STD"]
    )


def mocked_obs_node_get_index_nr(nr):
    return {0: 0, 1: 2, 2: 3}[nr]


@pytest.mark.parametrize(
    "obs_type,expected_loader",
    [
        ("GEN_OBS", mdu._get_general_data),
        ("SUMMARY_OBS", mdu._get_summary_data),
        ("BLOCK_OBS", mdu._get_block_data),
    ],
)
def test_data_loader_factory(obs_type, expected_loader):
    assert mdu.data_loader_factory(obs_type) == expected_loader


def test_data_loader_factory_fails():
    with pytest.raises(TypeError):
        mdu.data_loader_factory("BAD_TYPE")


@pytest.mark.usefixtures("facade")
def test_get_general_data(facade, monkeypatch):
    mock_node = MagicMock()
    mock_node.__len__.return_value = 3
    mock_node.get_data_points.return_value = [10.0, 10.0, 10.0]
    mock_node.get_std.return_value = [1.0, 1.0, 1.0]
    mock_node.getIndex.side_effect = mocked_obs_node_get_index_nr

    with patch("res.enkf.jobs.scaling.measured_data_utils.GenDataCollector") as mock_dc:
        mock_dc.loadGenData.return_value = pd.DataFrame(data=[10, 10, 10, 10])
        facade.obs()["some_key"].getNode.return_value = mock_node

        result = mdu._get_general_data(facade, "some_key", "test_case")

        mock_dc.loadGenData.assert_called_once_with(
            facade.impl(), "test_case", "test_data_key", 1
        )
        mock_node.get_data_points.assert_called_once()
        mock_node.get_std.assert_called_once()

    assert result.equals(create_expected_data())


@pytest.mark.usefixtures("facade")
def test_get_block_data(facade, monkeypatch):
    mocked_get_block_measured = Mock(
        return_value=pd.DataFrame(data=[[10.0, 10.0, 10.0, 10.0]])
    )

    monkeypatch.setattr(mdu, "_get_block_measured", mocked_get_block_measured)

    with patch(
        "res.enkf.jobs.scaling.measured_data_utils.PlotBlockDataLoader"
    ) as mock_dl:
        mock_dl_instance = mock_dl()
        block_data = Mock()
        mock_dl_instance.load.return_value = block_data

        mocked_block_obs = MockedBlockObservation(
            {"values": [10.0, None, 10.0, 10.0], "stds": [1.0, None, 1.0, 1.0]}
        )
        mock_dl_instance.getBlockObservation.return_value = mocked_block_obs

        result = mdu._get_block_data(facade, "some_key", "a_random_name")
        mocked_get_block_measured.assert_called_once_with(facade.ens_size(), block_data)
        assert result.equals(create_expected_data())


@pytest.mark.usefixtures("facade")
def test_get_summary_data(facade, monkeypatch):
    mocked_get_summary_obs = Mock(return_value=create_summary_obs())
    mocked_get_summary_measured = Mock(
        return_value=pd.DataFrame(data=[[10.0, 10.0, 10.0, 10.0]])
    )

    monkeypatch.setattr(mdu, "_add_summary_observations", mocked_get_summary_obs)
    monkeypatch.setattr(mdu, "_add_summary_data", mocked_get_summary_measured)

    result = mdu._get_summary_data(facade, "some_key", "a_random_name")
    mocked_get_summary_obs.assert_called_once_with(
        facade, facade.data_key_for_obs_key("some_key"), "a_random_name"
    )
    mocked_get_summary_measured.assert_called_once_with(
        facade, facade.data_key_for_obs_key("some_key"), "a_random_name"
    )
    assert result.equals(create_expected_data())
