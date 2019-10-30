import pluggy

from res.plugins.hook_specifications import RES_JOB_PLUGIN_NAMESPACE


hook_specification = pluggy.HookspecMarker(RES_JOB_PLUGIN_NAMESPACE)


@hook_specification
def installable_jobs():
    """
    :return: dict with job names as keys and path to script as value
    :rtype: dict[str,str]
    """
