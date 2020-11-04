import os
import signal
import sys
import json

import job_runner.reporting as reporting
from job_runner.reporting.message import Finish
from job_runner.runner import JobRunner
from job_runner import JOBS_FILE


def _setup_reporters(is_interactive_run, ee_id):
    reporters = []
    if is_interactive_run:
        reporters.append(reporting.Interactive())
    elif ee_id:
        reporters.append(reporting.File())
        reporters.append(reporting.Network())
        reporters.append(reporting.Event())
    else:
        reporters.append(reporting.File())
        reporters.append(reporting.Network())
    return reporters


def main(args):
    # If run_path is defined, enter into that directory
    if len(args) > 1:
        run_path = args[1]

        if not os.path.exists(run_path):
            sys.exit("No such directory: {}".format(run_path))
        os.chdir(run_path)

    ee_id = None
    try:
        with open(JOBS_FILE, "r") as json_file:
            jobs_data = json.load(json_file)
            ee_id = jobs_data.get("ee_id")
    except ValueError as e:
        raise IOError("Job Runner cli failed to load JSON-file.{}".format(str(e)))

    jobs_to_run = []
    if len(args) > 2:
        jobs_to_run = args[2:]

    is_interactive_run = len(args) > 2
    reporters = _setup_reporters(is_interactive_run, ee_id)

    job_runner = JobRunner(jobs_data)

    for job_status in job_runner.run(jobs_to_run):
        for reporter in reporters:
            reporter.report(job_status)

        if isinstance(job_status, Finish) and not job_status.success():
            pgid = os.getpgid(os.getpid())
            os.killpg(pgid, signal.SIGKILL)
