#ifndef __SUPER__BOARD__H
#define __SUPER__BOARD__H

#include "board.h"
#include "move.h"
#include <igraph.h>
#include "meeple.h"

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
  struct meeple_t meeple;
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


void create_neato(struct super_board_t * super_board, char * file_name);

/*Copy the super_board
* @param:the super_board
* @return: a copy of the super_board
*/
struct super_board_t copy_super_board(struct super_board_t super_board);


//client 
/*Add find where to place a meeple
 *@param: the meeple and the move and super_board
 *@return: the moove with the new meeple according to the player who played 
 * */
int add_meeple(struct move_t* move, struct super_board_t sboard, enum gamemode_t gt);

//serveur
/*Add the meeple played in the move
 *@param: the meeple and the move and super_board
 *@return: the meeple with the new meeple according to the player who played 
 * */
int add_meeple_to_board(struct meeple_t *meeple, struct move_t *move, struct super_board_t sboard, enum gamemode_t gt);
/* check if there is a valide place to let a meeple*/
int check_add_meeple( struct super_board_t sboard, enum conn_t indexVertex, struct meeple_t *meeple);

void free_copy_super_board(struct super_board_t* copy);



#endif



