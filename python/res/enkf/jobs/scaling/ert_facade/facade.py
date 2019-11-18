class ErtFacade(object):
    def __init__(self, ert_implementation):
        self._ert = ert_implementation

    def current_case_name(self):
        return self._ert.getEnkfFsManager().getCurrentFileSystem().getCaseName()

    def impl_type_name_for_obs_key(self, key):
        return self._ert.getObservations()[key].getImplementationType().name

    def obs(self):
        return self._ert.getObservations()

    def ens_size(self):
        return self._ert.getEnsembleSize()

    def current_fs(self):
        return self._ert.getEnkfFsManager().getCurrentFileSystem()

    def impl(self):
        return self._ert

    def data_key_for_obs_key(self, observation_key):
        return self._ert.getObservations()[observation_key].getDataKey()

    def matching_wildcards(self):
        return self._ert.getObservations().getMatchingKeys

    def obs_key(self, index):
        return self._ert.getObservations()[index].getKey()
