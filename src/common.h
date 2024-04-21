#ifndef __COMMON_H__
#define __COMMON_H__

/** Represents a pair of integers */
struct int_pair_t {
  int a;
  int b;
};


/** Checks if an int_pair is already in the provided list.
 * @param list A pointer to the list of int_pair
 * @param size The size of the list
 * @param a The int that should be in the "a" field of the int_pair
 * @param b The int that should be in the "b" field of the int_pair
 * @return 1 if pair is in list, 0 if it is not
 */
int is_int_pair_in_list(struct int_pair_t *list, int size, int a, int b);

/** Checks if an int is already in the provided list.
 * @param list A pointer to the list of int
 * @param size The size of the list
 * @param a The int that we are searching for
 * @return 1 if a is in list, 0 if it is not
 */
int is_int_in_list(int *list, int size, int a);

#endif // __COMMON_H__
