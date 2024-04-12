#include"super_board.h"
#include "move.h"
#include "graph.h"
#include <stdlib.h>



/*Init the super_board
* @param: the size of colors, the first tile to init the board, the super_board
* @return:the super_board with the first tile in the board
*/
void init_super_board(struct tile_t tile,struct super_board_t *super_board){
	super_board->board=board_init(tile);
	super_board->colors=malloc(sizeof(enum color_t)*13);
	super_board->list=malloc(sizeof(struct utils_graph_t)*1);
	struct utils_graph_t ugraph = {.x=0, .y=0, .center=12};
	super_board->list[0] = ugraph;
	super_board->capacite=1;
	super_board->size=1;
}

/*Add the colors of a vertice of the graph in the board
* @param: sommet=numero du sommet, c= sa couleur, super_board
* @return:the super_board with the color of the new tile
*/
void add_color(int sommet, enum color_t c,struct super_board_t* board){
	if (board->capacite==board->size){
		// board->capacite=board->capacite*2; 
		board->colors=realloc(board->colors, 13*board->capacite*sizeof(enum color_t));
	}
	board->colors[sommet]=c;
}

/*Free the memory
* @param:the super_board
* @return: void
*/
void free_super_board(struct super_board_t* super_board){
	free(super_board->colors);
	free(super_board->list);
}
