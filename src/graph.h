#include <igraph.h>
#include <stdio.h>
#include "move.h"

//Trasnform a tile to graph
igraph_t transform_tile_to_graph(struct tile_t);

igraph_t add_tile_to_graph(struct tile_t,igraph_t,struct board);