#include "common.h"

int is_int_pair_in_list(struct int_pair_t *list, int size, int a, int b) {
  for (int i = 0; i < size; i++) {
    if (list[i].a == a && list[i].b == b)
      return 1;
  }
  return 0;
}

int is_int_in_list(int *list, int size, int a) {
  for (int i = 0; i < size; i++) {
    if (list[i] == a)
      return 1;
  }
  return 0;  
}
