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

void env_varlist_free(env_varlist_type * list) {
  hash_free(list->varlist);
  free(list);
}
