#include"super_board.h"
#include "move.h"
#include <stdlib.h>



/*Init the super_board
* @param: the size of colors, the first tile to init the board, the super_board
* @return:the super_board with the first tile in the board
*/
void init_super_board(int size,struct tile_t tile,struct super_board_t *super_board){
	super_board->board=board_init(tile);
	super_board->colors=malloc(sizeof(enum color_t)*size);
	super_board->capacite=size;
	super_board->size=1;
}

/*Init the super_board
* @param: the size of colors, the first tile to init the board, the super_board
* @return:the super_board with the first tile in the board
*/
void add_color(int sommet, enum color_t c,struct super_board_t* board){
	if (board->capacite==board->size){
		board->capacite=board->capacite*2;
		board->colors=realloc(board->colors, 2*board->capacite);
	}
	board->colors[sommet]=c;
	board->size+=1;
}

/*Free the memory
* @param:the super_board
* @return: void
*/
void free_super_board(struct super_board_t* super_board){
	free(super_board->colors);
}