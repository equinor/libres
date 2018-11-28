set(TEST_SOURCES
    __init__.py
    test_ecl_config.py
    test_ecl_run.py
    test_rms_config.py
    test_rms_run.py
    test_shell.py
    test_templating.py
    test_grav.py
    test_ots.py
    test_ots_integration.py
    test_ots_reservoir.py
    test_ots_velocity.py
    ota_create_segy_file.py
    ots_util.py
)

add_python_package("python.tests.res.fm"  ${PYTHON_INSTALL_PREFIX}/tests/res/fm "${TEST_SOURCES}" False)
addPythonTest(tests.res.fm.test_ecl_config.EclConfigTest)
addPythonTest(tests.res.fm.test_ecl_run.EclRunTest)
addPythonTest(tests.res.fm.test_rms_config.RMSConfigTest)
addPythonTest(tests.res.fm.test_rms_run.RMSRunTest)
addPythonTest(tests.res.fm.test_shell.ShellTest)
addPythonTest(tests.res.fm.test_templating.TemplatingTest)

if (EXISTS ${STATOIL_TESTDATA_ROOT})
    addPythonTest(tests.res.fm.test_grav.GravTest)
    addPythonTest(tests.res.fm.test_ots.OTSTest)
    addPythonTest(tests.res.fm.test_ots_integration.OTSIntegrationTest)
    addPythonTest(tests.res.fm.test_ots_reservoir.OTSReservoirTest)
    addPythonTest(tests.res.fm.test_ots_velocity.OTSVelocityTest)
endif()

