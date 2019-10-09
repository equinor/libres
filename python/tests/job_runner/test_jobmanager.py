import json
import os
import os.path
import stat
from unittest import TestCase

from job_runner.reporting.message import Exited, Start
from job_runner.runner import JobRunner
from res.job_queue import EnvironmentVarlist, ExtJob, ExtJoblist, ForwardModel
from res.util import SubstitutionList

from tests.utils import tmpdir

# Test data generated by ForwardModel
JSON_STRING = """
{
  "DATA_ROOT" : "/path/to/data",
  "run_id"    : "ERT_RUN_ID",
  "umask" : "0000",
  "jobList" : [ {"name" : "PERLIN",
  "executable" : "perlin.py",
  "target_file" : "my_target_file",
  "error_file" : "error_file",
  "start_file" : "some_start_file",
  "stdout" : "perlin.stdoit",
  "stderr" : "perlin.stderr",
  "stdin" : "intput4thewin",
  "argList" : ["-speed","hyper"],
  "environment" : {"TARGET" : "flatland"},
  "license_path" : "this/is/my/license/PERLIN",
  "max_running_minutes" : 12,
  "max_running" : 30
},
{"name" : "PERGEN",
  "executable" : "pergen.py",
  "target_file" : "my_target_file",
  "error_file" : "error_file",
  "start_file" : "some_start_file",
  "stdout" : "perlin.stdoit",
  "stderr" : "perlin.stderr",
  "stdin" : "intput4thewin",
  "argList" : ["-speed","hyper"],
  "environment" : {"TARGET" : "flatland"},
  "license_path" : "this/is/my/license/PERGEN",
  "max_running_minutes" : 12,
  "max_running" : 30
}],
 "ert_version" : [2, 2, "git"]
}
"""

JSON_STRING_NO_DATA_ROOT = """
{
  "umask" : "0000",
  "ert_version" : [2, 2, "git"],
  "jobList"   : []
}
"""


def create_jobs_json(job_list, umask="0000"):
    data = {"umask": umask,
            "DATA_ROOT": "/path/to/data",
            "jobList": job_list}

    jobs_file = os.path.join(os.getcwd(), "jobs.json")
    with open(jobs_file, "w") as f:
        f.write(json.dumps(data))


