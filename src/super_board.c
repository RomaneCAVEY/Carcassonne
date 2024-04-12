#include"super_board.h"
#include "move.h"
#include "graph.h"
#include <stdlib.h>



/*Init the super_board
* @param: the size of colors, the first tile to init the board, the super_board
* @return:the super_board with the first tile in the board
*/
void init_super_board(struct tile_t tile,struct super_board_t *super_board){
	super_board->board = board_init(tile);
	super_board->graph = init_graph(tile);
	super_board->colors = malloc(sizeof(enum color_t)*MAX_CONNECTIONS);
	super_board->list = malloc(sizeof(struct utils_graph_t)*1);
	struct utils_graph_t ugraph = {.x=0, .y=0, .center=12};
	super_board->list[0] = ugraph;
	super_board->capacite = 1;
	super_board->size = 1;
}

/*Add the colors of a vertice of the graph in the board
* @param: sommet=numero du sommet, c= sa couleur, super_board
* @return:the super_board with the color of the new tile
*/
void add_color_to_super_board(struct tile_t tile, struct super_board_t* super_board)
{
	for (int i = 0; i < MAX_CONNECTIONS; ++i) {
		super_board->colors[MAX_CONNECTIONS*super_board->size + i] = tile.c[i];
	}
}


int size_super_board(struct super_board_t * super_board)
{
	return super_board->size;
}
/* 
struct utils_graph_t{
	int x;
	int y;
	int center;
};

// the size of list is size
struct super_board_t{
	struct board_t *board;
	igraph_t graph;
	struct utils_graph *list;
	enum color_t* colors;
	int capacite;
	int size;
};
*/


void add_to_utils_graph(struct super_board_t * super_board, int x, int y, int center)
{
	struct utils_graph_t u = {.x=x, .y=y, .center=center};
	super_board->list[size_super_board(super_board)] = u;
}


void add_tile_to_super_board(struct tile_t tile, struct super_board_t * super_board, int x, int y)
{
	if (super_board->capacite == super_board->size) {
		super_board->capacite *= 2;
		super_board->colors = realloc(super_board->colors, 13*super_board->capacite*sizeof(enum color_t));
		super_board->list = realloc(super_board->list, super_board->capacite*sizeof(struct utils_graph_t));
	}
	board_add(super_board->board, tile, x, y);
	add_tile_to_graph(tile, super_board->graph, super_board, x, y);
	add_to_utils_graph(super_board, x, y, (size_super_board(super_board)+1)*13); // n° of the last vertice in the tile
	add_color_to_super_board(tile, super_board);
	super_board->size += 1;
}

/*Free the memory
* @param:the super_board
* @return: void
*/
void free_super_board(struct super_board_t* super_board){
	free(super_board->colors);
	free(super_board->list);
	free_graph(super_board->graph);
}
