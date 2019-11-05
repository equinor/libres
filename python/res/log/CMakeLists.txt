set(PYTHON_SOURCES
    __init__.py
    log_set_up.py
)

add_python_package("python.res.log" ${PYTHON_INSTALL_PREFIX}/res/log "${PYTHON_SOURCES}" True)
