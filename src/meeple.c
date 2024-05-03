#include <stdio.h>
#include <stdlib.h>
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

int add_meeple(struct meeple_t *meeple, struct move_t* move, struct super_board_t sboard, enum gamemode_t gt)
{
  if(gt == FINITE_MEEPLE){
    if (move->player_id == 1 && meeple->size2 == 7 ){
      	move->meeple = 14;
		return 0;
    }
    if (move->player_id == 0 && meeple->size1 == 7 ){
      move->meeple = 14;
	  return 0;
    }
  }
  
  if( gt == INFINITE_MEEPLE){
    if (meeple->capacity1 == meeple->size1 ){
      meeple->capacity1 *= 2;
      meeple->player1 = realloc(meeple->player1, sizeof(int)*meeple->capacity1);
    }

    if (meeple->capacity2 == meeple->size2 ){
      meeple->capacity2 *= 2;
      meeple->player2 = realloc(meeple->player2, sizeof(int)*meeple->capacity2);
    }
  }  
  
  
  for (int i= 0; i <12; i++){
    if(check_add_meeple(sboard, i, meeple)){
      if (move->player_id == 0 ){
	move->meeple = i;
	meeple->player1[meeple->size1] = ((sboard.size-1) * 13) + move->meeple ;
	meeple->size1 +=1;
	return 1;
      }

      if (move->player_id == 1 ){
	move->meeple = i;
	meeple->player2[meeple->size2] = ((sboard.size-1) * 13) + move->meeple ;
	meeple->size2 +=1;
	return 1;
      }
    }
  }
  	move->meeple = 14;
  	return 0;
  
}

int check_add_meeple( struct super_board_t sboard, enum conn_t indexVertex, struct meeple_t *meeple){
   
  igraph_vector_int_t components;
  igraph_vector_int_t csize;
  igraph_integer_t count = 0;

  igraph_vector_int_init(&csize, 0);
  igraph_vector_int_init(&components, 0);

  igraph_connected_components(&sboard.graph, &components, &csize, &count, IGRAPH_WEAK);

  igraph_integer_t nb_vertices = igraph_vector_int_size(&components);

  int *vertices = malloc(nb_vertices * sizeof(int));
  int size;
  //igraph_vector_int_print(&components);
  igraph_integer_t a= VECTOR(components)[(sboard.size-1) * 13 + indexVertex];
  size = vector_extract_component(components, a,vertices);

  for (int i=0; i<size; i++){
    for (int p1=0;p1 < meeple->size1 ; p1++){
      if(meeple->player1[p1] == vertices[i]){
	return 0;
      }
    }
    for (int p2=0; p2 < meeple->size2 ; p2++){
      if(meeple->player2[p2] == vertices[i]){
	return 0;
      }
    }
  }
  return 1;
}

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
