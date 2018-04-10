from ecl.util.util import BoolVector

from res.enkf import ResConfig, EnKFMain, EnkfConfigNode, EnkfNode, NodeId
from .batch_simulator_context import BatchContext

def _slug(entity):
    entity = ' '.join(str(entity).split())
    return ''.join([x if x.isalnum() else '_' for x in entity.strip()])

class BatchSimulator(object):

    def __init__(self, res_config, controls, results, callback = None):
        """Will create simulator which can be used to run multiple simulations.

        The @res_config argument should be a ResConfig object, representing the
        fully configured state of libres.


        The @controls argument configures which parameters the simulator should
        get when actually simulating. The @controls argument should be a
        dictionary like this:

            controls = {"cmode": ["Well", "Group"], "order": ["W1", "W2", "W3"]}

        In this example the simulator will expect two arrays 'cmode' and
        'order', consisting of two and three elements respectively. When
        actually simualating these values will be written to json files looking
        like:

             cmode.json = {"Well": 1.0, "Group": 2.0}
             order.json = {"W1": 1, "W2": 1.0, "W3": 1.0}

        When later invoking the start() method the simulator expects to get
        values for all parameters configured with the @controls argument,
        otherwise an exception will be raised. Internally in libres code the
        controls will be implemented as 'ext_param' instances.


        The @results argument is a list of keys of results which the simulator
        expects to be generated by the forward model. If argument @results
        looks like:

             results = ["CMODE", "order"]

        The simulator will look for the files 'CMODE_0' and 'order_0' in the
        simulation folder. If those files are not produced by the simulator an
        exception will be raised.

        The optional argument callback can be used to provide a callable
        which will be called as:

              callback(run_context)

        When the simulator has started. For the callable passed as
        callback you are encouraged to use the future proof signature:

             def callback(*args, **kwargs):
                 ....

        """
        if not isinstance(res_config, ResConfig):
            raise ValueError("The first argument must be valid ResConfig instance")

        self.res_config = res_config
        self.ert = EnKFMain(self.res_config)
        self.control_keys = tuple(controls.keys())
        self.result_keys = tuple(results)
        self.callback = callback

        ens_config = self.res_config.ensemble_config
        for control_name, variable_names in controls.items():
            ens_config.addNode(EnkfConfigNode.create_ext_param(control_name, variable_names))

        for key in results:
            ens_config.addNode(EnkfConfigNode.create_gen_data(key, "{}_%d".format(key)))


    def _setup_case(self, case, file_system):
        for sim_id, (geo_id, controls)  in enumerate(case):
            assert isinstance(geo_id, int)

            node_id = NodeId(0, sim_id)
            if set(controls.keys()) != set(self.control_keys):
                err_msg = "Mismatch between initialized and provided control names."
                raise KeyError(err_msg)

            for control_name, control in controls.items():
                ens_config = self.res_config.ensemble_config
                node = EnkfNode(ens_config[control_name])
                ext_node = node.as_ext_param()

                if len(ext_node) != len(control.keys()):
                    err_msg = "Expected %d variables for control: %s, received %d."
                    err_in = (len(ext_node), control_name, len(control.keys()))
                    raise KeyError(err_msg % err_in)

                for var_name, value in control.items():
                    ext_node[var_name] = value

                node.save(file_system, node_id)


    def start(self, case_name, case_data):
        """Will start batch simulation, returning a handle to query status and results.

        The start method will submit simulations to the queue system and then
        return a BatchContext handle which can be used to query for simulation
        status and results. The @case_name argument should just be string which
        will be used as name for the storage of these simulations in the
        system. The @controls argument is the set of control values, and the
        corresponding ID of the external realisation used for the simulations.
        The @control argument must match the control argument used when the
        simulator was instantiated. Assuming the following @control argument
        was passed to simulator construction:

             controls = {"cmode": ["Well", "Group"], "order" : ["W1", "W2", "W3"]}

        Then the following @case_data argument can be used in the start method
        to simulate four simulations:

              [
                  (1,
                   {
                       "cmode": {"Well": 2, "Group": 2},
                       "order": {"W1": 2, "W2": 2, "W3": 5},
                   }),
                  (1,
                   {
                       "cmode": {"Well": 1, "Group": 3},
                       "order": {"W1": 2, "W2": 2, "W3": 7},
                   }),
                  (1,
                   {
                       "cmode": {"Well": 1, "Group": 7},
                       "order": {"W1": 2, "W2": 0, "W3": 5},
                   }),
                  (2,
                   {
                       "cmode": {"Well": 1, "Group": -1},
                       "order": {"W1": 2, "W2": 2, "W3": 1},
                   }),
              ]

        The first integer argument in the tuple is the realisation id, so this
        simulation batch will consist of a total of four simulations, where the
        three first are based on realisation 1, and the last is based on
        realisation 2.

        Observe that only one BatchSimulator should actually be running at a
        time, so when you have called the 'start' method you need to let that
        batch complete before you start a new batch.
        """

        self.ert.addDataKW("<CASE_NAME>", _slug(case_name))
        file_system = self.ert.getEnkfFsManager().getFileSystem(case_name)
        self._setup_case(case_data, file_system)

        # The input should be validated before we instantiate the BatchContext
        # object, at that stage a job_queue object with multiple threads is
        # started, and things will typically be in a quite sorry state if an
        # exception occurs.
        itr = 0
        mask = BoolVector(default_value=True, initial_size=len(case_data))
        sim_context = BatchContext(self.result_keys, self.ert, file_system, mask, itr)

        for sim_id, (geo_id, _) in enumerate(case_data):
            sim_context.addSimulation(sim_id, geo_id)

        if self.callback:
            self.callback(sim_context)
        return sim_context