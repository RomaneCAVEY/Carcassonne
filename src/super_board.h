#ifndef __SUPER__BOARD__H
#define __SUPER__BOARD__H

#include "board.h"
#include "move.h"
#include <igraph.h>

struct utils_graph_t{
  int x;
  int y;
  int center;
};

/** Stores all completed structures (i.e. structures whose points have already been computed).
 */
struct finished_structures_t {
  int *list; /* Pointer to an array containing the first vertex of each completed structures. */
  int count; /* The number of elements inside "list". */
  int size;  /* The number of "cells" allocated in memory for "list" (sizeof(list) = size*sizeof(int)). */
};

// the size of list is size
struct super_board_t{
  struct board_t *board;
  igraph_t graph;
  struct utils_graph_t *list;
  struct finished_structures_t finished_structures;
  enum color_t* colors;
  int capacite;
  int size;
};

//Init the board
void init_super_board(struct tile_t tile, struct super_board_t* super_board) ;


// TO DO :
// facteur *13 pour la capcité pour les couleurs par rapport à list
int add_tile_to_super_board(struct tile_t tile, struct super_board_t * super_board, int x,int y);


//Add the color of the tile in the board
void add_color_to_super_board(struct tile_t tile, struct super_board_t* super_board);


// free the memory in the super_board
void free_super_board(struct super_board_t* super_board);


void create_neato(struct super_board_t * super_board);

#endif
