import numpy as np

from scipy.interpolate import CloughTocher2DInterpolator

import segyio
from segyio import TraceField


class OTSVelSurface(object):

    def __init__(self, res_surface, vcube):
        """
        Create a surface where the timeshift can be calculated.

        Read a reservoir
        """

        self._x = None
        self._y = None
        self._z = None

        self._nx = None
        self._ny = None

        self._z3d = None
        self._dt = None

        self._mapReservoirSurfaceToVelocity(res_surface, vcube)

    @property
    def x(self):
        return self._x

    @property
    def y(self):
        return self._y

    @property
    def z(self):
        return self._z

    @property
    def nx(self):
        return self._nx

    @property
    def ny(self):
        return self._ny

    @property
    def z3d(self):
        return self._z3d

    @property
    def dt(self):
        return self._dt

    def __len__(self):
        return len(self.x)

    def __str__(self):

        s = "x: " + str(self.x)
        s += "\ny: " + str(self.y)
        s += "\nz: " + str(self.z)
        s += "\ndt=" + str(self.dt)
        s += "\nz3d=" + str(self.z3d)
        return s

    def _read_velocity(self, vcube, cell_corners):
        """
        Read velocity from segy file. Upscale.
        :param vcube:
        :param cell_corners:
        :return:
        """

        with segyio.open(vcube) as f:
            x = np.empty(f.tracecount, dtype=np.float64)
            y = np.empty(f.tracecount, dtype=np.float64)
            dt = segyio.dt(f) / 1e6
            nt = len(f.samples)

            for i, h in enumerate(f.header):
                scalar = h[TraceField.SourceGroupScalar]
                if scalar < 0:
                    scalar = -1. / scalar
                x[i] = h[TraceField.CDP_X] * scalar
                y[i] = h[TraceField.CDP_Y] * scalar

            # TODO nx <-> ny??
            ny = len(f.xlines)
            nx = len(f.ilines)

            # TODO memory issue?
            traces = f.trace.raw[:]

            if f.sorting == segyio.TraceSortingFormat.INLINE_SORTING:
                x = x.reshape(ny, nx)
                y = y.reshape(ny, nx)
                traces = traces.reshape(ny, nx, nt)
                x = np.transpose(x)
                y = np.transpose(y)
                traces = np.transpose(traces, (1, 0, 2))
            elif f.sorting == segyio.TraceSortingFormat.CROSSLINE_SORTING:
                x = x.reshape(nx, ny)
                y = y.reshape(nx, ny)
                traces = traces.reshape(nx, ny, nt)
            else:
                raise RuntimeError

        x, y, traces, nt, dt = self._upscale_velocity(cell_corners, x, y, traces, nt, dt)

        return x, y, traces, nt, dt

    @staticmethod
    def _upscaling_size_stepping(res_corners, axis, vel_axis):
        """
        Upscales axis of velocity.

        :param res_corners: All corner points of reservoir model, along @axis
        :param axis: Gives which axis is being upscaled. 0 for x, 1 for y
        :param vel_axis: axis of model, x or y given by @axis
        :return:
        """
        res_n = res_corners.shape[axis]
        size = np.mean(np.max(res_corners, axis) - np.min(res_corners, axis)) / res_n
        nn = np.ceil(np.mean(np.max(vel_axis, axis) - np.min(vel_axis, axis)) / size).astype(np.int)

        n = vel_axis.shape[axis]
        ups = np.floor((n - 1) / nn).astype(np.int)
        if ups < 1:
            ups = 1
            nn = n

        return nn, ups

    def _upscale_velocity(self, res_corners, x_vel, y_vel, traces, nt, dt):

        nxx, upsx = self._upscaling_size_stepping(res_corners[:, :, 0], 0, x_vel)

        nyy, upsy = self._upscaling_size_stepping(res_corners[:, :, 1], 1, y_vel)

        ntt = int(np.floor(nt / (16e-3 / dt)))
        upst = int(np.floor((nt - 1) / ntt))
        if upst == 0:
            upst = 1
            ntt = nt
        dt = dt * upst

        x = x_vel[0:(nxx - 1) * upsx + 1:upsx,
                  0:(nyy - 1) * upsy + 1:upsy]
        y = y_vel[0:(nxx - 1) * upsx + 1:upsx,
                  0:(nyy - 1) * upsy + 1:upsy]

        traces_upscaled = traces[0:(nxx - 1) * upsx + 1:upsx,
                                   0:(nyy - 1) * upsy + 1:upsy,
                                   0:(ntt - 1) * upst + 1:upst]

        return x, y, traces_upscaled, nt, dt

    def _mapReservoirSurfaceToVelocity(self, res_surface, vcube):
        """
        Interpolates reservoir top surface to velocity grid
        """

        x, y, traces, nt, self._dt = self._read_velocity(vcube, res_surface.cell_corners)

        vel_t_int = np.zeros_like(traces)
        vel_t_int[:, :, 1:] = (traces[:, :, 0:-1] + traces[:, :, 1:]) / 2
        self._z3d = np.cumsum(vel_t_int * self._dt / 2, 2).reshape(-1, vel_t_int.shape[-1])

        ip = CloughTocher2DInterpolator((res_surface.x, res_surface.y), res_surface.z)
        z = ip(x, y)

        self._nx, self._ny = x.shape
        self._x = x.reshape(-1)
        self._y = y.reshape(-1)
        self._z = z.reshape(-1)

