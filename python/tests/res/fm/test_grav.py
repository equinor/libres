import unittest
import os
import os.path
import math
import datetime
import stat
from unittest import skipIf
import py_compile

from ecl import EclPhaseEnum, EclTypeEnum, EclDataType
from ecl.eclfile import EclFile, EclKW, FortIO, openFortIO
from res.fm import grav
from res.fm.grav import GravConfig, Grav, GravResult
from tests import ResTest
from ecl.util.test import TestAreaContext


class GravTest(ResTest):

    def compileScript(self , script_file):
        if os.path.isfile(script_file):
            if os.access( script_file , os.X_OK):
                py_compile.compile( script_file , doraise = True )
            else:
                raise OSError("The script file:%s was not executable" % script_file)
        else:
            raise IOError("The script file:%s was not found" % script_file)

    def test_script(self):

        # The package doc string contains an example script between
        # [script:start] and [script:end] (which has been indented
        # four spaces ...). This test will extract that script to a
        # file and make sure it can be loaded.

        doc_string = grav.__doc__
        with TestAreaContext("script"):
            with open("run_grav", "w") as f:
                on = False
                for line in doc_string.split("\n"):
                    if line.startswith("[script:end]"):
                        on = False

                    if on:
                        f.write("%s\n" % line[4:])

                    if line.startswith("[script:start]"):
                        on = True


            st = os.stat("run_grav")
            os.chmod("run_grav" , st.st_mode | stat.S_IEXEC )
            script = self.compileScript("run_grav" )



    def test_config(self):
        with self.assertRaises(IOError):
            conf = GravConfig( "/does/not/exist" , None)


        with TestAreaContext("test-config1"):
            with openFortIO("CASE.X0010", mode = FortIO.WRITE_MODE) as f:
                for kw in [EclKW(x , 1000 , EclDataType.ECL_FLOAT_TYPE ) for x in ["FIPOIL","FIPWAT","FIPGAS","RFIP" , "RPORV" , "PORV_MOD"]]:
                    kw.fwrite( f )

            rst_file1 = EclFile("CASE.X0010")
            rst_file2 = EclFile(self.createTestPath("Statoil/ECLIPSE/Gurbat/ECLIPSE.UNRST"))

            # To few items
            with open("grav.config","w") as f:
                f.write("STATION NAME 10 10")
            with self.assertRaises(SystemExit):
                conf = GravConfig( "grav.config"  , rst_file1)


            # Invalid format:
            with open("grav.config","w") as f:
                f.write("STATION NAME 10 10 X")
            with self.assertRaises(SystemExit):
                conf = GravConfig( "grav.config"  , rst_file1)


            with open("grav.config","w") as f:
                f.write("SURVEY  S1  2000-01-01  FIP \n")
                f.write("SURVEY  S2  2001-01-01  FIP \n")

            conf = GravConfig( "grav.config"  , rst_file2)
            surveys = conf.surveys( )
            self.assertEqual( len(surveys) , 2)
            s1 = surveys[0]
            self.assertEqual( s1.name , "S1" )
            self.assertEqual( s1.date , datetime.date( 2000 , 1 , 1 ))

            s2 = surveys[1]
            self.assertEqual( s2.name , "S2" )
            self.assertEqual( s2.date , datetime.date( 2001 , 1 , 1 ))


            # Invalid Type
            with open("true.txt" , "w") as f:
                f.write("N 0.25 0.25\n")

            # Invalid Survey name
            with open("grav.config","w") as f:
                f.write("STATION  N  1 1 1 \n")
                f.write("SURVEY  S1  2000-1-1 FIP \n")
                f.write("SURVEY  S2  2001-1-1 FIP \n")
                f.write("RESULT  res1 S1 SX true.txt\n")
            with self.assertRaises(KeyError):
                conf = GravConfig( "grav.config"  , rst_file2)


            # Invalid Survey name
            with open("grav.config","w") as f:
                f.write("STATION  N  1 1 1 \n")
                f.write("SURVEY  S1  2000-1-1 FIP \n")
                f.write("SURVEY  S2  2001-1-1 FIP \n")
                f.write("RESULT  res2 SX S2  true.txt\n")
            with self.assertRaises(KeyError):
                conf = GravConfig( "grav.config" , rst_file2)


            # Missing true File:
            with open("grav.config","w") as f:
                f.write("STATION  N  1 1 1 \n")
                f.write("SURVEY  S1  2000-1-1 FIP \n")
                f.write("SURVEY  S2  2001-1-1 FIP \n")
                f.write("RESULT  res2 S1 S2  no-such-file\n")
            with self.assertRaises(SystemExit):
                conf = GravConfig( "grav.config" , rst_file2)


            # True - missing true argument; that is OK
            with open("grav.config","w") as f:
                f.write("STATION  N  1 1 1 \n")
                f.write("SURVEY  S1  2000-1-1 FIP \n")
                f.write("SURVEY  S2  2001-1-1 FIP \n")
                f.write("RESULT  res2 S1 S2\n")
            conf = GravConfig( "grav.config" , rst_file2)



            # True File invalid format
            with open("true.txt" , "w") as f:
                f.write("N 0.25 X\n")
            with open("grav.config","w") as f:
                f.write("STATION  N  1 1 1 \n")
                f.write("SURVEY  S1  2000-1-1 FIP \n")
                f.write("SURVEY  S2  2001-1-1 FIP \n")
                f.write("RESULT  res2 S1 S2  true.txt\n")
            with self.assertRaises(ValueError):
                conf = GravConfig( "grav.config" , rst_file2)

            with open("true.txt" , "w") as f:
                f.write("N 1 2 \n")
            with open("grav.config","w") as f:
                f.write("STATION  N  1 1 1 \n")
                f.write("SURVEY  S1  2000-1-1 FIP \n")
                f.write("SURVEY  S2  2001-1-1 FIP \n")
                f.write("RESULT  res2 S1 S2  true.txt\n")
            conf = GravConfig( "grav.config" , rst_file2)

            data = conf.observations()["res2"]
            self.assertEqual( data["N"] , (1,2))
            with self.assertRaises(KeyError):
                _ = data["NO-not-this-key"]

            self.assertEqual( len(data) , 1 )



            # Invalid Type
            with open("true.txt" , "w") as f:
                f.write("N 0.25 0.25\n")

            with open("grav.config","w") as f:
                f.write("STATION  N  1 1 1 \n")
                f.write("SURVEY  S1  2000-1-1 FIP \n")
                f.write("SURVEY  S2  2001-1-1 FIPX \n")
                f.write("RESULT  res2 S1 S2  true.txt\n")

            with self.assertRaises(SystemExit):
                conf = GravConfig( "grav.config"  , rst_file2)


            # Date not in restart file
            with open("grav.config","w") as f:
                f.write("SURVEY  S1  2000-1-15 FIP \n")
            with self.assertRaises(ValueError):
                conf = GravConfig( "grav.config"  , rst_file2)


            # Valid
            with open("grav.config","w") as f:
                f.write("OIL_DEN 1\n")
                f.write("WAT_DEN 2\n")
                f.write("GAS_DEN 3\n")

            conf = GravConfig( "grav.config"  , rst_file2)
            self.assertEqual( conf.stdDensity() , {EclPhaseEnum.ECL_GAS_PHASE: 3,
                                                   EclPhaseEnum.ECL_OIL_PHASE: 1,
                                                   EclPhaseEnum.ECL_WATER_PHASE: 2})



    def test_grav(self):
        rst_file = EclFile(self.createTestPath("Statoil/ECLIPSE/Gurbat/ECLIPSE.UNRST"))
        case = self.createTestPath("Statoil/ECLIPSE/Gurbat/ECLIPSE.DATA")
        with TestAreaContext("test-grav"):

            with open("true1.txt" , "w") as f:
                f.write("N1 0.25 0.25\n")

            with open("true2.txt" , "w") as f:
                f.write("N2 0.25 0.25\n")

            with open("grav.config","w") as f:
                f.write("OIL_DEN 1\n")
                f.write("WAT_DEN 2\n")
                f.write("GAS_DEN 3\n")
                f.write("SURVEY  S1  2000-1-1 FIP \n")
                f.write("SURVEY  S2  2000-1-1 FIP \n")
                f.write("RESULT  res1 S1  S2 true1.txt\n")
                f.write("RESULT  res2 S1  S2 true2.txt\n")
                f.write("STATION  N1  1 1 1 \n")
                f.write("STATION  N2  2 2 2 \n")

            with self.assertRaises(IOError):
                grav = Grav( case , "does_not_exist" )


            with self.assertRaises(IOError):
                grav = Grav( "does/not/exist/CASE" , "grav.config" )


            grav = Grav(case , "grav.config" )
            grav_result = grav.eval( )

            with self.assertRaises(KeyError):
                _ = grav_result["NO-NOT-THIS_KEY"]

            res1 = grav_result["res1"]
            res2 = grav_result["res2"]

            obs1 = grav.config().observations()["res1"]
            ch2 = obs1.chi2( res1 )


    def test_result(self):
        res = GravResult()
        out = str(res)

        with self.assertRaises(KeyError):
            _ = res["TEST1"]


if __name__ == "__main__":
    unittest.main()
