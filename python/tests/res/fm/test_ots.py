import unittest
import datetime
from collections import namedtuple

from unittest import skipIf

from ecl.grid import EclGridGenerator

from res.fm.geophysics.ots import OTS
from tests import ResTest
from ecl.util.test import TestAreaContext
from ecl.util.geometry import Surface

from ota_create_segy_file import *
from ots_util import *

from res.util import ResVersion
currentVersion = ResVersion( )




parms = namedtuple('Parms', ['seabed',
                             'above',
                             'youngs',
                             'poisson',
                             'rfactor',
                             'mapaxes',
                             'convention',
                             'output_dir',
                             'horizon',
                             'ascii',
                             'velocity_model',
                             'eclbase'])


@skipIf(currentVersion < (2, 2, "X"), "Must have at least version 2.2")
class OTSTest(ResTest):
    __name__ = "OTSTest"

    def setUp(self):
        self._vcube = self.createTestPath("OTS/Velocity/velcube.segy")

        spec = segyio.spec()
        spec.format = 5
        spec.sorting = 2
        spec.samples = range(0, 40, 4)
        spec.ilines = range(2)
        spec.xlines = range(2)
        self._actnum = [0, 0, 0, 0, 1, 1, 1, 1]

        self._spec = spec

        parms.output_dir = None
        parms.horizon = None
        parms.ascii = None
        parms.velocity_model = None
        parms.seabed = 10
        parms.above = 10
        parms.youngs = 0.5
        parms.poisson = 0.3
        parms.rfactor = 20
        parms.convention = 1

    def test_create(self):
        grid = EclGridGenerator.createRectangular(dims=(10, 10, 10), dV=(1, 1, 1))

        with TestAreaContext("missing_restart"):
            create_init(grid, "TEST")

            parms.eclbase = "TEST"

            with self.assertRaises(IOError):
                OTS(parms=parms)

        with TestAreaContext("missing_init"):

            parms.eclbase = "TEST"

            with self.assertRaises(IOError):
                OTS(parms=parms)

        with TestAreaContext("missing_grid"):
            grid = EclGridGenerator.createRectangular(dims=(10, 10, 10), dV=(1, 1, 1))

            create_init(grid, "TEST")
            create_restart(grid, "TEST")

            parms.eclbase = "TEST"

            with self.assertRaises(IOError):
                OTS(parms=parms)

        with TestAreaContext("missing_vel"):
            grid = EclGridGenerator.createRectangular(dims=(10, 10, 10), dV=(1, 1, 1))
            grid.save_EGRID("TEST.EGRID")

            create_init(grid, "TEST")
            create_restart(grid, "TEST")

            parms.velocity_model = "missing.segy"

            with self.assertRaises(IOError):
                OTS(parms=parms)

        with TestAreaContext("no_error"):
            grid = EclGridGenerator.createRectangular(dims=(10, 10, 10), dV=(1, 1, 1))

            grid.save_EGRID("TEST.EGRID")
            create_init(grid, "TEST")
            create_restart(grid, "TEST")

            parms.velocity_model = "TEST.segy"
            create_segy_file(parms.velocity_model, self._spec)

            OTS(parms=parms)

    def test_eval(self):
        grid = EclGridGenerator.createRectangular(dims=(2, 2, 2), dV=(100, 100, 100), actnum=self._actnum)

        with TestAreaContext("test_eval"):
            grid.save_EGRID("TEST.EGRID")
            create_init(grid, "TEST")
            create_restart(grid, "TEST")

            parms.velocity_model = "TEST.segy"
            create_segy_file(parms.velocity_model, self._spec)

            ots = OTS(parms=parms)
            with self.assertRaises(ValueError):
                ots.addSurvey("S1", datetime.date(2000, 1, 15))

            vintage_pairs = [(datetime.date(1900, 1, 1), datetime.date(2010, 1, 1))]

            with self.assertRaises(ValueError):
                ots.geertsma_ts_simple(vintage_pairs)

            vintage_pairs = [(datetime.date(2010, 1, 1), datetime.date(1900, 1, 1))]

            with self.assertRaises(ValueError):
                ots.geertsma_ts_simple(vintage_pairs)

            vintage_pairs = [(datetime.date(2000, 1, 1), None)]
            with self.assertRaises(ValueError):
                ots.geertsma_ts_simple(vintage_pairs)

            vintage_pairs = [(datetime.date(2000, 1, 1), datetime.date(2010, 1, 1))]

            ots.geertsma_ts_simple(vintage_pairs)

    def test_geertsma_TS_simple(self):
        grid = EclGridGenerator.createRectangular(dims=(2, 2, 2), dV=(100, 100, 100), actnum=self._actnum)

        with TestAreaContext("test_geertsmaTS_simple"):
            create_restart(grid, "TEST")
            create_init(grid, "TEST")
            grid.save_EGRID("TEST.EGRID")

            parms.velocity_model = "TEST.segy"

            l = [50, 150]
            create_segy_file(parms.velocity_model, self._spec, xl=l, il=l, cdp_x=l, cdp_y=l)

            ots = OTS(parms=parms)

            vintage_pairs = [(datetime.date(2000, 1, 1), datetime.date(2010, 1, 1)),
                             (datetime.date(2010, 1, 1), datetime.date(2011, 1, 1))]

            tshift = ots.geertsma_ts_simple(vintage_pairs)
            self.assertAlmostEqual(tshift[0][0], -0.01006, 4)

            parms.convention = -1
            ots = OTS(parms=parms)

            vintage_pairs = [(datetime.date(2000, 1, 1), datetime.date(2010, 1, 1)),
                             (datetime.date(2010, 1, 1), datetime.date(2011, 1, 1))]

            tshift = ots.geertsma_ts_simple(vintage_pairs)
            self.assertAlmostEqual(tshift[0][0], 0.01006, 4)

    def test_geertsma_TS_rporv(self):
        grid = EclGridGenerator.createRectangular(dims=(2, 2, 2), dV=(100, 100, 100), actnum=self._actnum)

        with TestAreaContext("test_geertsmaTS_rporv", store_area=True):
            create_restart(grid, "TEST", rporv=[10 for i in range(grid.getNumActive())])
            create_init(grid, "TEST")
            grid.save_EGRID("TEST.EGRID")

            parms.velocity_model = "TEST.segy"

            l = [50, 150]
            create_segy_file(parms.velocity_model, self._spec, xl=l, il=l, cdp_x=l, cdp_y=l)

            ots = OTS(parms=parms)

            vintage_pairs = [(datetime.date(2000, 1, 1), datetime.date(2010, 1, 1)),
                             (datetime.date(2010, 1, 1), datetime.date(2011, 1, 1))]

            tshift = ots.geertsma_ts_rporv(vintage_pairs)

    def test_geertsma_TS(self):
        grid = EclGridGenerator.createRectangular(dims=(2, 2, 2), dV=(100, 100, 100), actnum=self._actnum)

        with TestAreaContext("test_geertsmaTS"):
            create_restart(grid, "TEST")
            create_init(grid, "TEST")
            grid.save_EGRID("TEST.EGRID")

            parms.velocity_model = "TEST.segy"

            l = [50, 150]
            create_segy_file(parms.velocity_model, self._spec, xl=l, il=l, cdp_x=l, cdp_y=l)

            ots = OTS(parms=parms)

            vintage_pairs = [(datetime.date(2000, 1, 1), datetime.date(2010, 1, 1)),
                             (datetime.date(2010, 1, 1), datetime.date(2011, 1, 1))]

            tshift = ots.geertsma_ts(vintage_pairs)

            self.assertAlmostEqual(tshift[0][0], -0.00104, 4)

            parms.convention = -1

            ots = OTS(parms=parms)

            vintage_pairs = [(datetime.date(2000, 1, 1), datetime.date(2010, 1, 1)),
                             (datetime.date(2010, 1, 1), datetime.date(2011, 1, 1))]

            tshift = ots.geertsma_ts(vintage_pairs)

            self.assertAlmostEqual(tshift[0][0], 0.00104, 4)

    def test_dPV(self):
        grid = EclGridGenerator.createRectangular(dims=(2, 2, 2), dV=(100, 100, 100), actnum=self._actnum)

        with TestAreaContext("test_dPV"):
            grid.save_EGRID("TEST.EGRID")
            create_restart(grid, "TEST")
            create_init(grid, "TEST")

            ots = OTS(parms=parms)

            vintage_pairs = [(datetime.date(2000, 1, 1), datetime.date(2010, 1, 1)),
                             (datetime.date(2010, 1, 1), datetime.date(2011, 1, 1))]

            tshift = ots.dpv(vintage_pairs)
            self.assertAlmostEqual(tshift[0][0], ((20 - 10) * 1e6 + (0 - 0) * 1e6)/1e9)
            self.assertAlmostEqual(tshift[0][2], ((20 - 10) * 1e6 + (0 - 0) * 1e6)/1e9)

            self.assertAlmostEqual(tshift[1][0], ((25 - 20) * 1e6 + (0 - 0) * 1e6)/1e9)
            self.assertAlmostEqual(tshift[1][2], ((25 - 20) * 1e6 + (0 - 0) * 1e6)/1e9)

            parms.convention = -1

            ots = OTS(parms=parms)
            tshift_b_m = ots.dpv(vintage_pairs)
            self.assertAlmostEqual(tshift[0][0], -tshift_b_m[0][0])

    def test_irap_surface(self):
        grid = EclGridGenerator.createRectangular(dims=(2, 2, 2), dV=(100, 100, 100), actnum=self._actnum)

        with TestAreaContext("test_irap_surface"):
            create_restart(grid, "TEST")
            create_init(grid, "TEST")
            grid.save_EGRID("TEST.EGRID")

            parms.velocity_model = "TEST.segy"
            create_segy_file(parms.velocity_model, self._spec)

            ots = OTS(parms=parms)

            f_name = "irap.txt"
            s = ots._create_surface()
            s.write(f_name)
            s = Surface(f_name)

            self.assertEquals(s.getNX(), 2)
            self.assertEquals(s.getNY(), 2)

            for i in range(len(s)):
                self.assertEquals(s[i], 90)


if __name__ == "__main__":
    unittest.main()
