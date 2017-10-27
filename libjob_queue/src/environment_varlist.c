#include <ert/job_queue/environment_varlist.h>

#include <ert/util/hash.h>

struct env_varlist_struct {
  hash_type * varlist;
};

env_varlist_type * env_varlist_alloc() {
  env_varlist_type * list = util_malloc( sizeof * list );
  list->varlist = hash_alloc();
  return list;
}

void env_varlist_add(env_varlist_type * list, char * var_name, char * var_value) {
  hash_insert_string(list->varlist, var_name, var_value);
}

int env_varlist_get_size(env_varlist_type * list) {
  return hash_get_size(list->varlist);
}

char ** env_varlist_alloc_varlist(env_varlist_type * list) {
  return hash_alloc_keylist(list->varlist);
}

char * env_varlist_get_value(env_varlist_type * list, char * var) {
  return hash_get(list->varlist, var);
}

void env_varlist_free(env_varlist_type * list) {
  hash_free(list->varlist);
  free(list);
}
