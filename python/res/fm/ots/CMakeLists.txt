set(PYTHON_SOURCES
    __init__.py
    ots.py
    ots_res_surface.py
    ots_vel_surface.py
)

add_python_package("python.res.fm.ots" ${PYTHON_INSTALL_PREFIX}/res/fm/ots "${PYTHON_SOURCES}" True)
