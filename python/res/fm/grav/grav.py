from collections import OrderedDict
import os.path

from ecl.eclfile import EclFile
from ecl.grid import EclGrid
from ecl.gravimetry import EclGrav

from . import GravConfig, GravResult

class Grav(object):

    def __init__(self , case, config_file):
        """Master object used to manage a set of gravimetric surveys.

        The case argument should be the path to an existing Eclipse
        simulation, optionally with a leading path and an arbitrary
        extension. The construct will look for the corresponding
        UNRST, INIT and EGRID files.

        The config_file argument should be the name of a configuration
        file for the gravimetric surveys. For details of the
        configuration file see:

             pydoc ert_statoil.grav

        """
        case = os.path.splitext( case )[0]

        unrst_file = EclFile( ".".join( (case, "UNRST")))
        self._config = GravConfig( config_file , unrst_file )

        init_file = EclFile( ".".join( (case, "INIT")))
        grid = EclGrid( ".".join( (case, "EGRID")))
        self.ecl_grav = EclGrav( grid , init_file )

        for phase,density in self._config.stdDensity().items():
            self.ecl_grav.new_std_density( phase , density )
        
        for survey in self._config.surveys():
            rst_view = unrst_file.restartView( sim_time = survey.date )
            self.ecl_grav.addSurvey( survey.name , rst_view , survey.type_string )
            

            
    def eval(self):
        """Evaluate all the gravimetric results.

        Will iterate over all the result sets which have been
        configured in the configuration file. The returned value is
        an instance of GravResult, see the documentation of this class
        for how to inspect the results more detailed.

        """
        results = GravResult()
        for res in self._config.results().values():
            sim_data = self.evalOutput( res )
            results[res.name] = sim_data
            
        return results
            
            

    def evalOutput(self, res):
        """Evaluate the results from one 4D survey.

        The results is a dictionary where the station names are used
        as keys, and the gravitational change is the value. 
        """
        results = OrderedDict()
        for station in self._config.stations().values():
            dG = self.evalPoint( res.base, res.monitor , station.x , station.y , station.z )
            results[station.name] = dG

        return results

    
            
    def evalPoint(self , base, monitor , x , y , z):
        """Evaluate gravimetric response in a point.

        The evalPoint() method will evaluate the gravimetric response
        in the point x,y,z based on the two surveys named @base and
        @monitor, the @base and @monitor surveys must be configured in
        the configuration file.

        Example:

             grav = Grav( "/PATH/TO/CASE" , config_file )
             ...
             dG = evalPoint( "SUMMER-2007" , "SUMMER-2010" , 1000, 625424.0 , 972)  
        
        """
        
        dG = self.ecl_grav.eval( base , monitor , (x,y,z) , region = None)
        return dG


    def config(self):

        """Will return the GravConfig instance.
        
        The resulting GravConfig object can be queried for stations,
        surveys and so on.
        """
        return self._config
