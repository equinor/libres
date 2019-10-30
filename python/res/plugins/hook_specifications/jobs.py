import pluggy

RES_JOB_PLUGIN_NAMESPACE = "res_jobs"
hook_specification = pluggy.HookspecMarker(RES_JOB_PLUGIN_NAMESPACE)
hook_implementation = pluggy.HookimplMarker(RES_JOB_PLUGIN_NAMESPACE)


@hook_specification
def installable_jobs():
    """
    :return: dict with job names as keys and path to config as value
    :rtype: dict[str,str]
    """


@hook_specification
def installable_workflow_jobs():
    """
    :return: dict with workflow job names as keys and path to config as value
    :rtype: dict[str,str]
    """

