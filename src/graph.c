#include <stdio.h>
#include "graph.h"


igraph_t transform_tile_to_graph(struct tile_t tile) {

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

int main(void) {

	FILE* out = fopen("graph.dot", "w");

	struct tile_t tile = CARC_TILE_INIT;

	igraph_t graph = transform_tile_to_graph(tile);
	
	igraph_write_graph_edgelist(&graph, stdout);
	igraph_write_graph_dot(&graph, out);
	fclose(out);
	igraph_destroy(&graph);

	return 0;

	// gcc graph.c -std=gnu99 -Wall -Wextra -Wno-trampolines -fPIC -g3 -I/home/gael/Documents/lib/igraph/include/igraph -c
	// gcc graph.o -ligraph -lm -ldl -L/home/gael/Documents/lib/igraph/lib -Wl,--rpath=/home/gael/Documents/lib/igraph/lib -o tst_igraph
	// ./tst_igraph 
	// dot -Tx11 graph.dot 
}