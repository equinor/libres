#  Copyright (C) 2017  Statoil ASA, Norway.
#
#  The file 'test_model_config.py' is part of ERT - Ensemble based Reservoir Tool.
#
#  ERT is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  ERT is distributed in the hope that it will be useful, but WITHOUT ANY
#  WARRANTY; without even the implied warranty of MERCHANTABILITY or
#  FITNESS FOR A PARTICULAR PURPOSE.
#
#  See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html>
#  for more details.

import os

from ecl.test import ExtendedTestCase, TestAreaContext

from res.enkf.enums import KeepRunpath
from res.enkf import ModelConfig, ResConfig

class ModelConfigTest(ExtendedTestCase):


    def setUp(self):
        self.case_directory = self.createTestPath("local/simple_config/")
        self.config_file = "simple_config/minimum_config"


    def test_enum(self):
        self.assertEnumIsFullyDefined(
                KeepRunpath,
                "keep_runpath_type",
                "libenkf/include/ert/enkf/enkf_types.h",
                verbose=True
                )


    def assert_pre_clear_runpath(self, write_pcr, exp_pcr):
        area_name = "model_config_pre_clear_runpath_%s" % write_pcr
        with TestAreaContext(area_name) as work_area:
            work_area.copy_directory(self.case_directory)

            # Append config file
            if write_pcr:
                with open(self.config_file, 'a') as cf:
                    cf.write("\nPRE_CLEAR_RUNPATH %s\n" % write_pcr)

            res_config = ResConfig(self.config_file)
            model_config = res_config.model_config
            self.assertEqual(model_config.pre_clear_runpath, exp_pcr)


    def test_pre_clear_runpath(self):
        test_cases = [(None, False), ("FALSE", False), ("TRUE", True)]

        for (write_pcr, exp_pcr) in test_cases:
            self.assert_pre_clear_runpath(write_pcr, exp_pcr)


    def assert_delete_runpath(self, delete_runpath):
        with TestAreaContext("model_config_delete") as work_area:
            work_area.copy_directory(self.case_directory)

            # Increase the number of realizations
            with open(self.config_file, 'w') as cf:
                cf.writelines(map(
                    lambda line: line+os.linesep,
                    [
                    "JOBNAME  Job%d",
                    "RUNPATH /tmp/simulations/run%d",
                    "NUM_REALIZATIONS 100",
                    "JOB_SCRIPT script.sh"
                    ]))

            # Append config file
            if delete_runpath:
                output_ranges = [
                                    str(interval)
                                    if isinstance(interval, int) else
                                    " - ".join(map(str, interval))
                                    for interval in delete_runpath
                                 ]

                with open(self.config_file, 'a') as cf:
                    cf.write(
                            "\nDELETE_RUNPATH %s\n" %
                            ", ".join(output_ranges)
                            )

            exp_keep_runpath = [KeepRunpath.DEFAULT_KEEP]*100
            for interval in delete_runpath:
                if isinstance(interval, int):
                   interval = [interval] * 2

                for i in range(interval[0], interval[1]+1):
                    exp_keep_runpath[i] = KeepRunpath.EXPLICIT_DELETE

            res_config = ResConfig(self.config_file)
            self.assertEqual(
                    exp_keep_runpath,
                    res_config.model_config.keep_runpath
                    )

    def test_delete_runpath(self):
        test_cases = [
                        [],
                        [1,5,[8,10]],
                        [2,3,4,5],
                        [[1,10], [40,99]],
                        [[0, 99]]
                     ]

        for delete_runpath in test_cases:
            self.assert_delete_runpath(delete_runpath)
