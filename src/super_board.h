#ifndef __SUPER__BOARD__H
#define __SUPER__BOARD__H

#include "move.h"
struct super_board{
	struct board;
	enum color_t* colors;
	int capacite;
	int size;
};


struct super_board init_board(int taille);


//realloc to use 
void add_color(enum color_t,struct super_board*);

#endif