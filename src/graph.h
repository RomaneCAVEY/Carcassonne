#include <igraph.h>
#include <stdio.h>
#include "move.h"

//Trasnform a tile to graph
igraph_t transform_tile_to_graph(struct tile_t);



//Add a tile which is transformed in graph to the main graph and add the colors of the vertices to the tab color
//Use realloc if colors is full (but no size so...)
igraph_t add_tile_to_graph(struct tile_t tile,igraph_t g,struct board, enum color_t* color);

