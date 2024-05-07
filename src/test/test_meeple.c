#include <stdio.h>
#include <assert>
#include "meeple.h"

void test_init_meeple()
{
  int capacity = 7;

  struct meeple_t mee = init_meeple(capacity);

  assert(mee.capacity1 == 7);
  assert(mee.capacity2 == 7);
  assert(mee.size1 == 0);
  assert(mee.size2 == 0);
  

  free_meeple(mee);
}

void test_get_bck_meeple()
{
  int capacity = 7;
  struct meeple_t mee = init_meeple(capacity);
  mee.size1 == 1;
  mee.player1[1] == 1;

  int vertices[7]= {1} ;
  int size = 1;

  get_back_meeple(&mee, vertices, size);

  assert(mee.size1 == 0);

  free_meeple(mee);
}

void test_copy_meeple()
{
  int capacity = 7;

  struct meeple_t mee = init_meeple(capacity);

  mee.size1 = 2;
  mee.player1[0] = 1;
  mee.player1[1] = 3;

  mee.size2 = 1;
  mee.player2[0] = 1;

  struct meeple_t copy_mee = copy_meeple(mee);

  assert( mee.size1 == copy_mee.size1);
  assert( mee.size2 == copy_mee.size2);
  assert( mee.capacity1 == copy_mee.capacity1);
  assert( mee.capacity2 == copy_mee.capacity2);
  assert( mee.player1[0] == copy_mee.player1[0]);
  assert( mee.player1[1] == copy_mee.player1[1]);
  assert( mee.player2[0] == copy_mee.player2[0]);

  free_meeple(mee);
  free_meeple(copy_mee);
}
