#ifndef __SUPER__BOARD__H
#define __SUPER__BOARD__H

#include "board.h"
#include "move.h"

struct utils_graph_t{
	int x;
	int y;
	int center;
};

// the size of list is size
struct super_board_t{
	struct board_t *board;
	struct utils_graph *list;
	enum color_t* colors;
	int capacite;
	int size;
};

//Init the board
void init_super_board(int taille,struct tile_t tile,struct super_board_t*) ;


//Add the color of the tile in the board
void add_color(int sommet, enum color_t c,struct super_board_t* board);


// free the memory in the super_board
void free_super_board(struct super_board_t*);

#endif