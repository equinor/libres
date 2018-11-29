from __future__ import print_function
import sys
from collections import namedtuple
import os.path
from datetime import datetime as dt
import numpy as np
from scipy.interpolate import CloughTocher2DInterpolator
from ecl.eclfile import EclFile, Ecl3DKW
from ecl.grid import EclGrid
from ecl.gravimetry import EclSubsidence
from ecl.util.geometry import Surface
from res.config import ContentTypeEnum, ConfigParser

from .ots_vel_surface import OTSVelSurface
from .ots_res_surface import OTSResSurface


def parse_file(config):
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
                                 'eclbase'
                                 ])

    vintage_pairs = namedtuple('Vintages', ['ts',
                                            'ts_simple',
                                            'dpv',
                                            'ts_rporv'])
    parms.seabed = None
    parms.above = None
    parms.youngs = None
    parms.poisson = None
    parms.rfactor = None
    parms.mapaxes = None
    parms.convention = 1
    parms.output_dir = None
    parms.horizon = None
    parms.ascii = None

    vintage_pairs.ts = []
    vintage_pairs.ts_simple = []
    vintage_pairs.dpv = []
    vintage_pairs.ts_rporv = []

    count = 0

    conf = ConfigParser()

    conf.add("ECLBASE", True, ContentTypeEnum.CONFIG_PATH)
    conf.add("VELOCITY", True, ContentTypeEnum.CONFIG_EXISTING_PATH)
    conf.add("ABOVERES", True, ContentTypeEnum.CONFIG_INT)
    conf.add("SEABED", True, ContentTypeEnum.CONFIG_INT)
    conf.add("YOUNGS", True, ContentTypeEnum.CONFIG_FLOAT)
    conf.add("POISSON", True, ContentTypeEnum.CONFIG_FLOAT)
    conf.add("RFACTOR", True, ContentTypeEnum.CONFIG_FLOAT)
    conf.add("MAPAXES", True, ContentTypeEnum.CONFIG_BOOL)
    conf.add("OUTPUT_DIR", True, ContentTypeEnum.CONFIG_STRING)
    conf.add("CONVENTION", False, ContentTypeEnum.CONFIG_INT)
    conf.add("HORIZON", False, ContentTypeEnum.CONFIG_STRING)
    conf.add("ASCII", False, ContentTypeEnum.CONFIG_STRING)

    schema_item = conf.add("DPV", False)
    schema_item.iset_type(0, ContentTypeEnum.CONFIG_STRING)
    schema_item.iset_type(1, ContentTypeEnum.CONFIG_STRING)

    schema_item = conf.add("TS", False)
    schema_item.iset_type(0, ContentTypeEnum.CONFIG_STRING)
    schema_item.iset_type(1, ContentTypeEnum.CONFIG_STRING)

    schema_item = conf.add("TS_SIMPLE", False)
    schema_item.iset_type(0, ContentTypeEnum.CONFIG_STRING)
    schema_item.iset_type(1, ContentTypeEnum.CONFIG_STRING)

    schema_item = conf.add("TS_RPORV", False)
    schema_item.iset_type(0, ContentTypeEnum.CONFIG_STRING)
    schema_item.iset_type(1, ContentTypeEnum.CONFIG_STRING)

    content = conf.parse(config)

    parms.eclbase = content.getValue("ECLBASE")
    parms.velocity_model = content.getValue("VELOCITY")
    parms.above = content.getValue("ABOVERES")
    parms.seabed = content.getValue("SEABED")
    parms.youngs = content.getValue("YOUNGS")
    parms.poisson = content.getValue("POISSON")
    parms.rfactor = content.getValue("RFACTOR")
    parms.mapaxes = content.getValue("MAPAXES")
    parms.output_dir = content.getValue("OUTPUT_DIR")

    if content.hasKey("CONVENTION"):
        parms.convention = content.getValue("CONVENTION")

    if content.hasKey("HORIZON"):
        parms.horizon = content.getValue("HORIZON")

    if content.hasKey("ASCII"):
        parms.ascii = content.getValue("ASCII")

    if "DPV" in content:
        lines = content["DPV"]
        for l in lines:
            base = dt.strptime(l[0], '%Y.%m.%d')
            monitor = dt.strptime(l[1], '%Y.%m.%d')
            vintage_pairs.dpv.append((base, monitor))
            count += 1
    if "TS" in content:
        lines = content["TS"]
        for l in lines:
            base = dt.strptime(l[0], '%Y.%m.%d')
            monitor = dt.strptime(l[1], '%Y.%m.%d')
            vintage_pairs.ts.append((base, monitor))
            count += 1
    if "TS_SIMPLE" in content:
        lines = content["TS_SIMPLE"]
        for l in lines:
            base = dt.strptime(l[0], '%Y.%m.%d')
            monitor = dt.strptime(l[1], '%Y.%m.%d')
            vintage_pairs.ts_simple.append((base, monitor))
            count += 1
    if "TS_RPORV" in content:
        lines = content["TS_RPORV"]
        for l in lines:
            base = dt.strptime(l[0], '%Y.%m.%d')
            monitor = dt.strptime(l[1], '%Y.%m.%d')
            vintage_pairs.ts_rporv.append((base, monitor))
            count += 1


    if count < 1:
        raise ValueError("No pairs of vintages found")

    return parms, vintage_pairs


