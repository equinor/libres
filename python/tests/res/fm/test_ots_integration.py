import unittest
from unittest import skipIf

from res.fm.ots import ots_run
from tests import ResTest
from ecl.util.test import TestAreaContext
from ecl.util.geometry import Surface

try:
    from res.util import ResVersion
    currentVersion = ResVersion( )
except ImportError:
    from ert.util import Version
    currentVersion = Version.currentVersion( )



@skipIf(currentVersion < (2, 2, "X"), "Must have at least version 2.2")
class OTSIntegrationTest(ResTest):
    __name__ = "OTSIntegrationTest"

    def setUp(self):
        self._output_dir = "ts"
        self._horizon = "horizon.irap"

        self._vcube = self.createTestPath("Statoil/OTS/Velocity/velcube.segy")
        self._eclbase = self.createTestPath("Statoil/OTS/upscaled_model_28x61x35/SYNC_150109_28x61x35")

    def test_ots_run(self):

        with TestAreaContext("test_ots_run"):

            config_file = "config"
            with open(config_file, 'w') as f:
                f.write("ECLBASE {}\n".format(self._eclbase))
                f.write("VELOCITY {}\n".format(self._vcube))
                f.write("ABOVERES {}\n".format(100))
                f.write("SEABED {}\n".format(300))
                f.write("YOUNGS {}\n".format(0.5))
                f.write("POISSON {}\n".format(0.3))
                f.write("RFACTOR {}\n".format(20))
                f.write("MAPAXES {}\n".format(False))
                f.write("CONVENTION {}\n".format(1))
                f.write("OUTPUT_DIR {}\n".format(self._output_dir))
                f.write("HORIZON {}\n".format(self._horizon))
                f.write("ASCII {}\n".format("ts.txt"))
                f.write("DPV {} {}\n".format("1997.07.18", "2009.06.19"))
                f.write("TS_SIMPLE {} {}\n".format("1997.07.18", "2009.06.19"))
                f.write("TS_SIMPLE {} {}\n".format("2010.01.01", "2011.01.01"))
                f.write("TS_SIMPLE {} {}\n".format("2011.01.01", "2010.01.01"))
                # f.write("TS {} {}\n".format("1997.07.18", "2009.06.19"))

            ots_run(config_file)

            ts_irap0 = Surface(self._output_dir+"_dpv/ots_1997_07_18_2009_06_19.irap")
            ts_irap1 = Surface(self._output_dir+"_ts_simple/ots_1997_07_18_2009_06_19.irap")
            ts_irap2 = Surface(self._output_dir+"_ts_simple/ots_2010_01_01_2011_01_01.irap")
            ts_irap3 = Surface(self._output_dir+"_ts_simple/ots_2011_01_01_2010_01_01.irap")
            # ts_irap4 = Surface(self._output_dir+"_ts/ots_1997_07_18_2009_06_19.irap")
            horizon_irap = Surface(self._horizon)

            self.assertEqual(40, ts_irap0.getNX())
            self.assertEqual(58, ts_irap0.getNY())
            self.assertEqual(2320, len(ts_irap0))

            self.assertEqual(40, ts_irap1.getNX())
            self.assertEqual(58, ts_irap1.getNY())
            self.assertEqual(2320, len(ts_irap1))

            self.assertFloatEqual(0.0162, ts_irap0[41])
            self.assertFloatEqual(-0.0026, ts_irap0[1001])
            self.assertFloatEqual(-2.2648, ts_irap0[1500])

            self.assertFloatEqual(-0.009500, ts_irap1[41])
            self.assertFloatEqual(-0.016700, ts_irap1[1001])
            self.assertFloatEqual(1.247900, ts_irap1[1500])

            self.assertFloatEqual(0.053700, ts_irap2[41])
            self.assertFloatEqual(-0.002700, ts_irap2[1001])
            self.assertFloatEqual(-0.086500, ts_irap2[1500])

            self.assertFloatEqual(-0.010700, ts_irap3[41])
            self.assertFloatEqual(0.007300, ts_irap3[1001])
            self.assertFloatEqual(2.162000, ts_irap3[1500]*5)

            # self.assertFloatEqual(-0.0097, ts_irap4[41], 4)
            # self.assertFloatEqual(-0.0144, ts_irap4[1001], 4)
            # self.assertFloatEqual(1.1213, ts_irap4[1500], 4)

            self.assertEqual(40, horizon_irap.getNX())
            self.assertEqual(58, horizon_irap.getNY())
            self.assertEqual(2320, len(horizon_irap))

            self.assertFloatEqual(2604.5301, horizon_irap[41])
            self.assertFloatEqual(2709.4176, horizon_irap[1001])
            self.assertFloatEqual(2316.7253, horizon_irap[1500])


if __name__ == "__main__":
    unittest.main()
