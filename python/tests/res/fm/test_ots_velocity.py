import unittest
from unittest import skipIf

from ecl.grid import EclGridGenerator
from res.fm.geophysics.ots import OTSVelSurface, OTSResSurface
from tests import ResTest
from ecl.util.test import TestAreaContext

try:
    from res.util import ResVersion
    currentVersion = ResVersion( )
except ImportError:
    from ecl.util.util import Version
    currentVersion = Version.currentVersion( )

from .ota_create_segy_file import *



@skipIf(currentVersion < (2, 2, "X"), "Must have at least version 2.2")
class OTSVelocityTest(ResTest):
    """
    Tests creation of surface from velocity and reservoir
    """

    __name__ = "OTSVelocityTest"

    def setUp(self):
        spec = segyio.spec()
        spec.format = 5
        spec.sorting = 2
        spec.samples = range(0, 40, 4)
        spec.ilines = range(2)
        spec.xlines = range(2)

        self._spec = spec

    def test_create(self):
        with TestAreaContext("OTSSurfaceTest_test_create"):
            vel = "TEST.segy"
            create_segy_file(vel, self._spec)
            grid = EclGridGenerator.createRectangular(dims=(2, 2, 2), dV=(100, 100, 100))
            res_surface = OTSResSurface(grid=grid, above=0)

            OTSVelSurface(res_surface, vel)

    def test_surface(self):

        with TestAreaContext("OTSSurfaceTest_test_surface"):
            vel = "TEST.segy"
            l = [50, 150]
            create_segy_file(vel, self._spec, xl=l, il=l, cdp_x=l, cdp_y=l)
            grid = EclGridGenerator.createRectangular(dims=(2, 2, 2), dV=(100, 100, 100))
            res_surface = OTSResSurface(grid=grid, above=0)
            ots_s = OTSVelSurface(res_surface, vel)

            self.assertEquals(ots_s.x[0], 50)

            self.assertEquals(ots_s.y[0], 50)

            self.assertEquals(ots_s.z[0], 0)

            self.assertEquals(ots_s.nx, 2)
            self.assertEquals(ots_s.ny, 2)

    def test_z3d(self):
        with TestAreaContext("OTSSurfaceTest_test_z3d"):
            vel = "TEST.segy"
            l = [50, 150]
            create_segy_file(vel, self._spec, xl=l, il=l, cdp_x=l, cdp_y=l)
            grid = EclGridGenerator.createRectangular(dims=(2, 2, 2), dV=(100, 100, 100))
            res_surface = OTSResSurface(grid=grid, above=0)
            ots_s = OTSVelSurface(res_surface, vel)

            self.assertEquals((4, 2), ots_s.z3d.shape)


if __name__ == "__main__":
    unittest.main()
