import numpy as np

from copy import deepcopy


class DataMatrix(object):
    def __init__(self, input_data):
        """
        Takes input data in the form of a Pandas dataframe with observations,
        standard deviation and simulated values. Assumes observations are
        prepended with _OBS and standard deviation with _STD.
        """
        self.data = input_data
        if input_data.shape[1] == 0:
            raise ValueError("Empty dataset, all data has been filtered out")


    def get_data_matrix(self, observation_keys):
        """
        Extracts data from a dataframe given a set of keys and returns a matrix
        """
        return self.data.loc[list(observation_keys)].values

    def std_normalization(self, observation_keys, inplace=False):
        """
        Duplicates the behavior of obs_data_scale, and scales the simulated data
        by 1 / (observation standard deviation), per observation key, i.e. each
        simulation data point is scaled by its corresponding std deviation
        from observations.
        """
        output_data = deepcopy(self.data)
        for key in observation_keys:
            data_matrix = self.data.loc[key]
            std_vector = self.data.loc["STD_" + key]
            output_data.loc[key] = (data_matrix * (1. / std_vector))

        if inplace:
            self.data = output_data
        return output_data

    def get_scaling_factor(self, events):
        """
        Performs PCA, calculates the number of primary components based on
        a threshold and returns a scaling factor based on the number of
        primary components and the number of observations.
        """
        data_matrix = self.get_data_matrix(events.keys)
        nr_components = self._get_nr_primary_components(data_matrix, threshold=events.threshold)
        scaling_factor = self._calculate_scaling_factor(len(events.keys), data_matrix.shape[1], nr_components)

        return scaling_factor

    @staticmethod
    def _get_nr_primary_components(data_matrix, threshold):
        """
        Takes a matrix, does PCA and calculates the cumulative variance ratio
        and returns an int which is the number of primary components where
        the cumulative variance is smaller than user set threshold. Notice the
        way of calculating number of primary components. This is done to
        replicate existing behavior:

        int num_significant  = 0;
        {
        double running_sigma2  = 0;
        for (int i=0; i < num_singular_values; i++) {
          if (running_sigma2 / total_sigma2 < truncation) {
             num_significant++;
             running_sigma2 += sig0[i] * sig0[i];
          } else
             break;
        }
        """
        _, s, _ = np.linalg.svd(data_matrix.T.astype(np.float), full_matrices=False)
        variance_ratio = np.cumsum(s ** 2) / np.sum(s ** 2)
        return len([1 for i in variance_ratio[: -1] if i < threshold]) + 1

    @staticmethod
    def _calculate_scaling_factor(nr_keys, nr_observations, nr_components):
        """
        Calculates a observation scaling factor which is:
            sqrt(nr_obs / pc)
        where:
            nr_obs is the number of observations, i.e. the number of
                observations times the number of scaled keys
            pc is the number of primary components from PCA analysis
                below a user threshold
        """
        return np.sqrt(nr_keys * nr_observations / float(nr_components))
