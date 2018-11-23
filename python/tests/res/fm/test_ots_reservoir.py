import unittest
import numpy as np
from unittest import skipIf

from res.fm.ots import OTSResSurface
from tests import ResTest

from ecl.grid import EclGrid, EclGridGenerator


try:
    from res.util import ResVersion
    currentVersion = ResVersion( )
except ImportError:
    from ecl.util.util import Version
    currentVersion = Version.currentVersion( )



def get_source_ert(grid):
    x = np.zeros(grid.getNumActive(), np.float64)
    y = np.zeros(grid.getNumActive(), np.float64)
    z = np.zeros(grid.getNumActive(), np.float64)
    v = np.zeros(grid.getNumActive(), np.float64)
    for i in range(grid.getNumActive()):
        (x[i], y[i], z[i]) = grid.get_xyz(active_index=i)
        v[i] = grid.cell_volume(active_index=i)

    return x, y, z, v


@skipIf(currentVersion < (2, 2, "X"), "Must have at least version 2.2")
class OTSReservoirTest(ResTest):
    """
    Tests reading of grid from file, creation of surface

    Somewhat just testing EclGrid from ert...
    Should generate synthetic grid to test...
    """

    __name__ = 'OTSReservoirTest'

    def test_source(self):
        grid_path = self.createTestPath("Statoil/OTS/upscaled_model_28x61x35/SYNC_150109_28x61x35.EGRID")

        grid = EclGrid(grid_path, apply_mapaxes=False)

        x, y, z, v = get_source_ert(grid)

        np.testing.assert_array_almost_equal(x[0], 4.535518001622683e+05, 3)
        np.testing.assert_array_almost_equal(x[1], 4.538886969976674e+05, 3)
        np.testing.assert_array_almost_equal(x[len(x)/2-1], 4.579477462533208e+05, 2)
        np.testing.assert_array_almost_equal(x[len(x)/2], 4.582838834499333e+05, 2)
        np.testing.assert_array_almost_equal(x[-2], 4.613848046654874e+05, 2)
        np.testing.assert_array_almost_equal(x[-1], 4.609323121672277e+05, 2)

        np.testing.assert_array_almost_equal(y[0], 6.821185816913798e+06, 4)
        np.testing.assert_array_almost_equal(y[1], 6.820984599279324e+06, 4)
        np.testing.assert_array_almost_equal(y[len(y)/2-1], 6.818591851216132e+06, 4)
        np.testing.assert_array_almost_equal(y[len(y)/2], 6.818391674288494e+06, 4)
        np.testing.assert_array_almost_equal(y[-2], 6.817540282516188e+06, 4)
        np.testing.assert_array_almost_equal(y[-1], 6.817342974496862e+06, 4)

        np.testing.assert_array_almost_equal(z[0], 2.5608349e+03, 4)
        np.testing.assert_array_almost_equal(z[1], 2.6045288625e+03, 4)
        np.testing.assert_array_almost_equal(z[len(z)/2-1], 2.5348755e+03, 4)
        np.testing.assert_array_almost_equal(z[len(z)/2], 2.4366458625e+03, 4)
        np.testing.assert_array_almost_equal(z[-2], 2.8073701e+03, 4)
        np.testing.assert_array_almost_equal(z[-1], 2.8175391375e+03, 4)

        np.testing.assert_array_almost_equal(v[0]/100, 1.250112698672849e+06/100, 0)
        np.testing.assert_array_almost_equal(v[1]/100, 2.358535774861434e+06/100, 0)
        np.testing.assert_array_almost_equal(v[len(v)/2-1]/100, 7.417641691610970e+06/100, 0)
        np.testing.assert_array_almost_equal(v[len(v)/2]/100, 5.711322478719483e+06/100, 0)
        np.testing.assert_array_almost_equal(v[-2]/100, 2.790361609909404e+05/100, 0)
        np.testing.assert_array_almost_equal(v[-1]/100, 1.210308756793321e+06/100, 0)

    def test_res_surface(self):
        grid_path = self.createTestPath("Statoil/OTS/upscaled_model_28x61x35/SYNC_150109_28x61x35.EGRID")

        grid = EclGrid(grid_path, apply_mapaxes=False)

        rec = OTSResSurface(grid=grid, above=100)

        self.assertEqual(rec.nx, 28)
        self.assertEqual(rec.ny, 61)

        x = rec.x.reshape(28, 61).T.reshape(-1)
        y = rec.y.reshape(28, 61).T.reshape(-1)
        z = rec.z.reshape(28, 61).T.reshape(-1)
        
        self.assertAlmostEqual(x[0], 4.568478855576380e+05, 0)
        self.assertAlmostEqual(x[1], 4.572046162574860e+05, 0)
        self.assertAlmostEqual(x[len(x)/2-1], 4.549687493070726e+05, 0)
        self.assertAlmostEqual(x[len(x)/2], 4.552976845012890e+05, 0)
        self.assertAlmostEqual(x[-2], 4.536327852408870e+05, 0)
        self.assertAlmostEqual(x[-1], 4.538948201806216e+05, 0)

        self.assertAlmostEqual(y[0], 6.830726025121028e+06, 0)
        self.assertAlmostEqual(y[1], 6.830545927146919e+06, 0)
        self.assertAlmostEqual(y[len(y)/2-1], 6.817520789567830e+06, 0)
        self.assertAlmostEqual(y[len(y)/2], 6.817321535527016e+06, 0)
        self.assertAlmostEqual(y[-2], 6.804540078338952e+06, 0)
        self.assertAlmostEqual(y[-1], 6.804391019548364e+06, 0)

        self.assertAlmostEqual(z[0], 3.334453975e+03, 0)
        self.assertAlmostEqual(z[1], 3.29944525e+03, 0)
        self.assertAlmostEqual(z[len(z)/2-1], 2.3338267e+03, 0)
        self.assertAlmostEqual(z[len(z)/2], 2.303966e+03, 0)
        self.assertAlmostEqual(z[-2], 3.382618775e+03, 0)
        self.assertAlmostEqual(z[-1], 3.45438575e+03, 0)

    def test_surface(self):
        grid = EclGridGenerator.createRectangular(dims=(2, 2, 2), dV=(100, 100, 100), actnum=[0, 0, 0, 0, 1, 1, 1, 1])

        surface = OTSResSurface(grid=grid)

        z = np.ones(4)*100
        self.assertTrue(np.all(z == surface.z))

    def test_surface_above(self):
        grid = EclGridGenerator.createRectangular(dims=(2, 2, 2), dV=(100, 100, 100), actnum=[0, 0, 0, 0, 1, 1, 1, 1])

        surface = OTSResSurface(grid=grid, above=10)

        z = np.ones(4)*(100-10)
        self.assertTrue(np.all(z == surface.z))



if __name__ == "__main__":
    unittest.main()
