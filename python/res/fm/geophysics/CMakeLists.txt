set(PYTHON_SOURCES
    __init__.py
)

add_subdirectory(grav)
if (EXISTS ${STATOIL_TESTDATA_ROOT})
    add_subdirectory(ots)
endif()
add_python_package("python.res.fm.geophysics" ${PYTHON_INSTALL_PREFIX}/res/fm/geophysics "${PYTHON_SOURCES}" True)
