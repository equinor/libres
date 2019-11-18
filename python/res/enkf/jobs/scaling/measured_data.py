import pandas as pd

from res.enkf.jobs.scaling import measured_data_utils


class MeasuredData(object):
    def __init__(self, facade, keys, index_lists=None):
        self._facade = facade
        self.data = self._get_data(keys, index_lists)

    def remove_nan_and_filter(self, keys, std_cutoff, alpha):
        self.remove_nan()
        self.filter_out_outliers(keys, std_cutoff, alpha)

    def remove_nan(self):
        """
        Removes NaN values from the case, first on a row basis to remove failed realizations,
        then on a column basis.
        """
        self.data[~self.data.index.isin(["OBS", "STD"])] = self.data[
            ~self.data.index.isin(["OBS", "STD"])
        ].dropna(axis=0, how="all")
        self.data = self.data.dropna(axis=1)
        if self.data.shape[1] == 0:
            raise ValueError("Empty dataset, all data has been filtered out")

    def filter_out_outliers(self, keys, std_cutoff, alpha):
        self.data = self._filter_out_outliers(keys, std_cutoff, alpha)

    def _get_data(self, observation_keys, index_lists):
        """
        Adds simulated and observed data and returns a dataframe where ensamble members will
        have a data key, observed data will be named OBS and observated standard
        deviation will be named STD.
        """
        measured_data = pd.DataFrame()
        case_name = self._facade.current_case_name()

        if index_lists is None:
            index_lists = [None] * len(observation_keys)

        if len(index_lists) != len(observation_keys):
            raise ValueError("index list must be same length as observations keys")

        for key, index_list in zip(observation_keys, index_lists):
            observation_type = self._facade.impl_type_name_for_obs_key(key)
            data_loader = measured_data_utils.data_loader_factory(observation_type)

            data = data_loader(self._facade, key, case_name)
            data = MeasuredData.filter_on_column_index(data, index_list)
            data = pd.concat({key: data}, axis=1)

            measured_data = pd.concat([measured_data, data], axis=1)

        return measured_data

    def _filter_out_outliers(self, keys, std_cutoff, alpha):
        """
        Goes through the observation keys and filters out outliers. It first extracts
        key data such as ensamble mean and std, and observation values and std. It creates
        a filtered index which is a pandas series of indexes where the data will be removed.
        This can have duplicates of indicies.
        """
        filters = []

        for key in keys:
            ens_data = self.data[key][~self.data[key].index.isin(["OBS", "STD"])]

            ens_mean = ens_data.mean(axis=0)
            ens_std = ens_data.std(axis=0)
            obs_values = self.data[key].loc["OBS"]
            obs_std = self.data[key].loc["STD"]

            filters.append(self._filter_ensamble_std(ens_std, std_cutoff))
            filters.append(
                self._filter_ens_mean_obs(ens_mean, ens_std, obs_values, obs_std, alpha)
            )

        combined_filter = self._combine_filters(filters)
        return self.data.drop(columns=combined_filter[combined_filter].index)

    @staticmethod
    def _filter_ensamble_std(ensamble_std, std_cutoff):
        """
        Filters on ensamble variation versus a user defined standard
        deviation cutoff.
        """
        return ensamble_std < std_cutoff

    @staticmethod
    def _filter_ens_mean_obs(
        ensamble_mean, ensamble_std, observation_values, observation_std, alpha
    ):
        """
        Filters on distance between the observed data and the ensamble mean based on variation and
        a user defined alpha.
        """
        return abs(observation_values - ensamble_mean) > alpha * (
            ensamble_std + observation_std
        )

    @staticmethod
    def _combine_filters(filters):
        combined_filter = pd.Series()
        for individual_filter in filters:
            combined_filter = individual_filter | combined_filter
        return combined_filter

    @staticmethod
    def filter_on_column_index(dataframe, index_list):
        """
        Retuns a subset where the columns in index_list are filtered out
        """
        if isinstance(index_list, (list, tuple)):
            if max(index_list) > dataframe.shape[1]:
                msg = (
                    "Index list is larger than observation data, please check input, max index list:"
                    "{} number of data points: {}".format(
                        max(index_list), dataframe.shape[1]
                    )
                )
                raise IndexError(msg)
            return dataframe.iloc[:, list(index_list)]
        else:
            return dataframe
