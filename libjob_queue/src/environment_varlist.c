/*
   Copyright (C) 2017  Statoil ASA, Norway. 
    
   The file 'environment_varlist.c' is part of ERT - Ensemble based Reservoir Tool. 
    
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

#include <ert/job_queue/environment_varlist.h>

#include <ert/util/util_env.h>
#include <ert/util/hash.h>

#define ENV_VAR_KEY_STRING "global_environment"

struct env_varlist_struct {
  hash_type * varlist;
};

env_varlist_type * env_varlist_alloc() {
  env_varlist_type * list = util_malloc( sizeof * list );
  list->varlist = hash_alloc();
  return list;
}

void env_varlist_update_path(env_varlist_type * list, const char * path_var, const char * new_path)  {
  hash_insert_string( list->varlist, path_var , util_update_path_var( path_var , new_path , false));
}

void env_varlist_setenv(env_varlist_type * list, const char * key, const char * value) {
  char * _value = util_interp_setenv(key, value);
  hash_insert_string(list->varlist, key, _value);
}

void env_varlist_json_fprintf(env_varlist_type * list, FILE * stream) {
  int size = hash_get_size(list->varlist);
  fprintf(stream, "\"%s\" : {", ENV_VAR_KEY_STRING);
  stringlist_type * stringlist = hash_alloc_stringlist(list->varlist);
  int i_max = size - 1;
  for (int i = 0; i < size; i++) {
    char * key = stringlist_iget(stringlist, i);
    fprintf(stream, "\"%s\" : \"%s\"", key, (char*)hash_get(list->varlist, key)   );
    if (i < i_max)
      fprintf(stream, ", ");
  }
  stringlist_free(stringlist);
  fprintf(stream, "}");
}

void env_varlist_free(env_varlist_type * list) {
  hash_free(list->varlist);
  free(list);
}
