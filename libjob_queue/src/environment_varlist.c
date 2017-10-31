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

void env_varlist_add(env_varlist_type * list, const char * var_name, const char * var_value) {
  hash_insert_string(list->varlist, var_name, var_value);
}

const char * env_varlist_add_pathvar(env_varlist_type * list, const char * var, const char * value) {
  char * _value = NULL;
  if (hash_has_key(list->varlist, var))
    _value = hash_get(list->varlist, var);
  env_varlist_add(list, var, value);
  return _value;
}

int env_varlist_get_size(const env_varlist_type * list) {
  return hash_get_size(list->varlist);
}

char ** env_varlist_alloc_varlist(const env_varlist_type * list) {
  return hash_alloc_keylist(list->varlist);
}

const char * env_varlist_get_value(const env_varlist_type * list, const char * var) {
  return hash_get(list->varlist, var);
}


void env_varlist_setenv(env_varlist_type * list, const char * key, const char * value) {
  util_interp_setenv(key, value);
  hash_insert_string(list->varlist, key, value);
}

void env_varlist_fprintf_json(env_varlist_type * list) {
  FILE * stream    = util_fopen("jobs.json", "w");
  fprintf(stream, "{\n");
  fprintf(stream, "   ");
  env_varlist_fprintf(list, stream);
  fprintf(stream, "\n}");
  fclose(stream);
}

void env_varlist_fprintf(env_varlist_type * list, FILE * stream) {
  int size = hash_get_size(list->varlist);
  fprintf(stream, "\"%s\" : {", ENV_VAR_KEY_STRING);
  char ** keylist = hash_alloc_keylist(list->varlist);
  int i_max = size - 1;
  for (int i = 0; i < size; i++) {
    fprintf(stream, "\"%s\" : \"%s\"", keylist[i], (char*)hash_get(list->varlist, keylist[i])   );
    if (i < i_max)
      fprintf(stream, ", ");
    free(keylist[i]);
  }
  free(keylist);
  fprintf(stream, "}");
}

void env_varlist_free(env_varlist_type * list) {
  hash_free(list->varlist);
  free(list);
}
