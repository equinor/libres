set(PYTHON_SOURCES
    __init__.py
)

add_python_package("python.res.plugins"  ${PYTHON_INSTALL_PREFIX}/res/plugins "${PYTHON_SOURCES}" True)

add_subdirectory( hook_specifications )
