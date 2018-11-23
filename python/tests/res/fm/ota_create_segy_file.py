import numpy as np

import segyio
from segyio import TraceField


def create_segy_file(name, spec, trace=None, il=None, xl=None, cdp_x=None, cdp_y=None):

    if trace is None:
        trace = np.empty(len(spec.samples), dtype=np.single)
        for i in range(len(spec.samples)):
            trace[i] = 1500+i

    l = [50, 150]
    if il is None:
        il = l
    if xl is None:
        xl = l
    if cdp_x is None:
        cdp_x = l
    if cdp_y is None:
        cdp_y = l

    with segyio.create(name, spec) as f:

        scalar = 1

        for i, v in enumerate(spec.ilines):
            f.header.iline[v] = {TraceField.SourceGroupScalar: scalar}
            if il is not None:
                f.header.iline[v] = {TraceField.INLINE_3D: il[i]}
            if cdp_y is not None:
                f.header.iline[v] = {TraceField.CDP_Y: cdp_y[i]}

        for i, v in enumerate(spec.xlines):
            f.header.xline[v] = {TraceField.SourceGroupScalar: scalar}
            if xl is not None:
                f.header.xline[v] = {TraceField.CROSSLINE_3D: xl[i]}
            if cdp_x is not None:
                f.header.xline[v] = {TraceField.CDP_X: cdp_x[i]}

        if trace is not None:
            for i in range(len(f.trace)):
                f.trace[i] = trace
