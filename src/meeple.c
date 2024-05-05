#include <stdio.h>
#include <stdlib.h>
#include "meeple.h"

struct meeple_t init_meeple(int capacity)
{
  struct meeple_t meeple={};
  meeple.capacity1 = capacity;
  meeple.capacity2 = capacity;
  meeple.size1 = 0;
  meeple.size2 = 0;
  meeple.player1 = malloc(sizeof(int)*meeple.capacity1);
  meeple.player2 = malloc(sizeof(int)*meeple.capacity2);

  return meeple;
};

// SERVER VERSION


void get_back_meeple(struct meeple_t *meeple, int *vertices, int size)
{
  for (int i=0; i<size; i++){
    for (int p1=0;p1 < meeple->size1 ; p1++){
      if(meeple->player1[p1] == vertices[i]){
	meeple->size1 -=1;
      }
    }
    for (int p2=0; p2 < meeple->size2 ; p2++){
      if(meeple->player2[p2] == vertices[i]){
	meeple->size2 -=1;
      }
    }
  }
}

void free_meeple(struct meeple_t meeple)
{
  free(meeple.player1);
  free(meeple.player2);
}

struct meeple_t copy_meeple(struct meeple_t meeple){
	struct meeple_t copy={};
	copy.size1=meeple.size1;
	copy.size2=meeple.size2;
	copy.capacity1=meeple.capacity1;
	copy.capacity2=meeple.capacity2;
  	copy.player1 = malloc(sizeof(int)*copy.capacity1);
	copy.player2 = malloc(sizeof(int)*copy.capacity2);

	for (int i=0; meeple.size1;i++){ 
		copy.player1[i]=meeple.player1[i];
	}
	for (int i=0; meeple.size2;i++){
		copy.player2[i]=meeple.player2[i];
	}
	return copy;

}	