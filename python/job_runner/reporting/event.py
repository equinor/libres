import json
import os
import socket
import time

from job_runner.reporting.message import (
    _JOB_STATUS_FAILURE,
    _JOB_STATUS_RUNNING,
    _JOB_STATUS_SUCCESS,
    _RUNNER_STATUS_FAILURE,
    _RUNNER_STATUS_INITIALIZED,
    _RUNNER_STATUS_SUCCESS,
    Exited,
    Finish,
    Init,
    Running,
    Start,
)
from job_runner.util import data as data_util


class Event:
    def __init__(self, event_log="event_log"):
        self._event_log = event_log
        self.status_dict = {}
        self.node = socket.gethostname()

    def report(self, msg):
        job_status = {}
        if msg.job:
            index = msg.job.index
            job_status = self.status_dict["jobs"][index]

        if isinstance(msg, Init):
            self.status_dict = self._init_job_status_dict(
                msg.timestamp, msg.run_id, msg.jobs
            )
            self._dump_event(self.status_dict)

        elif isinstance(msg, Start):
            if msg.success():
                job_status["status"] = _JOB_STATUS_RUNNING
                job_status["start_time"] = data_util.datetime_serialize(msg.timestamp)
            else:
                error_msg = msg.error_message
                job_status["status"] = _JOB_STATUS_FAILURE
                job_status["error"] = error_msg
                job_status["end_time"] = data_util.datetime_serialize(msg.timestamp)
            self._dump_event(job_status)

        elif isinstance(msg, Exited):
            job_status["end_time"] = data_util.datetime_serialize(msg.timestamp)
            self.status_dict["exit_code"] = msg.exit_code

            if msg.success():
                job_status["status"] = _JOB_STATUS_SUCCESS
            else:
                error_msg = msg.error_message
                job_status["error"] = error_msg
                job_status["status"] = _JOB_STATUS_FAILURE
            self._dump_event(job_status)

        elif isinstance(msg, Running):
            job_status["max_memory_usage"] = msg.max_memory_usage
            job_status["current_memory_usage"] = msg.current_memory_usage
            job_status["status"] = _JOB_STATUS_RUNNING
            self._dump_event(job_status)

        elif isinstance(msg, Finish):
            self.status_dict["end_time"] = data_util.datetime_serialize(msg.timestamp)
            if msg.success():
                self.status_dict["status"] = _RUNNER_STATUS_SUCCESS
            else:
                self.status_dict["status"] = _RUNNER_STATUS_FAILURE
            self._dump_event(self.status_dict)

    def _dump_event(self, event):
        with open(self._event_log, "a") as el:
            json.dump(event, el)
            el.write("\n")

    def _init_job_status_dict(self, start_time, run_id, jobs):
        return {
            "run_id": run_id,
            "status": _RUNNER_STATUS_INITIALIZED,
            "start_time": data_util.datetime_serialize(start_time),
            "end_time": None,
            "jobs": [data_util.create_job_dict(j) for j in jobs],
        }
