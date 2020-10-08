set(PYTHON_SOURCES
    __init__.py
    event.py
    file.py
    interactive.py
    message.py
    network.py
)


add_python_package("python.job_runner.reporting"  ${PYTHON_INSTALL_PREFIX}/job_runner/reporting "${PYTHON_SOURCES}" True)
