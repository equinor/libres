"""Gravity code

The package ert_statoil.grav contains classes which can be used to
simplify calculations of simulated gravimetric response, and comparing
simulated response with observed response. The package consists of
several classes, but the most important class is the Grav class, a
simple script using this functionality can be implemented only based
on the Grav class.


When instantiating a Grav instance you need to supply a configuration
file. The configuration file has the following keywords:

 OIL_DEN: Used to give the density of the oil phase in the model.
 
 GAS_DEN: Used to give the gas density.

 WAT_DEN: Used to give the water density.

 STATION: This is how you input the location of the gravimetric
     measurememt stations. The STATION argument expects four
     arguments, the name of the station and the position given as
     x.y.z.

 SURVEY: This keyword is used to define a gravimetric survey. A survey
     is defined with a unique name, a date and a string telling which
     method should be used to calculate the mass change. The date
     should be given as the string YYYY-MM-DD and the restart file
     should have a report exactly at this date, the allowed method
     strings are: PORV_MOD, RFIP, RPORV and FIP.

 RESULT: This keyword configures a 4D response which we wish to
     calculate. The keyword arguments are a name to assign to this
     result, the name of base and monitor surveys and a filename with
     observed values. The observed values are optional and can be
     omitted.

An example config file could look like this:

-- The standard densities of the three phases.
OIL_DEN  850
GAS_DEN  0.778
WAT_DEN  1045


-- Define three stations named '1', 'B' and 'C3' where we want to
-- calculate the gravimetric response.
STATION  1   541028    6709902   297.0286944
STATION  B   543765    6707228	 295.4384952
STATION  C3  537510    6714290	 300.5589352


-- Define three surveys named S1, S2 and S3. The two first surveys are
-- based on the FIP method, and the last survey is based on the RPORV
-- calculation:
SURVEY   S1   2005-10-10   FIP
SURVEY   S2   2008-06-01   FIP
SURVEY   S3   2012-06-10   RPORV


-- Define two simulated 4D responses which we wish to calculate, S2 -
-- S1 and S3 - S1 respectively. 
RESULT res1   S1   S2   observed_s1_s2.txt
RESULT res2   S1   S3

For the S2 - S1 response we also have a file with observed values
named 'observed_s1_s2.txt' - we have no such file for the S3 - S1
response. The file with observed values should be formatted as a
simple text file where each line consists of the name of the station,
the observed value and the uncertainty. I.e. for the stations listed
above a results file could look like:

1    10.0  1.56
B    12.0  2.67
C3   16.7  4.88


The purpose of the grav package is that it should be easy to create
small end-user applications to calculate the gravimetric response. An
example of such a script could be:

[script:start]   
    #!/usr/bin/env python
    import sys
    from ert_statoil.grav import Grav


    # Check that we have exactly two commandline arguments.
    if len(sys.argv) != 3:
        sys.exit("Need two arguments: case config_file")


    case = sys.argv[1]
    config_file = sys.argv[2]


    # Create the Grav master object based on the input eclipse case
    # and a configuration file.
    grav = Grav( case , config_file )

    
    # Evaluate all the results requested in the configuration file, the
    # results are returned as a GravResult instance.
    results = grav.eval( )


    # Fetch the GravConfig object from the grav instance, this can then
    # be used for further queries of properties.
    config = grav.config()

    for key in results.keys():
        values = results[key]
        print "-----------------------------------------------------------------"
        res_config = config.results()[key]
        print "Result: %s  %s - %s" % (key , res_config.base , res_config.monitor)
        for st in values.keys():
            print "%8s : %g" % (st , values[st])

        obs_map = config.observations()
        if key in obs_map:
            obs = obs_map[key]
            print "Total misfit: %s" % obs.chi2( values )
[script:end]

"""

from .grav_config import GravConfig, Survey
from .grav_result import GravResult
from .grav import Grav

