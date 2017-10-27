/*
   Copyright (C) 2017  Statoil ASA, Norway. 
    
   The file 'environment_varlist_test.c' is part of ERT - Ensemble based Reservoir Tool. 
    
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

#include <ert/util/test_util.h>

void test_varlist() {
  char * first = util_alloc_string_copy("FIRST");
  char * second = util_alloc_string_copy("SECOND");
  char * first_value = util_alloc_string_copy("TheFirstValue");
  char * second_value = util_alloc_string_copy("TheSecondValue");

  env_varlist_type * list = env_varlist_alloc();
  env_varlist_add(list, first, first_value);
  env_varlist_add(list, second, second_value);
  int size = env_varlist_get_size(list);
  test_assert_int_equal(size, 2);

  char ** keylist = env_varlist_alloc_varlist(list);
 
  char * value_0 = env_varlist_get_value(list, keylist[0]);
  test_assert_string_equal(value_0, second_value);
  char * value_1 = env_varlist_get_value(list, keylist[1]);
  test_assert_string_equal(value_1, first_value);

  env_varlist_free(list);
  free(first);  free(first_value);
  free(second); free(second_value);
  free(keylist[0]);
  free(keylist[1]);
  free(keylist);
}


int main() {
  test_varlist();
  return 0;
}
