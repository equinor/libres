set(PYTHON_SOURCES
    __init__.py
    jobs.py
)


add_python_package("python.res.plugins.hook_specifications"  ${PYTHON_INSTALL_PREFIX}/res/plugins/hook_specifications "${PYTHON_SOURCES}" True)
