#include <ert/job_queue/environment_varlist.h>

#include <ert/util/test_util.h>

void test_varlist() {
  char * first  = "FIRST";  char * first_value = "TheFirstValue";
  char * second = "SECOND"; char * second_value = "TheSecondValue";

  env_varlist_type * list = env_varlist_alloc();
  env_varlist_add(list, first, first_value);
  env_varlist_add(list, second, second_value);
  int size = env_varlist_get_size(list);
  test_assert_int_equal(size, 2);

  char ** keylist = env_varlist_get_varlist(list);
 
  char * value_0 = env_varlist_get_value(list, keylist[0]);
  test_assert_string_equal(value_0, second_value);
  char * value_1 = env_varlist_get_value(list, keylist[1]);
  test_assert_string_equal(value_1, first_value);

  env_varlist_free(list);
  free(keylist);
  free(value_0);
  free(value_1);
}


int main() {
  test_varlist();
  return 0;
}
