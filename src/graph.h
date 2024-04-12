#include <stdio.h>

#include <igraph.h>
#include <stdio.h>
#include "move.h"
#include "board.h"
#include "deck.h"
#include "super_board.h"

//Trasnform a tile to graph
igraph_t transform_tile_to_graph(struct tile_t/* ,struct super_board */);


//Init the graph with the first tile
igraph_t init_graph(struct tile_t t);

//Add a tile which is transformed in graph to the main graph and add the colors of the vertices to the tab color
//Use realloc if colors is full (but no size so...)
igraph_t add_tile_to_graph(struct tile_t tile, igraph_t main_graph, struct super_board_t* sboard, int x, int y);

// Create a .dot with the graph of the party
void create_dot_igraph1(igraph_t graph);

void create_dot_igraph2(igraph_t graph);

//free graph
void free_graph(igraph_t graph);

