#  Copyright (C) 2017  Statoil ASA, Norway.
#
#  The file 'test_enkf_transfer_env.py' is part of ERT - Ensemble based Reservoir Tool.
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
import subprocess

from ecl.test import ExtendedTestCase, TestAreaContext
from ecl.util import BoolVector

from res.enkf import (EnsembleConfig, AnalysisConfig, ModelConfig, SiteConfig,
                      EclConfig, PlotSettings, EnkfObs, ErtTemplates, EnkfFs,
                      EnKFState, EnkfVarType, ObsVector, RunArg, ResConfig)
from res.enkf.config import EnkfConfigNode
from res.enkf.enkf_main import EnKFMain
from res.enkf.ert_run_context import ErtRunContext
from res.enkf.enums import (EnkfObservationImplementationType, LoadFailTypeEnum,
                            EnkfInitModeEnum, ErtImplType, RealizationStateEnum,
                            EnkfRunType, EnkfFieldFileFormatEnum,
                            EnkfTruncationType, ActiveMode)

from res.enkf.observations.summary_observation import SummaryObservation
from res.test import ErtTestContext


class EnKFTestTransferEnv(ExtendedTestCase):

  def setUp(self):
    pass

  def test_transfer_var(self):
    with TestAreaContext('enkf_test_transfer_env') as work_area:
      base_path = os.getcwd()
      source_path = '/private/stefos/ert/libres/test-data/local/transfer_env'
      work_area.copy_directory(source_path)
      subprocess.call(["ls", "-l"])
   
      

      #assert(os.path.isdir(  dir_ert  )  )
      #assert(os.path.isdir(  os.path.join(base_path, source_path, 'jobs')  )  )
      #file_ert = os.path.join(base_path, source_path, 'transfer_env.ert')
      #assert(  os.path.isfile(file_ert)  )
    
      #print(base_path)     
      #print(dir_ert)
      #print(file_ert)

      #with ErtTestContext( "transfer_env", model_config = file_ert, store_area = True) as ctx:
        #ert = ctx.getErt( )
        #fs_manager = ert.getEnkfFsManager()
        #result_fs = fs_manager.getCurrentFileSystem( )

        #model_config = ert.getModelConfig( )
        #runpath_fmt = model_config.getRunpathFormat( )
        #subst_list = ert.getDataKW( )
        #itr = 0
        #mask = BoolVector( default_value = True, initial_size = 1 )
        #run_context = ErtRunContext.ensemble_experiment( result_fs, mask, runpath_fmt, subst_list, itr)
        #ert.getEnkfSimulationRunner().createRunPath( run_context )
        #job_queue = ert.get_queue_config().create_job_queue()
        #num = ert.getEnkfSimulationRunner().runEnsembleExperiment(job_queue, run_context)
        #assert(num == 1)
        #assert(os.path.isdir(     os.path.join(base_path, source_path, 'storage')       ) )
      
      
      
      
