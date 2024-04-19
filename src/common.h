#ifndef __COMMON_H__
#define __COMMON_H__

struct int_pair_t {
  int a;
  int b;
};

int is_int_pair_in_list(struct int_pair_t *haystack, int size, int a, int b);

#endif // __COMMON_H__
