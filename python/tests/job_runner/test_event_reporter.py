import os
import os.path
from unittest import TestCase

import pytest

from job_runner.job import Job
from job_runner.reporting import Event
from job_runner.reporting.message import Exited, Finish, Init, Running, Start


def test_report_with_init_message_argument(tmpdir):
    reporter = Event(event_log=tmpdir / "event_log")
    job1 = Job({"name": "job1", "stdout": "/stdout", "stderr": "/stderr"}, 0)

    reporter.report(Init([job1], 1, 19))

    with open(reporter._event_log, "r") as f:
        lines = f.readlines()
        assert len(lines) == 1
        assert '"name": "job1"' in lines[0], "log missing job1"
        assert '"status": "Waiting"' in lines[0], "log missing Waiting status"


def test_report_with_successful_start_message_argument(tmpdir):
    reporter = Event(event_log=tmpdir / "event_log")
    msg = Start(
        Job(
            {
                "name": "job1",
                "stdout": "/stdout.0",
                "stderr": "/stderr.0",
                "argList": ["--foo", "1", "--bar", "2"],
                "executable": "/bin/bash",
            },
            0,
        )
    )
    reporter.status_dict = reporter._init_job_status_dict(msg.timestamp, 0, [msg.job])

    reporter.report(msg)

    with open(reporter._event_log, "r") as f:
        lines = f.readlines()
        assert len(lines) == 1
        assert '"status": "Running"' in lines[0], "log missing Running status"
        assert '"start_time": null' not in lines[0], "start_time set"


def test_report_with_failed_start_message_argument(tmpdir):
    reporter = Event(event_log=tmpdir / "event_log")
    msg = Start(Job({"name": "job1"}, 0)).with_error("massive_failure")
    reporter.status_dict = reporter._init_job_status_dict(msg.timestamp, 0, [msg.job])

    reporter.report(msg)

    with open(reporter._event_log, "r") as f:
        lines = f.readlines()
        assert len(lines) == 1
        assert '"status": "Failure"' in lines[0], "log missing Failure status"
        assert '"error": "massive_failure"' in lines[0], "log missing error message"
        assert (
            reporter.status_dict["jobs"][0]["end_time"] is not None
        ), "end_time not set for job1"


def test_report_with_successful_exit_message_argument(tmpdir):
    reporter = Event(event_log=tmpdir / "event_log")
    msg = Exited(Job({"name": "job1"}, 0), 0)
    reporter.status_dict = reporter._init_job_status_dict(msg.timestamp, 0, [msg.job])

    reporter.report(msg)

    with open(reporter._event_log, "r") as f:
        lines = f.readlines()
        assert len(lines) == 1
        assert '"status": "Success"' in lines[0], "log missing Success status"


def test_report_with_failed_exit_message_argument(tmpdir):
    reporter = Event(event_log=tmpdir / "event_log")
    msg = Exited(Job({"name": "job1"}, 0), 1).with_error("massive_failure")
    reporter.status_dict = reporter._init_job_status_dict(msg.timestamp, 0, [msg.job])

    reporter.report(msg)

    with open(reporter._event_log, "r") as f:
        lines = f.readlines()
        assert len(lines) == 1
        assert '"status": "Failure"' in lines[0], "log missing Failure status"
        assert '"error": "massive_failure"' in lines[0], "log missing error message"
        assert reporter.status_dict["jobs"][0]["end_time"] is not None


def test_report_with_running_message_argument(tmpdir):
    reporter = Event(event_log=tmpdir / "event_log")
    msg = Running(Job({"name": "job1"}, 0), 100, 10)
    reporter.status_dict = reporter._init_job_status_dict(msg.timestamp, 0, [msg.job])

    reporter.report(msg)

    with open(reporter._event_log, "r") as f:
        lines = f.readlines()
        assert len(lines) == 1
        assert '"status": "Running"' in lines[0], "log missing status"
        assert '"max_memory_usage": 100' in lines[0], "log missing max_memory_usage"
        assert (
            '"current_memory_usage": 10' in lines[0]
        ), "log missing current_memory_usage"


def test_report_with_successful_finish_message_argument(tmpdir):
    reporter = Event(event_log=tmpdir / "event_log")
    msg = Finish()
    reporter.status_dict = reporter._init_job_status_dict(msg.timestamp, 0, [])

    reporter.report(msg)

    with open(reporter._event_log, "r") as f:
        lines = f.readlines()
        assert len(lines) == 1
        assert '"status": "Success"' in lines[0], "log missing status"
