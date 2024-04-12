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

// the size of list is size
struct super_board_t{
	struct board_t *board;
	igraph_t graph;
	struct utils_graph_t *list;
	enum color_t* colors;
	int capacite;
	int size;
};

//Init the board
void init_super_board(struct tile_t tile, struct super_board_t* super_board) ;


// TO DO :
// facteur *13 pour la capcité pour les couleurs par rapport à list
void add_tile_to_super_board(struct tile_t tile, struct super_board_t * super_board, int x,int y);


//Add the color of the tile in the board
void add_color_to_super_board(struct tile_t tile, struct super_board_t* super_board);


// free the memory in the super_board
void free_super_board(struct super_board_t* super_board);

#endif
