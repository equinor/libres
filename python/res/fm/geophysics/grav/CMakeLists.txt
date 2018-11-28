set(PYTHON_SOURCES
    __init__.py
    grav.py
    grav_config.py
    grav_obs.py
    grav_result.py
)

add_python_package("python.res.fm.geophysics.grav" ${PYTHON_INSTALL_PREFIX}/res/fm/geophysics/grav "${PYTHON_SOURCES}" True)
