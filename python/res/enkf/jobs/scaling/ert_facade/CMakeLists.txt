set(PYTHON_SOURCES
    facade.py
    __init__.py
)

add_python_package("python.res.enkf.jobs.scaling.ert_facade" ${PYTHON_INSTALL_PREFIX}/res/enkf/jobs/scaling/ert_facade "${PYTHON_SOURCES}" True)
