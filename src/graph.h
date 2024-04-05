#include <stdio.h>
#include "graph.h"

#include <igraph.h>
#include <stdio.h>
#include "move.h"
#include "board.h"
#include "deck.h"

//Trasnform a tile to graph
igraph_t transform_tile_to_graph(struct tile_t);


//Init the graph with the first tile
igraph_t init_graph(struct tile_t t);

//Add a tile which is transformed in graph to the main graph and add the colors of the vertices to the tab color
//Use realloc if colors is full (but no size so...)
igraph_t graph_add(struct tile_t tile,igraph_t g,struct board, enum color_t* color);


//free graph
void free_graph(igraph_t graphe);

