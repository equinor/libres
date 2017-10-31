import os
import json
import subprocess

from ecl.test import ExtendedTestCase
from res.job_queue.environment_varlist import EnvironmentVarlist

from ecl.test import TestAreaContext

class EnvironmentVarlistTest(ExtendedTestCase):

  def test_setenv_dump(self):

    varlist_string = "global_environment"
    first = "FIRST"
    second = "SECOND"
    first_value = "TheFirstValue"
    second_value = "TheSecondValue"
    Varlist = EnvironmentVarlist()
    Varlist.SetEnv(first, first_value)
    Varlist.SetEnv(second, second_value)

    with TestAreaContext("write_env_var_to_json"):
      Varlist.WriteJson()
      json_file = "jobs.json"
      with open(json_file, "r") as js:
        data = json.load(js)
        env_var = data[varlist_string]
        self.assertEqual(first_value, env_var[first]  )
        self.assertEqual(second_value, env_var[second] )


      
