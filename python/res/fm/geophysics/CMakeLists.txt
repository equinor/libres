set(PYTHON_SOURCES
    __init__.py
)

add_subdirectory(grav)
add_subdirectory(ots)
add_python_package("python.res.fm.geophysics" ${PYTHON_INSTALL_PREFIX}/res/fm/geophysics "${PYTHON_SOURCES}" True)
