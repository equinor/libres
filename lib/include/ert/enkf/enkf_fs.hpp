/*
   Copyright (C) 2011  Statoil ASA, Norway.

   The file 'enkf_fs.h' is part of ERT - Ensemble based Reservoir Tool.

   ERT is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   ERT is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.

   See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html>
   for more details.
*/

#ifndef ERT_ENKF_FS_H
#define ERT_ENKF_FS_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdbool.h>

#include <ert/util/stringlist.hpp>
#include <ert/util/type_macros.hpp>
#include <ert/util/buffer.hpp>
#include <ert/util/stringlist.hpp>

#include <ert/enkf/fs_driver.hpp>
#include <ert/enkf/enkf_types.hpp>
#include <ert/enkf/fs_types.hpp>
#include <ert/enkf/enkf_fs_type.hpp>
#include <ert/enkf/time_map.hpp>
#include <ert/enkf/cases_config.hpp>
#include <ert/enkf/state_map.hpp>
#include <ert/enkf/misfit_ensemble_typedef.hpp>
#include <ert/enkf/summary_key_set.hpp>
#include <ert/enkf/custom_kw_config_set.hpp>

  const      char * enkf_fs_get_mount_point( const enkf_fs_type * fs );
  const      char * enkf_fs_get_root_path( const enkf_fs_type * fs );
  const      char * enkf_fs_get_case_name( const enkf_fs_type * fs );
  bool              enkf_fs_is_read_only(const enkf_fs_type * fs);
  void              enkf_fs_fsync( enkf_fs_type * fs );
  void              enkf_fs_add_index_node(enkf_fs_type *  , int , int , const char * , enkf_var_type, ert_impl_type);

  enkf_fs_type    * enkf_fs_get_ref( enkf_fs_type * fs );
  int               enkf_fs_decref( enkf_fs_type * fs );
  int               enkf_fs_incref( enkf_fs_type * fs );
  int               enkf_fs_get_refcount( const enkf_fs_type * fs );
  enkf_fs_type    * enkf_fs_mount( const char * path );
  bool              enkf_fs_update_disk_version(const char * mount_point , int src_version , int target_version);
  int               enkf_fs_disk_version(const char * mount_point );
  int               enkf_fs_get_version104( const char * path );
  void              enkf_fs_fwrite_node(enkf_fs_type * enkf_fs , buffer_type * buffer , const char * node_key, enkf_var_type var_type,
                                        int report_step , int iens);

  void              enkf_fs_fwrite_vector(enkf_fs_type * enkf_fs ,
                                          buffer_type * buffer ,
                                          const char * node_key,
                                          enkf_var_type var_type,
                                          int iens);

  bool              enkf_fs_exists( const char * mount_point );

  void              enkf_fs_fread_node(enkf_fs_type * enkf_fs , buffer_type * buffer ,
                                       const char * node_key , enkf_var_type var_type ,
                                       int report_step , int iens);

  void              enkf_fs_fread_vector(enkf_fs_type * enkf_fs , buffer_type * buffer ,
                                         const char * node_key ,
                                         enkf_var_type var_type ,
                                         int iens);


  bool              enkf_fs_has_vector(enkf_fs_type * enkf_fs , const char * node_key , enkf_var_type var_type , int iens);
  bool              enkf_fs_has_node(enkf_fs_type * enkf_fs , const char * node_key , enkf_var_type var_type , int report_step , int iens);

  void              enkf_fs_debug_fprintf( const enkf_fs_type * fs);

  enkf_fs_type *    enkf_fs_create_fs( const char * mount_point , fs_driver_impl driver_id , void * arg, bool mount);

  char             * enkf_fs_alloc_case_filename( const enkf_fs_type * fs , const char * input_name);
  char             * enkf_fs_alloc_case_member_filename( const enkf_fs_type * fs , int iens , const char * input_name);
  char             * enkf_fs_alloc_case_tstep_filename( const enkf_fs_type * fs , int tstep , const char * input_name);
  char             * enkf_fs_alloc_case_tstep_member_filename( const enkf_fs_type * fs , int tstep , int iens , const char * input_name);

  FILE             * enkf_fs_open_case_tstep_member_file( const enkf_fs_type * fs , const char * input_name , int tstep , int iens , const char * mode);
  FILE             * enkf_fs_open_case_file( const enkf_fs_type * fs , const char * input_name , const char * mode);
  FILE             * enkf_fs_open_case_tstep_file( const enkf_fs_type * fs , const char * input_name , int tstep , const char * mode);
  FILE             * enkf_fs_open_case_member_file( const enkf_fs_type * fs , const char * input_name , int iens , const char * mode);

  FILE             * enkf_fs_open_excase_tstep_member_file( const enkf_fs_type * fs , const char * input_name , int tstep , int iens);
  FILE             * enkf_fs_open_excase_file( const enkf_fs_type * fs , const char * input_name);
  FILE             * enkf_fs_open_excase_tstep_file( const enkf_fs_type * fs , const char * input_name , int tstep );
  FILE             * enkf_fs_open_excase_member_file( const enkf_fs_type * fs , const char * input_name , int iens );

  time_map_type             * enkf_fs_alloc_readonly_time_map( const char * mount_point );
  state_map_type            * enkf_fs_alloc_readonly_state_map( const char * mount_point );
  summary_key_set_type      * enkf_fs_alloc_readonly_summary_key_set( const char * mount_point );
  state_map_type            * enkf_fs_get_state_map( const enkf_fs_type * fs );
  time_map_type             * enkf_fs_get_time_map( const enkf_fs_type * fs );
  cases_config_type         * enkf_fs_get_cases_config( const enkf_fs_type * fs);
  misfit_ensemble_type      * enkf_fs_get_misfit_ensemble( const enkf_fs_type * fs );
  summary_key_set_type      * enkf_fs_get_summary_key_set( const enkf_fs_type * fs );
  custom_kw_config_set_type * enkf_fs_get_custom_kw_config_set( const enkf_fs_type * fs );

  void             enkf_fs_increase_run_count(enkf_fs_type * fs);
  void             enkf_fs_decrease_run_count(enkf_fs_type * fs);
  bool             enkf_fs_is_running(const enkf_fs_type * fs);


  UTIL_SAFE_CAST_HEADER( enkf_fs );
  UTIL_IS_INSTANCE_HEADER( enkf_fs );


#ifdef __cplusplus
}
#endif
#endif