def write_surface(vintage_pairs, ts, output_dir, type_str):
    for iv, vp in enumerate(vintage_pairs):
        d0 = vp[0].strftime('%Y_%m_%d')
        d1 = vp[1].strftime('%Y_%m_%d')
        ts[iv].write(output_dir+type_str+"/ots"+"_"+d0+"_"+d1+".irap")


def ots_run(parameter_file, verbose=False):

    parms, vintage_pairs = parse_file(parameter_file)

    ots = OTS(parms=parms, verbose=verbose)

    tshift_ts = ots.geertsma_ts(vintage_pairs.ts)
    tshift_ts_simple = ots.geertsma_ts_simple(vintage_pairs.ts_simple)
    tshift_dpv = ots.dpv(vintage_pairs.dpv)
    tshift_ts_rporv = ots.geertsma_ts_rporv(vintage_pairs.ts_rporv)

    surface_horizon = ots.get_horizon()

    write_surface(vintage_pairs.ts, tshift_ts, parms.output_dir, "_ts")
    write_surface(vintage_pairs.ts_simple, tshift_ts_simple, parms.output_dir, "_ts_simple")
    write_surface(vintage_pairs.dpv, tshift_dpv, parms.output_dir, "_dpv")
    write_surface(vintage_pairs.ts_rporv, tshift_ts_rporv, parms.output_dir, "_ts_rporv")

    if parms.horizon is not None:
        surface_horizon.write(parms.horizon)

    num_pairs = (
            len(vintage_pairs.ts)
            + len(vintage_pairs.ts_simple)
            + len(vintage_pairs.dpv)
            + len(vintage_pairs.ts_rporv)
            )
    line = "{}, {}, {}" + ", {}"*num_pairs + "\n"

    if parms.ascii is not None:
        with open(parms.ascii, 'w') as f:
            for point in range(len(surface_horizon)):
                xy = surface_horizon.getXY(point)
                ts = []
                for iv in range(len(vintage_pairs.ts)):
                    ts.append(tshift_ts[iv][point])
                for iv in range(len(vintage_pairs.ts_simple)):
                    ts.append(tshift_ts_simple[iv][point])
                for iv in range(len(vintage_pairs.dpv)):
                    ts.append(tshift_dpv[iv][point])
                for iv, _ in enumerate(vintage_pairs.ts_rporv):
                    ts.append(tshift_rprov[iv][point])
                f.write(line.format(xy[0], xy[1], surface_horizon[point], *ts))


