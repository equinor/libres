#  Copyright (C) 2017  Statoil ASA, Norway.
#
#  The file 'keep_runpath_enum.py' is part of ERT - Ensemble based Reservoir Tool.
#
#  ERT is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  ERT is distributed in the hope that it will be useful, but WITHOUT ANY
#  WARRANTY; without even the implied warranty of MERCHANTABILITY or
#  FITNESS FOR A PARTICULAR PURPOSE.
#
#  See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html>
#  for more details.

from cwrap import BaseCEnum

class KeepRunpath(BaseCEnum):
    TYPE_NAME = "keep_runpath_enum"

    DEFAULT_KEEP    = None
    EXPLICIT_DELETE = None
    EXPLICIT_KEEP   = None


KeepRunpath.addEnum("DEFAULT_KEEP",    0)
KeepRunpath.addEnum("EXPLICIT_DELETE", 1)
KeepRunpath.addEnum("EXPLICIT_KEEP",   2)
