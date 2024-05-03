#include "../common.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void test_is_int_pair_in_list()
{
  printf(" • %s", __func__);

  struct int_pair_t l[2];
  l[0].a = 1;
  l[0].b = 2;

  l[1].a = 3;
  l[1].b = 4;

  assert(is_int_pair_in_list(l, 2, 1, 2) == 1);
  assert(is_int_pair_in_list(l, 2, 3, 4) == 1);
  
  assert(is_int_pair_in_list(l, 2, 2, 1) == 0);
  assert(is_int_pair_in_list(l, 2, 4, 3) == 0);

  assert(is_int_pair_in_list(l, 2, 12, 4) == 0);
  assert(is_int_pair_in_list(l, 2, 1, 4) == 0);
  assert(is_int_pair_in_list(l, 2, 3, 2) == 0);

  assert(is_int_pair_in_list(l, 1, 3, 4) == 0);
  
  printf("\t\e[1;102m SUCCESS \e[0m\n");
}

void test_is_int_in_list()
{
  printf(" • %s", __func__);

  int l[2];
  l[0] = 1;
  l[1] = 3;

  assert(is_int_in_list(l, 2, 1) == 1);
  assert(is_int_in_list(l, 2, 3) == 1);
  
  assert(is_int_in_list(l, 2, 2) == 0);
  assert(is_int_in_list(l, 2, 4) == 0);

  assert(is_int_in_list(l, 1, 3) == 0);
  
  printf("\t\e[1;102m SUCCESS \e[0m\n");
}

int common_tests()
{
  printf("\n\e[30;47mFile %s\e[0m\n", __FILE__);
  
  test_is_int_pair_in_list();
  test_is_int_in_list();
	
  return 0;
}
