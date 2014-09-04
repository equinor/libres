#  Copyright (C) 2013  Statoil ASA, Norway. 
#   
#  The file 'time_map.py' is part of ERT - Ensemble based Reservoir Tool. 
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
import os
import errno 

from ert.cwrap import CWrapper, BaseCClass
from ert.enkf import ENKF_LIB
from ert.util import CTime


class TimeMap(BaseCClass):
    def __init__(self, filename = None):
        c_ptr = TimeMap.cNamespace().alloc()
        super(TimeMap, self).__init__(c_ptr)
        if filename:
            self.load(filename)


    def load(self, filename):
        if os.path.isfile( filename ):
            TimeMap.cNamespace().load(self , filename)
        else:
            raise IOError(( errno.ENOENT , "File not found: %s" % filename))


    def fload(self , filename):
        """
        Will load a timemap as a formatted file consisting of a list of dates: DD/MM/YYYY
        """
        if os.path.isfile( filename ):
            OK = TimeMap.cNamespace().fload(self , filename)
            if not OK:
                raise Exception("Error occured when loading timemap from:%s" % filename)
        else:
            raise IOError(( errno.ENOENT , "File not found: %s" % filename))



    def isStrict(self):
        return TimeMap.cNamespace().is_strict( self )


    def setStrict(self , strict):
        return TimeMap.cNamespace().set_strict( self , strict)
        

    def getSimulationDays(self, step):
        """ @rtype: double """
        if not isinstance(step, int):
            raise TypeError("Expected an integer")

        size = len(self)
        if step < 0 or step >= size:
            raise IndexError("Index out of range: 0 <= %d < %d" % (step, size))

        return TimeMap.cNamespace().iget_sim_days(self, step)


    def __getitem__(self, index):
        """ @rtype: CTime """
        if not isinstance(index, int):
            raise TypeError("Expected an integer")

        size = len(self)
        if index < 0 or index >= size:
            raise IndexError("Index out of range: 0 <= %d < %d" % (index, size))

        return TimeMap.cNamespace().iget(self, index)



    def update(self , index , time):
        if TimeMap.cNamespace().try_update(self , index , CTime(time)):
            return True
        else:
            if self.isStrict():
                raise Exception("Tried to update with inconsistent value")
            else:
                return False
            


    def __iter__(self):
        cur = 0

        while cur < len(self):
            yield self[cur]
            cur += 1

    def __len__(self):
        """ @rtype: int """
        return TimeMap.cNamespace().size(self)

    def free(self):
        TimeMap.cNamespace().free(self)


    def dump(self):
        """ 
        Will return a list of tuples (step , CTime , days).
        """
        step_list = []
        for step,t in enumerate(self):
            step_list.append( (step , t , self.getSimulationDays( step )) )
        return step_list



        
##################################################################
cwrapper = CWrapper(ENKF_LIB)
cwrapper.registerType("time_map", TimeMap)
cwrapper.registerType("time_map_obj", TimeMap.createPythonObject)
cwrapper.registerType("time_map_ref", TimeMap.createCReference)


##################################################################
##################################################################

TimeMap.cNamespace().free = cwrapper.prototype("void time_map_free( time_map )")
TimeMap.cNamespace().fread_alloc_readonly = cwrapper.prototype("c_void_p time_map_fread_alloc_readonly(char*)")
TimeMap.cNamespace().alloc = cwrapper.prototype("c_void_p time_map_alloc()")
TimeMap.cNamespace().load = cwrapper.prototype("bool time_map_fread(time_map , char*)")
TimeMap.cNamespace().fload = cwrapper.prototype("bool time_map_fscanf(time_map , char*)")
TimeMap.cNamespace().iget_sim_days = cwrapper.prototype("double time_map_iget_sim_days(time_map, int)")
TimeMap.cNamespace().iget = cwrapper.prototype("time_t time_map_iget(time_map, int)")
TimeMap.cNamespace().size = cwrapper.prototype("int time_map_get_size(time_map)")
TimeMap.cNamespace().try_update = cwrapper.prototype("bool time_map_try_update(time_map , int , time_t)")
TimeMap.cNamespace().is_strict = cwrapper.prototype("bool time_map_is_strict( time_map )")
TimeMap.cNamespace().set_strict = cwrapper.prototype("void time_map_set_strict( time_map , bool)")