class OTS(object):

    def __init__(self, parms, verbose=False):
        """
        The OTS class manages the information required to calculate
        overburden timeshift.

        The constructor will look for the Eclipse files INIT, EGRID
        and UNRST based on the input case, if some of the files are
        missing an exception will be raised. It will then instantiate
        a EclSubsidence object will be used to manage the rest of the
        overburden timeshift calculations.
        """

        if parms.eclbase is None:
            raise RuntimeError

        case = os.path.splitext(parms.eclbase)[0]
        self._init_file = EclFile("%s.INIT" % case)
        self._rst_file = EclFile("%s.UNRST" % case)
        self._grid = EclGrid("%s.EGRID" % case, apply_mapaxes=parms.mapaxes)

        self.subsidence = EclSubsidence(self._grid, self._init_file)

        self._seabed = parms.seabed
        if not isinstance(parms.youngs, float):
            parms.youngs = 0
        self._youngs_modulus = parms.youngs*1e9
        self._poisson_ratio = parms.poisson
        self._r_factor = parms.rfactor
        self._convention = parms.convention
        self._verbose = verbose

        self._surface = res_surface = OTSResSurface(grid=self._grid, above=parms.above)
        if parms.velocity_model:
            self._surface = OTSVelSurface(res_surface=res_surface, vcube=parms.velocity_model)

        self._restart_views = {}

    def get_horizon(self):
        return self._create_surface()

    def _create_surface(self, z=None):
        """
        Generate irap surface

        :param z: replace z values of surface
        """
        nx = self._surface.nx
        ny = self._surface.ny
        x = self._surface.x
        y = self._surface.y
        if z is None:
            z = self._surface.z

        xstart = np.min(x)
        ystart = np.min(y)

        if nx < 2 or ny < 2:
            raise RuntimeError("Cannot create IRAP surface if nx or ny is <2")

        xinc = (np.max(x) - xstart) / (nx - 1)
        yinc = (np.max(y) - ystart) / (ny - 1)

        surf = Surface(nx=nx, ny=ny, xinc=xinc, yinc=yinc,
                       xstart=xstart, ystart=ystart, angle=0)

        irap_x = np.empty(nx * ny)
        irap_y = np.array(irap_x)

        for i, s in enumerate(surf):
            irap_x[i], irap_y[i] = surf.getXY(i)

        # Interpolate vel grid to irap grid, should be the same apart from ordering
        z = np.nan_to_num(z)
        ip = CloughTocher2DInterpolator((x, y), z, fill_value=0)
        irap_z = ip(irap_x, irap_y)

        for i in range(len(surf)):
            surf[i] = irap_z[i]

        return surf

    def addSurvey(self, name, date):
        """The addSurvey() method will register a survey at a specific date.

        The name argument should be a unique string, this will later
        be used when evaluating the elastic strain in the
        overburden. The date should be python datetime.date()
        instance, this date should be present as a report step in the
        restart file - otherwise an exception will be raised.
        """
        restart_view = self._rst_file.restartView(sim_time=date)
        self.subsidence.add_survey_PRESSURE(name, restart_view)

        self._restart_views[name] = restart_view

        return restart_view

    @staticmethod
    def _divide_negative_shift(ts):
        for i in range(len(ts)):
            if ts[i] < 0:
                ts[i] /= 5.0


    def geertsma_ts_rporv(self, vintage_pairs):
        """
        Calculates TS without using velocity. Fast.
        Velocity is only used to get the surface on the velocity grid.
        Uses change in porevolume from Eclipse (RPORV in .UNRST) as input to
        Geertsma model.

        :param vintage_pairs:
        """

        if len(vintage_pairs) == 0:
            return 0, []

        vintages = self._vintages_name_date(vintage_pairs)
        surface = self._surface
        num_points = len(surface)
        num_points_calculated = 0
        du = np.zeros((len(vintages.name), num_points))
        ts_surfaces = []
        for iv, (vn, vd) in enumerate(zip(vintages.name, vintages.date)):
            if self._verbose:
                print("{:%x %X} TS_RPORV: Calculating vintage {:%Y.%m.%d}".format(dt.now(), vd))
                sys.stdout.flush()

            self.addSurvey(vn, vd)

            num_points_calculated = 0
            for point in range(num_points):

                if not np.isnan(surface.z[point]):
                    num_points_calculated += 1
                    r1 = (surface.x[point], surface.y[point], 0)
                    r2 = (surface.x[point], surface.y[point], surface.z[point]-self._seabed)
                    # subsidence and displacement have opposite sign
                    # should have minus on dz1 and dz2 here, more efficient when calculating ts
                    dz1 = self.subsidence.eval_geertsma_rporv(base_survey=vn,
                                                             monitor_survey=None,
                                                             pos=r1,
                                                             youngs_modulus=self._youngs_modulus,
                                                             poisson_ratio=self._poisson_ratio,
                                                             seabed=self._seabed)
                    dz2 = self.subsidence.eval_geertsma_rporv(base_survey=vn,
                                                             monitor_survey=None,
                                                             pos=r2,
                                                             youngs_modulus=self._youngs_modulus,
                                                             poisson_ratio=self._poisson_ratio,
                                                             seabed=self._seabed)
                    du[iv, point] = dz2 - dz1

        for ivp, vp in enumerate(vintage_pairs):
            if self._verbose:
                if self._convention == 1:
                    print("{:%x %X} TS_RPORV: Calculating shift {:%Y.%m.%d}-{:%Y.%m.%d} in {} points".
                          format(dt.now(), vp[0], vp[1], num_points_calculated))
                if self._convention == -1:
                    print("{:%x %X} TS_RPORV: Calculating shift {:%Y.%m.%d}-{:%Y.%m.%d} in {} points".
                          format(dt.now(), vp[1], vp[0], num_points_calculated))
                if num_points_calculated == 0:
                    print("Shift was calculated in 0 points.", file=sys.stderr)
                    print("Consider changing --apply_mapaxes value.")

                sys.stdout.flush()
            base_index = vintages.date.index(vp[0])
            monitor_index = vintages.date.index(vp[1])
            ts = -self._r_factor * (du[monitor_index] - du[base_index])
            self._divide_negative_shift(ts)

            ts = ts*self._convention

            ts_surfaces.append(self._create_surface(ts))

        return ts_surfaces


    def geertsma_ts_simple(self, vintage_pairs):
        """
        Calculates TS without using velocity. Fast.
        Velocity is only used to get the surface on the velocity grid.

        :param vintage_pairs:
        """

        if len(vintage_pairs) < 1:
            return 0, []

        vintages = self._vintages_name_date(vintage_pairs)
        surface = self._surface
        num_points = len(surface)
        num_points_calculated = 0
        du = np.zeros((len(vintages.name), num_points))
        ts_surfaces = []
        for iv, (vn, vd) in enumerate(zip(vintages.name, vintages.date)):
            if self._verbose:
                print("{:%x %X} TS_SIMPLE: Calculating vintage {:%Y.%m.%d}".format(dt.now(), vd))
                sys.stdout.flush()

            self.addSurvey(vn, vd)

            num_points_calculated = 0
            for point in range(num_points):

                if not np.isnan(surface.z[point]):
                    num_points_calculated += 1
                    r1 = (surface.x[point], surface.y[point], 0)
                    r2 = (surface.x[point], surface.y[point], surface.z[point]-self._seabed)
                    # subsidence and displacement have opposite sign
                    # should have minus on dz1 and dz2 here, more efficient when calculating ts
                    dz1 = self.subsidence.evalGeertsma(base_survey=vn,
                                                       monitor_survey=None,
                                                       pos=r1,
                                                       youngs_modulus=self._youngs_modulus,
                                                       poisson_ratio=self._poisson_ratio,
                                                       seabed=self._seabed)
                    dz2 = self.subsidence.evalGeertsma(base_survey=vn,
                                                       monitor_survey=None,
                                                       pos=r2,
                                                       youngs_modulus=self._youngs_modulus,
                                                       poisson_ratio=self._poisson_ratio,
                                                       seabed=self._seabed)
                    du[iv, point] = dz2 - dz1

        for ivp, vp in enumerate(vintage_pairs):
            if self._verbose:
                if self._convention == 1:
                    print("{:%x %X} TS_SIMPLE: Calculating shift {:%Y.%m.%d}-{:%Y.%m.%d} in {} points".
                          format(dt.now(), vp[0], vp[1], num_points_calculated))
                if self._convention == -1:
                    print("{:%x %X} TS_SIMPLE: Calculating shift {:%Y.%m.%d}-{:%Y.%m.%d} in {} points".
                          format(dt.now(), vp[1], vp[0], num_points_calculated))
                if num_points_calculated == 0:
                    print("Shift was calculated in 0 points.", file=sys.stderr)
                    print("Consider changing --apply_mapaxes value.")

                sys.stdout.flush()
            base_index = vintages.date.index(vp[0])
            monitor_index = vintages.date.index(vp[1])
            ts = -self._r_factor * (du[monitor_index] - du[base_index])
            self._divide_negative_shift(ts)

            ts = ts*self._convention

            ts_surfaces.append(self._create_surface(ts))

        return ts_surfaces

    def geertsma_ts(self, vintage_pairs):
        """
        Calculates TS using velocity. Slow.

        :param vintage_pairs:
        """

        if len(vintage_pairs) < 1:
            return 0, []

        surface = self._surface
        num_points = len(surface)

        ts_surfaces = []
        _, nz = surface.z3d.shape

        for ivp, vp in enumerate(vintage_pairs):
            du = np.zeros(num_points)
            if self._verbose:
                print("{:%x %X} TS: Calculating vintage {:%Y.%m.%d} - {:%Y.%m.%d}".format(dt.now(), vp[0], vp[1]))
                sys.stdout.flush()

            self.addSurvey("base", vp[1])
            self.addSurvey("monitor", vp[0])

            num_points_calculated = 0
            for point in range(num_points):

                if not np.isnan(surface.z[point]):
                    num_points_calculated += 1
                    for iz in range(nz):
                        rz = surface.z3d[point, iz]-self._seabed
                        if 0 <= rz <= (surface.z[point]-self._seabed):
                            r1 = (surface.x[point], surface.y[point], rz)
                            r2 = (surface.x[point], surface.y[point], rz + 0.1)
                            # subsidence and displacement have opposite sign
                            # should have minus here, more efficient when calculating ts
                            dz1 = self.subsidence.evalGeertsma(base_survey="base",
                                                               monitor_survey="monitor",
                                                               pos=r1,
                                                               youngs_modulus=self._youngs_modulus,
                                                               poisson_ratio=self._poisson_ratio,
                                                               seabed=self._seabed)
                            dz2 = self.subsidence.evalGeertsma(base_survey="base",
                                                               monitor_survey="monitor",
                                                               pos=r2,
                                                               youngs_modulus=self._youngs_modulus,
                                                               poisson_ratio=self._poisson_ratio,
                                                               seabed=self._seabed)

                            ezz = (dz2 - dz1) * 10
                            du[point] += ezz

            if self._verbose:
                if self._convention == 1:
                    print("{:%x %X} TS: Calculating shift {:%Y.%m.%d}-{:%Y.%m.%d} in {} points".
                          format(dt.now(), vp[0], vp[1], num_points_calculated))
                if self._convention == -1:
                    print("{:%x %X} TS: Calculating shift {:%Y.%m.%d}-{:%Y.%m.%d} in {} points".
                          format(dt.now(), vp[1], vp[0], num_points_calculated))
                if num_points_calculated == 0:
                    print("Shift was calculated in 0 points.", file=sys.stderr)
                    print("Consider changing --apply_mapaxes value.")

            # subsidence and displacement have opposite sign, thus the minus sign
            ts = -self._r_factor*du*surface.dt*1000
            self._divide_negative_shift(ts)

            ts = ts*self._convention

            ts_surfaces.append(self._create_surface(ts))

        return ts_surfaces

    @staticmethod
    def _vintages_name_date(vintage_pairs):
        vintages = set()
        for vp in vintage_pairs:
            vintages.add(vp[0])
            vintages.add(vp[1])
        vintages_date = list(vintages)
        vintages_name = []
        for i in range(len(vintages_date)):
            vintages_name.append("S{}".format(i))

        Vintages = namedtuple("Vintages", "name date")
        return Vintages(vintages_name, vintages_date)

    def dpv(self, vintage_pairs):
        """
        Calulates change in pressure multiplied by cell volume
        and sum for all cells in column

        dPV must have equal sign as TS, but opposite from mathematics

        monitor-base

        :param vintage_pairs: list of pairs of vintages
        :return:
        """

        if len(vintage_pairs) < 1:
            return 0, []

        vintages = self._vintages_name_date(vintage_pairs)

        surf = self._surface

        num_points = len(surf)
        num_points_calculated = 0

        shift_surfaces = []
        pv = np.zeros((len(vintages.name), num_points))

        for iv, (vn, vd) in enumerate(zip(vintages.name, vintages.date)):
            if self._verbose:
                print("{:%x %X} DPV: Calculating vintage {:%Y.%m.%d}".format(dt.now(), vd))
                sys.stdout.flush()

            self.addSurvey(vn, vd)
            pressure = Ecl3DKW.castFromKW(self._restart_views[vn]['PRESSURE'][0], self._grid)
            num_points_calculated = 0
            for point in range(num_points):

                if not np.isnan(surf.z[point]):
                    r = surf.x[point], surf.y[point], 0
                    try:
                        i, j = self._grid.findCellXY(*r)
                        num_points_calculated += 1
                        sum_pv = 0
                        for k in range(self._grid.getNZ()):
                            v = self._grid.cell_volume(ijk=(i, j, k))
                            sum_pv += pressure[i, j, k] * v

                        pv[iv, point] = sum_pv
                    except ValueError:
                        pv[iv, point] = 0
        for ivp, vp in enumerate(vintage_pairs):
            if self._verbose:
                if self._convention == 1:
                    print("{:%x %X} DPV: Calculating shift {:%Y.%m.%d}-{:%Y.%m.%d} in {} points".
                          format(dt.now(), vp[0], vp[1], num_points_calculated))
                if self._convention == -1:
                    print("{:%x %X} DPV: Calculating shift {:%Y.%m.%d}-{:%Y.%m.%d} in {} points".
                          format(dt.now(), vp[1], vp[0], num_points_calculated))
                if num_points_calculated == 0:
                    print("Overburden timeshift was calculated in 0 points.", file=sys.stderr)
                    print("Consider changing --apply_mapaxes value.")

            base_index = vintages.date.index(vp[0])
            monitor_index = vintages.date.index(vp[1])
            dpv = (pv[monitor_index] - pv[base_index]) / 1e9 * self._convention
            shift_surfaces.append(self._create_surface(dpv))

        return shift_surfaces
