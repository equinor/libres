#include <ert/job_queue/environment_varlist.h>


void test_varlist() {
  // test alloc
  // add pairs "FIRST" = "TheFirstValue", "SECOND" = "TheSecondValue"
  // get pairs and assert
  // free
  // run valgrind
  env_varlist_type * varlist = env_varlist_alloc();
  env_varlist_free(varlist);
}


int main() {
  test_varlist();
  return 0;
}
