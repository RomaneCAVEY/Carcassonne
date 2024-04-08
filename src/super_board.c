#include"super_board.h"
#include "move.h"
#include <stdlib.h>



struct super_board init_board(int taille){
	struct super_board super_board={};
	super_board.colors=malloc(sizeof(enum color_t)*taille);

}


void add_color(int sommet, enum color_t c,struct super_board* board){
	if (board->capacite==board->size){
		board->capacite=board->capacite*2;
		board->colors=realloc(board->colors, 2*board.capacite);
	}
	board->color[sommet]=c;
	board.size+=1;
}
