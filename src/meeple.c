#include <stdio.h>
#include <stdlib.h>
#include "move.h"
#include "meeple.h"

struct meeple_t init_meeple(int capacity)
{
  struct meeple_t meeple;
  meeple.capacity1 = capacity;
  meeple.capacity2 = capacity;
  meeple.size1 = 0;
  meeple.size2 = 0;
  meeple.player1 = malloc(sizeof(int)*meeple.capacity1);
  meeple.player2 = malloc(sizeof(int)*meeple.capacity2);

  return meeple;
};

void add_meeple(struct meeple_t *meeple, struct move_t move)
{
  if (meeple->capacity1 == meeple->size1 ){
    meeple->capacity1 *= 2;
    meeple->player1 = realloc(meeple->player1, sizeof(int)*meeple->capacity1);
  }

  if (meeple->capacity2 == meeple->size2 ){
    meeple->capacity2 *= 2;
    meeple->player2 = realloc(meeple->player2, sizeof(int)*meeple->capacity2);
  }

  if (move.player_id == 1){
    meeple->player1[meeple->size1] = move.meeple ;
    meeple->size1 +=1;
    }

   if (move.player_id == 2){
    meeple->player2[meeple->size2] = move.meeple ;
    meeple->size2 +=1;
    }
  
}

void free_meeple(struct meeple_t meeple)
{
  free(meeple.player1);
  free(meeple.player2);
}
