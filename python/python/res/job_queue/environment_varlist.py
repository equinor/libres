#  Copyright (C) 2017  Statoil ASA, Norway.
#
#  This file is part of ERT - Ensemble based Reservoir Tool.
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


from cwrap import BaseCClass
from res.job_queue import QueuePrototype

class EnvironmentVarlist(BaseCClass):
  TYPE_NAME      = "env_varlist"
  
  _alloc                    = QueuePrototype("void* env_varlist_alloc()", bind=False)
  _free                     = QueuePrototype("void env_varlist_free( env_varlist )")
  _setenv                   = QueuePrototype("void env_varlist_setenv(env_varlist, char*, char*)")

  def __init__(self):
    c_ptr = self._alloc()
    super(EnvironmentVarlist , self).__init__(c_ptr)


  def SetEnv(self, var, value):
    self._setenv(var, value)

  def free(self):
    self._free( )


