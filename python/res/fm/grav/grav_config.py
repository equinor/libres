from collections import OrderedDict
import os.path
import sys

from ert.config import ConfigParser, ContentTypeEnum
from ecl import EclPhaseEnum
from .grav_obs import GravObs

class Survey(object):
    def __init__(self , name , date , type_string):
        
        self.name = name
        self.date = date
        self.type_string = type_string
        

class Station(object):
    def __init__(self , name , x , y , z):
        self.name = name
        self.x = x
        self.y = y
        self.z = z

        

class Result(object):
    def __init__(self , name , base, monitor):
        self.name = name
        self.base = base
        self.monitor = monitor
        

class GravConfig(object):

    def __init__(self , config_file , rst_file):
        if os.path.isfile( config_file ):
            pass
        else:
            raise IOError("No such file: %s" % config_file)

        self.parse( config_file , rst_file )

        
        
    def parse(self, config_file, rst_file):
        self.std_density = {}
        self._surveys = OrderedDict()
        self._stations = OrderedDict()
        self._results = OrderedDict()
        self._obs_data = OrderedDict()
        
        parser = ConfigParser( )
        parser.add("OIL_DEN" , value_type = ContentTypeEnum.CONFIG_FLOAT )
        parser.add("GAS_DEN" , value_type = ContentTypeEnum.CONFIG_FLOAT )
        parser.add("WAT_DEN" , value_type = ContentTypeEnum.CONFIG_FLOAT )


        item = parser.add("STATION")
        item.iset_type(0 , ContentTypeEnum.CONFIG_STRING )
        item.iset_type(1 , ContentTypeEnum.CONFIG_FLOAT )
        item.iset_type(2 , ContentTypeEnum.CONFIG_FLOAT )
        item.iset_type(3 , ContentTypeEnum.CONFIG_FLOAT )
        item.set_argc_minmax( 4 , 4 )


        item = parser.add("SURVEY")
        item.iset_type(0 , ContentTypeEnum.CONFIG_STRING )
        item.iset_type(1 , ContentTypeEnum.CONFIG_ISODATE )
        item.iset_type(2 , ContentTypeEnum.CONFIG_STRING )
        item.set_argc_minmax( 3 , 3 )
        for kw in ["PORV_MOD", "RFIP", "RPORV"]:
            if kw in rst_file:
                item.addAlternative( 2 , kw)


        # The FIP alternative will be available if at least one of
        # FIPxxxx keywords is present; that is strictly speaking not
        # enough.
        has_fip = False
        for kw in ["FIPOIL","FIPGAS","FIPWAT"]:
            if kw in rst_file:
                has_fip = True
        if has_fip:
            item.addAlternative( 2 , "FIP" )

            
        item = parser.add("RESULT")
        item.iset_type(0 , ContentTypeEnum.CONFIG_STRING )
        item.iset_type(1 , ContentTypeEnum.CONFIG_STRING )
        item.iset_type(2 , ContentTypeEnum.CONFIG_STRING )
        item.iset_type(3 , ContentTypeEnum.CONFIG_EXISTING_PATH )
        item.set_argc_minmax( 3 , 4 )
        
        
        content = parser.parse(config_file , validate = False)
        if not content.isValid():
            sys.stderr.write("Errors when parsing: %s\n" % config_file)
            for count,error in enumerate(content.getErrors()):
                sys.stderr.write(" %02d: %s \n" % (count , error))
                raise SystemExit('Could not correctly parse configuration file "%s"' % config_file)
        
            
        if "OIL_DEN" in content:
            self.std_density[EclPhaseEnum.ECL_OIL_PHASE] = content.getValue( "OIL_DEN")

        if "GAS_DEN" in content:
            self.std_density[EclPhaseEnum.ECL_GAS_PHASE] = content.getValue( "GAS_DEN")

        if "WAT_DEN" in content:
            self.std_density[EclPhaseEnum.ECL_WATER_PHASE] = content.getValue( "WAT_DEN")


        for survey in content["SURVEY"]:
            name = survey[0]
            date = survey[1]
            type_string = survey[2]
            if rst_file.has_sim_time( date ):
                self._surveys[name] = Survey( name , date , type_string)
            else:
                raise ValueError("The survey date: %s was not found in the restart file" % date)

            

        for res in content["RESULT"]:
            name = res[0]
            base = res[1]
            monitor = res[2]
            if not base in self._surveys:
                raise KeyError("Invalid survey name:%s " % base)

            if not monitor in self._surveys:
                raise KeyError("Invalid survey name:%s " % monitor)
            
            self._results[name] = Result( name , base, monitor )
            if len(res) == 4:
                self._obs_data[name] = GravObs( res[3] )
            

        for st in content["STATION"]:
            name = st[0]
            x = st[1]
            y = st[2]
            z = st[3]

            self._stations[name] = Station( name , x , y , z  )


    def stdDensity(self):
        return self.std_density


    def surveys(self):
        return self._surveys.values()


    def results(self):
        return self._results


    def stations(self):
        return self._stations

    def observations(self):
        return self._obs_data
