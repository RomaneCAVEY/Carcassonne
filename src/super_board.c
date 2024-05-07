#include "super_board.h"
#include "board.h"
#include "move.h"
#include "graph.h"
#include <stdlib.h>
#include <stdio.h>

const char * str_color[8] = {
  "GREEN", "GRAY", "BLUE", "BROWN",
  "PURPLE", "GOLD", "AZURE", "MAHOGANY",
};

/*Init the super_board
* @param: the size of colors, the first tile to init the board, the super_board
* @return:the super_board with the first tile in the board
*/
void init_super_board(struct tile_t tile, struct super_board_t *super_board){
  super_board->board = board_init(tile);
  super_board->graph = init_graph(tile);
  super_board->colors = malloc(sizeof(enum color_t)*MAX_CONNECTIONS);
  super_board->list = malloc(sizeof(struct utils_graph_t)*1);
  struct utils_graph_t ugraph = {.x=0, .y=0, .center=12};
  super_board->list[0] = ugraph;
  super_board->capacite = 1;
  super_board->size = 0;
  add_color_to_super_board(tile, super_board);
  super_board->size++;

  super_board->finished_structures.count = 0;
  super_board->finished_structures.size = 1;
  super_board->finished_structures.list = malloc(sizeof(int));
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


void add_to_utils_graph(struct super_board_t * super_board, int x, int y, int center)
{
	struct utils_graph_t u = {.x=x, .y=y, .center=center};
	super_board->list[size_super_board(super_board)] = u;
}


// return 1 if success else return 0 
int add_tile_to_super_board(struct tile_t tile, struct super_board_t * super_board, int x, int y)
{
	for (int i = 0; i < super_board->size; ++i) {
		if ((super_board->list[i].x == x) && (super_board->list[i].y == y))
			return 0;
	}
	// printf("------ You want to add a tile on a super board ------\n");
	if (super_board->capacite == super_board->size) {
		// printf("Capacity double\n");
		super_board->capacite *= 2;
		super_board->colors = realloc(super_board->colors, 13*super_board->capacite*sizeof(enum color_t));
		super_board->list = realloc(super_board->list, super_board->capacite*sizeof(struct utils_graph_t));
	}
	// printf("add tile to board \n");
	board_add(super_board->board, tile, x, y);
	// printf("add in utils graph\n");
	add_to_utils_graph(super_board, x, y, (size_super_board(super_board)*13-1)+13); // n° of the last vertice in the tile
	// printf("add colors\n");
	add_color_to_super_board(tile, super_board);
	// printf("add tile to a graph\n");
	super_board->graph = add_tile_to_graph(tile, super_board->graph, super_board, x, y);
	super_board->size += 1;
	// printf("------ End add a tile on a super board ------\n");
	return 1;
}

void create_neato(struct super_board_t * super_board, char * file_name)
{
	FILE* out = fopen(file_name, "w+");
	fprintf(out, "graph {\n");
	fprintf(out, "  node[style=filled];\n");
	for (int i=0; i < super_board->size; ++i) {
		// printf("\n-----Index %d-----\n", i);
		float x = (float) super_board->list[i].x * 5;
		float y = (float) - super_board->list[i].y * 5;
		int center = super_board->list[i].center;
		struct tile_t tile = board_get(super_board->board, x/5, -y/5);
		// tile_display(tile);
		// printf("x = %f, y = %f, center = %d", x, y, center);
		for (int j=(center-12); j < center+1; ++j) {
			// printf("\n-----Index %d-----\n", j%13);
			switch (j%13)
			{
			case 0:
				fprintf(out, "  %d [fillcolor=%s,shape=\"circle\",pos=\"%f, %f!\"];\n", j, str_color[tile.c[j%13]], x-1, y+2);
				break;
			case 1:
				fprintf(out, "  %d [fillcolor=%s,shape=\"circle\",pos=\"%f, %f!\"];\n", j, str_color[tile.c[j%13]], x, y+2);
				break;
			case 2:
				fprintf(out, "  %d [fillcolor=%s,shape=\"circle\",pos=\"%f, %f!\"];\n", j, str_color[tile.c[j%13]], x+1, y+2);
				break;
			case 3:
				fprintf(out, "  %d [fillcolor=%s,shape=\"circle\",pos=\"%f, %f!\"];\n", j, str_color[tile.c[j%13]], x+2, y+1);
				break;
			case 4:
				fprintf(out, "  %d [fillcolor=%s,shape=\"circle\",pos=\"%f, %f!\"];\n", j, str_color[tile.c[j%13]], x+2, y);
				break;
			case 5:
				fprintf(out, "  %d [fillcolor=%s,shape=\"circle\",pos=\"%f, %f!\"];\n", j, str_color[tile.c[j%13]], x+2, y-1);
				break;
			case 6:
				fprintf(out, "  %d [fillcolor=%s,shape=\"circle\",pos=\"%f, %f!\"];\n", j, str_color[tile.c[j%13]], x+1, y-2);
				break;
			case 7:
				fprintf(out, "  %d [fillcolor=%s,shape=\"circle\",pos=\"%f, %f!\"];\n", j, str_color[tile.c[j%13]], x, y-2);
				break;
			case 8:
				fprintf(out, "  %d [fillcolor=%s,shape=\"circle\",pos=\"%f, %f!\"];\n", j, str_color[tile.c[j%13]], x-1, y-2);
				break;
			case 9:
				fprintf(out, "  %d [fillcolor=%s,shape=\"circle\",pos=\"%f, %f!\"];\n", j, str_color[tile.c[j%13]], x-2, y-1);
				break;
			case 10:
				fprintf(out, "  %d [fillcolor=%s,shape=\"circle\",pos=\"%f, %f!\"];\n", j, str_color[tile.c[j%13]], x-2, y);
				break;
			case 11:
				fprintf(out, "  %d [fillcolor=%s,shape=\"circle\",pos=\"%f, %f!\"];\n", j, str_color[tile.c[j%13]], x-2, y+1);
				break;
			case 12:
				fprintf(out, "  %d [fillcolor=%s,shape=\"circle\",pos=\"%f, %f!\"];\n", j, str_color[tile.c[j%13]], x, y);
				break;
			default:
				break;
			}
		}
	}
	fprintf(out, "}\n");
	fclose(out);
	// neato -Tx11 neato_graph.dot &
}

/* void add_node(FILE* out, int j, float x, float y, struct tile_t tile)
{
	if (j%13 < 3)
		fprintf(out, "  %d [fillcolor=%s,shape=\"circle\",pos=\"%f, %f!\"];\n", j, str_color[tile.c[j%13]], x-1, y+2);
} */
/*Free the memory
* @param:the super_board
* @return: void
*/
void free_super_board(struct super_board_t* super_board){
  free(super_board->colors);
  free(super_board->list);
  free_graph(super_board->graph);
  free(super_board->finished_structures.list);
}
