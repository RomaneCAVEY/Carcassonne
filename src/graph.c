#include <stdio.h>
#include "graph.h"


igraph_t transform_tile_to_graph(struct tile_t tile) 
{

	igraph_matrix_t mat;
	igraph_t graph;
	int m[13][13] = { {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0} };
	unsigned int center = tile.t[12];
	int taille = 13;
	if (tile.c[12] == GRAY) {
		taille = 12;
	}
	for (int i = 0; i < taille; ++i) {
		int pass = -1;
		// add_color(tile.c[i],super_board);
		for (int j = i; j < taille; ++j) {
			if ((tile.t[j] == tile.t[i]) && (tile.t[i] != center) && (j != i) && (pass == -1)) {
				m[i][j] = 1;
				m[j][i] = 1;
				pass = i;
			}
		}
		if (taille == 12) {
			if (center == tile.t[i]) {
				m[i][taille] = 1;
				m[taille][i] = 1;
			}
		}
	}
	igraph_vector_t weights;
	igraph_vector_int_t el;
	igraph_integer_t i, j;
	igraph_vector_int_init(&el, 0);
	igraph_vector_init(&weights, 0);

	igraph_matrix_init(&mat, 13, 13);
	for (i = 0; i < 13; i++) for (j = 0; j < 13; j++) {
		MATRIX(mat, i, j) = m[i][j];
	}

	igraph_weighted_adjacency(&graph, &mat, IGRAPH_ADJ_UNDIRECTED, &weights, IGRAPH_LOOPS_ONCE); // UNDIRECTED 
	igraph_matrix_destroy(&mat);
	igraph_vector_destroy(&weights);
	igraph_vector_int_destroy(&el);

	return graph;

}


igraph_t init_graph(struct tile_t t)
{
	return transform_tile_to_graph(t);
}


igraph_t add_tile_to_graph(struct tile_t tile, igraph_t main_graph, struct super_board sboard, int x, int y)
{
	igraph_t graph_tile = transform_tile_to_graph(tile);
	igraph_t union_graph;

	// vector with new edges
	igraph_vector_int_t add_edges;

	igraph_disjoint_union(&union_graph, &main_graph, &graph_tile);

	int size = sboard.size; // tile already in super_board
	int center_tile = (size)*13;
	int center_add;
	for (int i = 0; i < size; ++i) {
		// Connect with the down tile
		if ((sboard.list[i].x == x) && (sboard.list[i].y == y+1)) {
			center_add = sboard.list[i].center;
			igraph_vector_int_init_int(&add_edges, 6, (center_tile-12),(center_add-4), (center_tile-11),(center_add-5), (center_tile-10),(center_add-6));
			igraph_add_edges(&union_graph, &add_edges, NULL);
		}
		// Connect with the up tile
		if ((sboard.list[i].x == x) && (sboard.list[i].y == y-1)) {
			center_add = sboard.list[i].center;
			igraph_vector_int_init_int(&add_edges, 6, (center_tile-4),(center_add-12), (center_tile-5),(center_add-11), (center_tile-6),(center_add-10));
			igraph_add_edges(&union_graph, &add_edges, NULL);
		}
		// Connect with the right tile
		if ((sboard.list[i].x == x+1) && (sboard.list[i].y == y)) {
			center_add = sboard.list[i].center;
			igraph_vector_int_init_int(&add_edges, 6, (center_tile-1),(center_add-9), (center_tile-2),(center_add-8), (center_tile-3),(center_add-7));
			igraph_add_edges(&union_graph, &add_edges, NULL);
		}
		// Connect with the left tile
		if ((sboard.list[i].x == x-1) && (sboard.list[i].y == y)) {
			center_add = sboard.list[i].center;
			igraph_vector_int_init_int(&add_edges, 6, (center_tile-9),(center_add-1), (center_tile-8),(center_add-2), (center_tile-7),(center_add-3));
			igraph_add_edges(&union_graph, &add_edges, NULL);
		}
	}

	igraph_destroy(&main_graph);
    igraph_destroy(&graph_tile);
	igraph_vector_int_destroy(&add_edges);

	return union_graph;
}


void free_graph(igraph_t graph)
{
	igraph_destroy(&graph);
}

int main(void) {

	FILE* out = fopen("graph.dot", "w");

	struct tile_t tile1 = CARC_TILE_INIT;
	struct tile_t tile2 = CARC_TILE_INIT;

	igraph_vector_int_t add_edges;
	igraph_vector_int_init_int(&add_edges, 6, 3,24, 4,23, 5,22);

	igraph_t uni;
	igraph_t graph1 = transform_tile_to_graph(tile1);
	igraph_t graph2 = transform_tile_to_graph(tile2);
	
	igraph_disjoint_union(&uni, &graph1, &graph2);
	igraph_add_edges(&uni, &add_edges, NULL);

	igraph_write_graph_dot(&uni, out);
	fclose(out);
	igraph_destroy(&graph1);
	igraph_destroy(&graph2);
	igraph_destroy(&uni);

	// test
	/* struct super_board sboard = {};
	init_super_board(13, tile1, &sboard); */

	return 0;
}

	// gcc graph.c -std=gnu99 -Wall -Wextra -Wno-trampolines -fPIC -g3 -I/home/gael/Documents/lib/igraph/include/igraph -c
	// gcc graph.o -ligraph -lm -ldl -L/home/gael/Documents/lib/igraph/lib -Wl,--rpath=/home/gael/Documents/lib/igraph/lib -o tst_igraph
	// ./tst_igraph 
	// dot -Tx11 graph.dot 
