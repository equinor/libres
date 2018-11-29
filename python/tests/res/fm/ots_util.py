from ecl.eclfile import EclKW, openFortIO, FortIO
from ecl import EclDataType

def create_init(grid, case):
    poro = EclKW("PORO", grid.getNumActive(), EclDataType.ECL_FLOAT)
    porv = EclKW("PORV", grid.getGlobalSize(), EclDataType.ECL_FLOAT)

    with openFortIO("%s.INIT" % case, mode=FortIO.WRITE_MODE) as f:
        poro.fwrite(f)
        porv.fwrite(f)


def create_restart(grid, case, rporv=None):
    with openFortIO("%s.UNRST" % case, mode=FortIO.WRITE_MODE) as f:
        seq_hdr = EclKW("SEQNUM", 1, EclDataType.ECL_FLOAT)
        seq_hdr[0] = 1
        p = EclKW("PRESSURE", grid.getNumActive(), EclDataType.ECL_FLOAT)
        p.assign(1)

        for i in range(len(p)):
            p[i] = 10

        header = EclKW("INTEHEAD", 67, EclDataType.ECL_INT)
        header[64] = 1
        header[65] = 1
        header[66] = 2000

        seq_hdr.fwrite(f)
        header.fwrite(f)
        p.fwrite(f)

        if rporv:
            rp = EclKW("RPORV", grid.getNumActive(), EclDataType.ECL_FLOAT)
            for idx, val in enumerate(rporv):
                rp[idx] = val
            rp.fwrite(f)

        seq_hdr[0] = 2
        header[66] = 2010
        for i in range(len(p)):
            p[i] = 20

        seq_hdr.fwrite(f)
        header.fwrite(f)
        p.fwrite(f)

        if rporv:
            rp = EclKW("RPORV", grid.getNumActive(), EclDataType.ECL_FLOAT)
            for idx, val in enumerate(rporv):
                rp[idx] = val
            rp.fwrite(f)

        seq_hdr[0] = 3
        header[66] = 2011
        for i in range(len(p)):
            p[i] = 25

        seq_hdr.fwrite(f)
        header.fwrite(f)
        p.fwrite(f)

        if rporv:
            rp = EclKW("RPORV", grid.getNumActive(), EclDataType.ECL_FLOAT)
            for idx, val in enumerate(rporv):
                rp[idx] = val
            rp.fwrite(f)
