/*
   Copyright (C) 2017  Statoil ASA, Norway.

   The file 'ext_param_config.c' is part of ERT - Ensemble based Reservoir Tool.

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

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include <string>
#include <vector>
#include <algorithm>

#include <ert/util/util.h>
#include <ert/util/type_macros.h>

#include <ert/enkf/enkf_macros.hpp>
#include <ert/enkf/ext_param_config.hpp>

#define EXT_PARAM_CONFIG_ID 97124451
struct ext_param_config_struct {
  UTIL_TYPE_ID_DECLARATION;
  std::string key;
  std::vector<std::string> keys;
  std::vector<std::vector<std::string> > suffixes;
};

UTIL_SAFE_CAST_FUNCTION(ext_param_config, EXT_PARAM_CONFIG_ID)
UTIL_SAFE_CAST_FUNCTION_CONST(ext_param_config, EXT_PARAM_CONFIG_ID)

void ext_param_config_free( ext_param_config_type * config ) {
  free( config );
}

int ext_param_config_get_data_size( const ext_param_config_type * config ) {
  return config->keys.size();
}


const char* ext_param_config_iget_key( const ext_param_config_type * config , int index) {
  return config->keys[index].data();
}

int ext_param_config_get_key_index( const ext_param_config_type * config , const char * key) {
  const auto it = std::find(config->keys.begin(), config->keys.end(), key);
  return it == config->keys.end() ?
            -1 : 
            std::distance(config->keys.begin(), it);
}

bool ext_param_config_has_key( const ext_param_config_type * config , const char * key) {
  return std::find(config->keys.begin(), config->keys.end(), key) != config->keys.end();
}


ext_param_config_type * ext_param_config_alloc( const char * key, const stringlist_type * keys) {
  if (stringlist_get_size( keys ) == 0)
    return NULL;

  if (!stringlist_unique( keys ))
    return NULL;

  ext_param_config_type * config = new ext_param_config_type();
  UTIL_TYPE_ID_INIT( config , EXT_PARAM_CONFIG_ID);
  config->key = key;

  for(int i=0; i<stringlist_get_size(keys); i++) {
    config->keys.push_back(stringlist_iget(keys, i));
  }
  return config;
}


int ext_param_config_get_suffix_count( const ext_param_config_type * config, int key_id) {  
  return config->suffixes[key_id].size();
}

const char* ext_param_config_iget_suffix( const ext_param_config_type * config, int key_id, int suffix_id) {  
  return config->suffixes[key_id][suffix_id].data();
}

int ext_param_config_get_suffix_index( const ext_param_config_type * config, int key_id, const char * suffix) {
  const auto it = std::find(config->suffixes[key_id].begin(), config->suffixes[key_id].end(), suffix);
  return it == config->suffixes[key_id].end() ?
            -1 : 
            std::distance(config->suffixes[key_id].begin(), it);
}

VOID_FREE(ext_param_config)
VOID_GET_DATA_SIZE(ext_param)
