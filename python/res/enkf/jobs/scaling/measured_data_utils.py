import pandas as pd

from res.enkf.export import (
    GenDataCollector,
    SummaryCollector,
    SummaryObservationCollector,
)
from res.enkf.plot_data import PlotBlockDataLoader


def data_loader_factory(observation_type):
    """
    Currently, the methods returned by this factory differ. They should not.
    TODO: Remove discrepancies between returned methods.
        See https://github.com/equinor/libres/issues/808
    """
    if observation_type == "GEN_OBS":
        return _get_general_data
    elif observation_type == "SUMMARY_OBS":
        return _get_summary_data
    elif observation_type == "BLOCK_OBS":
        return _get_block_data
    else:
        raise TypeError("Unknown observation type: {}".format(observation_type))


def _get_general_data(facade, observation_key, case_name):
    obs_vector = facade.obs()[observation_key]
    data_key = obs_vector.getDataKey()

    data = pd.DataFrame()

    for time_step in obs_vector.getStepList().asList():
        # Fetch, then transpose the simulation data in order to make it
        # conform with the GenObservation data structure.
        gen_data = GenDataCollector.loadGenData(
            facade.impl(), case_name, data_key, time_step
        ).T

        # Observations and its standard deviation are a subset of the simulation data.
        # The index_list refers to indices in the simulation data. In order to
        # join these data in a DataFrame, pandas inserts the obs/std
        # data into the columns representing said indices, before appending
        # the simulation data.
        # You then get something like:
        #      0   1   2
        # OBS  NaN NaN 42
        # STD  NaN NaN 4.2
        #   0  1.2 2.3 7.5
        node = obs_vector.getNode(time_step)
        index_list = [node.getIndex(nr) for nr in range(len(node))]

        data = (
            data.append(
                pd.DataFrame(
                    [node.get_data_points()], columns=index_list, index=["OBS"]
                )
            )
            .append(pd.DataFrame([node.get_std()], columns=index_list, index=["STD"]))
            .append(gen_data)
        )
    return data


def _get_block_data(facade, observation_key, case_name):
    """
    _get_block_data is a part of the data_loader_factory, and the other
    methods returned by this factory, require case_name, so it is accepted
    here as well.
    """
    obs_vector = facade.obs()[observation_key]
    loader = PlotBlockDataLoader(obs_vector)

    data = pd.DataFrame()
    for report_step in obs_vector.getStepList().asList():

        block_data = loader.load(facade.current_fs(), report_step)
        obs_block = loader.getBlockObservation(report_step)

        data = (
            data.append(
                pd.DataFrame(
                    [[obs_block.getValue(i) for i in obs_block]], index=["OBS"]
                )
            )
            .append(
                pd.DataFrame([[obs_block.getStd(i) for i in obs_block]], index=["STD"])
            )
            .append(_get_block_measured(facade.ens_size(), block_data))
        )
    return data


def _get_summary_data(facade, observation_key, case_name):
    data_key = facade.data_key_for_obs_key(observation_key)
    return pd.concat(
        [
            _add_summary_observations(facade, data_key, case_name),
            _add_summary_data(facade, data_key, case_name),
        ]
    )


def _get_block_measured(ensamble_size, block_data):
    data = pd.DataFrame()
    for ensamble_nr in range(ensamble_size):
        data = data.append(pd.DataFrame([block_data[ensamble_nr]], index=[ensamble_nr]))
    return data


def _add_summary_data(facade, data_key, case_name):
    data = SummaryCollector.loadAllSummaryData(facade.impl(), case_name, [data_key])
    data = data[data_key].unstack(level=-1)
    return data.set_index(data.index.values)


def _add_summary_observations(facade, data_key, case_name):
    data = SummaryObservationCollector.loadObservationData(
        facade.impl(), case_name, [data_key]
    ).transpose()
    # The index from SummaryObservationCollector is {data_key} and STD_{data_key}"
    # to match the other data types this needs to be changed to OBS and STD, hence
    # the regex.
    data = data.set_index(data.index.str.replace(r"\b" + data_key, "OBS", regex=True))
    return data.set_index(data.index.str.replace("_" + data_key, ""))
