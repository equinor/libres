from ecl.eclfile import EclKW, openFortIO, FortIO
# TODO remove when this is in stable
try:
    from ecl import EclDataType
except ImportError:
    pass


def create_init(grid, case):
    poro = EclKW("PORO", grid.getNumActive(), EclDataType.ECL_FLOAT)
    porv = EclKW("PORV", grid.getGlobalSize(), EclDataType.ECL_FLOAT)

    with openFortIO("%s.INIT" % case, mode=FortIO.WRITE_MODE) as f:
        poro.fwrite(f)
        porv.fwrite(f)


def create_restart(grid, case):
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

        seq_hdr[0] = 2
        header[66] = 2010
        for i in range(len(p)):
            p[i] = 20

        seq_hdr.fwrite(f)
        header.fwrite(f)
        p.fwrite(f)

        seq_hdr[0] = 3
        header[66] = 2011
        for i in range(len(p)):
            p[i] = 25

        seq_hdr.fwrite(f)
        header.fwrite(f)
        p.fwrite(f)