class JobRunnerTest(TestCase):

    def setUp(self):
        self.dispatch_imp = None
        if "DATA_ROOT" in os.environ:
            del os.environ["DATA_ROOT"]

        if "ERT_RUN_ID" in os.environ:
            del os.environ["ERT_RUN_ID"]


    def tearDown(self):

        keys = ("KEY_ONE", "KEY_TWO", "KEY_THREE", "KEY_FOUR", "PATH104", "DATA_ROOT", "ERT_RUN_ID")

        for key in keys:
            if key in os.environ:
                del os.environ[key]


    def assert_clean_slate(self):
        self.assertFalse(os.path.isfile("jobs.py"))
        self.assertFalse(os.path.isfile("jobs.json"))

    @tmpdir(None)
    def test_no_jobs_json(self):
        self.assert_clean_slate()
        with self.assertRaises(IOError):
            JobRunner(jobs_file="does/not/exist")

    @tmpdir(None)
    def test_invalid_jobs_json(self):
        self.assert_clean_slate()
        # Syntax error
        with open("jobs.json", "w") as f:
            f.write("Hello - this is not valid JSON ...")

        with self.assertRaises(IOError):
            JobRunner()

    @tmpdir(None)
    def test_missing_joblist_json(self):
        self.assert_clean_slate()
        with open("jobs.json", "w") as f:
            f.write(json.dumps({"umask": "0000"}))

        with self.assertRaises(KeyError):
            JobRunner()

    @tmpdir(None)
    def test_missing_umask_json(self):
        self.assert_clean_slate()
        with open("jobs.json", "w") as f:
            f.write(json.dumps({"jobList": "[]"}))

        with self.assertRaises(KeyError):
            JobRunner()

    @tmpdir(None)
    def test_jobs_zero(self):
        with self.assertRaises(IOError):
            JobRunner(jobs_file="Does/not/exist")

    @tmpdir(None)
    def test_run_output_rename(self):
        job = {"name": "TEST_JOB",
               "executable": "/bin/mkdir",
               "stdout": "out",
               "stderr": "err"}
        joblist = [job, job, job, job, job]
        create_jobs_json(joblist)
        jobm = JobRunner()

        for status in enumerate(jobm.run([])):
            if isinstance(status, Start):
                self.assertEqual(
                    status.job.std_err, "err.{}".format(status.job.index)
                )
                self.assertEqual(
                    status.job.std_out, "out.{}".format(status.job.index)
                )

    @tmpdir(None)
    def test_run_multiple_ok(self):
        joblist = []
        dir_list = ["1", "2", "3", "4", "5"]
        for d in dir_list:
            job = {"name": "MKDIR",
                   "executable": "/bin/mkdir",
                   "stdout": "mkdir_out",
                   "stderr": "mkdir_err",
                   "argList": ["-p", "-v", d]}
            joblist.append(job)
        create_jobs_json(joblist)
        jobm = JobRunner()

        statuses = [s for s in list(jobm.run([])) if isinstance(s, Exited)]

        self.assertEqual(len(statuses), 5)
        for status in statuses:
            self.assertEqual(status.exit_code, 0)

        for index, dir_number in enumerate(dir_list):
            self.assertTrue(os.path.isdir(dir_list[index]))
            self.assertTrue(os.path.isfile("mkdir_out.%d" % index))
            self.assertTrue(os.path.isfile("mkdir_err.%d" % index))
            self.assertEqual(0, os.path.getsize("mkdir_err.%d" % index))

    @tmpdir(None)
    def test_run_multiple_fail_only_runs_one(self):
        joblist = []
        for index in range(1, 6):
            job = {"name": "exit",
                   "executable": "/bin/bash",
                   "stdout": "exit_out",
                   "stderr": "exit_err",
                   # produces something on stderr, and exits with
                   # exit_code=index
                   "argList": [
                       "-c",
                       "echo \"failed with {}\" 1>&2 ; exit {}"
                       .format(index, index)
                    ]}
            joblist.append(job)
        create_jobs_json(joblist)
        jobm = JobRunner()

        statuses = [s for s in list(jobm.run([])) if isinstance(s, Exited)]

        self.assertEqual(len(statuses), 1)
        for i, status in enumerate(statuses):
            self.assertEqual(status.exit_code, i+1)

    @tmpdir(None)
    def test_given_global_env_and_update_path_executable_env_is_updated(self):
        executable = "./x.py"
        outfile = "outfile.stdout"

        with open(executable, "w") as f:
            f.write("#!/usr/bin/env python\n")
            f.write("import os\n")
            f.write("print(os.environ['KEY_ONE'])\n")
            f.write("print(os.environ['KEY_TWO'])\n")
            f.write("print(os.environ['PATH104'])\n")
            f.write("print(os.environ['KEY_THREE'])\n")
            f.write("print(os.environ['KEY_FOUR'])\n")
        os.chmod(executable, stat.S_IEXEC + stat.S_IREAD)

        job = {"name": "TEST_GET_ENV1",
               "executable": executable,
               "stdout": outfile,
               "stderr": "outfile.stderr",
               "argList": []}

        data = {"umask": "0000",
                "global_environment": {"KEY_ONE": "FirstValue",
                                       "KEY_TWO": "SecondValue",
                                       "KEY_THREE": "ThirdValue",
                                       "KEY_FOUR": "ThirdValue:FourthValue"},
                "global_update_path": {"PATH104": "NewPath",
                                       "KEY_THREE": "FourthValue",
                                       "KEY_FOUR": "FifthValue:SixthValue"},
                "DATA_ROOT": "/path/to/data",
                "jobList": [job]}

        jobs_file = os.path.join(os.getcwd(), "jobs.json")
        with open(jobs_file, "w") as f:
            f.write(json.dumps(data))
        statuses = list(JobRunner().run([]))

        exited_messages = [m for m in statuses if isinstance(
            m, Exited) and m.success()]
        number_of_finished_scripts = len(exited_messages)
        self.assertEqual(number_of_finished_scripts, 1,
                         "guard check, script must finish successfully")

        with open(outfile + ".0", "r") as out0:
            content = list(out0.read().splitlines())
            self.assertEqual(content[0], 'FirstValue')
            self.assertEqual(content[1], 'SecondValue')
            self.assertEqual(content[2], 'NewPath')
            self.assertEqual(content[3], 'FourthValue:ThirdValue')
            self.assertEqual(content[4],
                             'FifthValue:SixthValue:ThirdValue:FourthValue',
                             """should be a concatenation of the variable from
                             environment and update_path""")

    @tmpdir(None)
    def test_exec_env(self):
        with open("exec_env.py", "w") as f:
            f.write("""#!/usr/bin/env python\n
import os
import json
with open("exec_env_exec_env.json") as f:
     exec_env = json.load(f)
assert exec_env["TEST_ENV"] == "123"
assert exec_env["NOT_SET"] is None
                """)
        os.chmod("exec_env.py", stat.S_IEXEC + stat.S_IREAD)

        with open("EXEC_ENV", "w") as f:
            f.write("EXECUTABLE exec_env.py\n")
            f.write("EXEC_ENV TEST_ENV 123\n")
            f.write("EXEC_ENV NOT_SET")

        ext_job = ExtJob("EXEC_ENV", False)
        job_list = ExtJoblist()
        job_list.add_job("EXEC_ENV", ext_job)
        forward_model = ForwardModel(job_list)
        forward_model.add_job("EXEC_ENV")
        global_args = SubstitutionList()
        env_varlist = EnvironmentVarlist()
        forward_model.formatted_fprintf(
            "run_id", None, "data_root", global_args, 0, env_varlist)

        for msg in list(JobRunner().run([])):
            if isinstance(msg, Start):
                with open("exec_env_exec_env.json") as f:
                    exec_env = json.load(f)
                    assert exec_env["TEST_ENV"] == "123"
                    assert exec_env["NOT_SET"] is None
