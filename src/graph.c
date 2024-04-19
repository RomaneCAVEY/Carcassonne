#include <stdio.h>
#include "graph.h"
#include "common.h"


igraph_t transform_tile_to_graph(struct tile_t tile) 
{

	igraph_matrix_t mat;
	igraph_t graph;
	int m[13][13] = { {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0} };
	unsigned int tcenter = tile.t[12];
	enum color_t ccenter = tile.c[12];
	int taille = 13;
	if (tile.c[12] == GRAY) {
		taille = 12;
	}
	for (int i = 0; i < taille; ++i) {
		int pass = -1;
		// add_color(tile.c[i],super_board);
		for (int j = i; j < taille; ++j) {
			if ((tile.t[j] == tile.t[i]) && (tile.c[i] != ccenter) && (j != i) && (pass == -1)) {
				m[i][j] = 1;
				m[j][i] = 1;
				pass = i;
			}
		}
		if (taille == 12) {
			if (ccenter == tile.c[i]) {
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


igraph_t add_tile_to_graph(struct tile_t tile, igraph_t main_graph, struct super_board_t* sboard, int x, int y)
{
	igraph_t graph_tile = transform_tile_to_graph(tile);

	igraph_t union_graph;

	// vector with new edges
	igraph_vector_int_t add_edges;

	igraph_disjoint_union(&union_graph, &main_graph, &graph_tile);

	int size = sboard->size;
	int center_tile = ((size)*13 - 1)+13;
	printf("Center of tile : %d\n", center_tile);
	int center_add;


	for (int i = 0; i < size; ++i) {
		// Connect with the down tile
		if ((sboard->list[i].x == x) && (sboard->list[i].y+1 == y)) {
			printf("down\n");
			center_add = sboard->list[i].center;
			igraph_vector_int_init_int(&add_edges, 6, (center_tile-12),(center_add-4), (center_tile-11),(center_add-5), (center_tile-10),(center_add-6));
			igraph_add_edges(&union_graph, &add_edges, NULL);
		}
		// Connect with the up tile
		if ((sboard->list[i].x == x) && (sboard->list[i].y-1 == y)) {
			printf("up\n");
			center_add = sboard->list[i].center;
			igraph_vector_int_init_int(&add_edges, 6, (center_tile-4),(center_add-12), (center_tile-5),(center_add-11), (center_tile-6),(center_add-10));
			igraph_add_edges(&union_graph, &add_edges, NULL);
		}
		// Connect with the right tile
		if ((sboard->list[i].x+1 == x) && (sboard->list[i].y == y)) {
			printf("right\n");
			center_add = sboard->list[i].center;
			printf("Center of left tile : %d\n", center_add);
			igraph_vector_int_init_int(&add_edges, 6, (center_tile-1),(center_add-9), (center_tile-2),(center_add-8), (center_tile-3),(center_add-7));
			igraph_add_edges(&union_graph, &add_edges, NULL);
		}
		// Connect with the left tile
		if ((sboard->list[i].x-1 == x) && (sboard->list[i].y == y)) {
			printf("left\n");
			center_add = sboard->list[i].center;
			igraph_vector_int_init_int(&add_edges, 6, (center_tile-9),(center_add-1), (center_tile-8),(center_add-2), (center_tile-7),(center_add-3));
			igraph_add_edges(&union_graph, &add_edges, NULL);
		}
	}
	
	igraph_destroy(&main_graph);
    igraph_destroy(&graph_tile);
	igraph_vector_int_destroy(&add_edges);

	return union_graph;
}


void create_dot_igraph1(igraph_t graph)
{
	FILE* out = fopen("player1_graph.dot", "w");
	igraph_write_graph_dot(&graph, out);
	fclose(out);
}


void create_dot_igraph2(igraph_t graph)
{
	FILE* out = fopen("player2_graph.dot", "w");
	igraph_write_graph_dot(&graph, out);
	fclose(out);
}

int calculate_points(struct super_board_t board) {
  igraph_vector_int_t components;
  igraph_vector_int_t csize;
  igraph_integer_t count;

  igraph_connected_components(&board.graph, &components, &csize, &count, IGRAPH_WEAK);

  igraph_vector_int_t nb_vertices = igraph_vector_size(&components);
  int *vertices = malloc(nb_vertices * sizeof(int));
  int size;
  struct int_pair_t *tiles_sides = malloc(nb_vertices * sizeof(struct int_pair_t));

  /* Steps (for each component):
   *  - List all the tiles it spans on
   *  - For each tile, enumerate the sides the component is on
   *  - Check for each of these sides if there is a tile next to it (we don't care if the tile properly connects with it, as this should be ensured by the game logic)
   *  - If thats the case for all, the count the number of vertices
   *  - Count the number of center vertices
   *  - The score for that component is: (nb_vertices+center_vertices)/2 * color_score_factor
   */

  // Pour chaque composant
  for (igraph_integer_t i = 0; i < count; i++) {
    // On récupère les indices des sommets du composant
    size = vector_extract_component(components, i, vertices);
    int is_finished = 1;

    int tile_count = 0;
    // On détermine la liste des (tuile, côté) sur laquelle la composante s'étend
    
    // TODO: skip les composantes terminées déjà évaluées à un tour précédent
    // (pour ça, on peut lister les premiers sommets de chaque composante déjà évaluée dans super_board par exemple)
    for (int j = 0; j < size; j++) {
      // On détermine quel côtés est concerné
      int tile = vertices[j] / 13;
      int side = (vertices[j] % 13) / 3; // 0 = nord, 1 = est, etc.

      // Si le couple (tuile, côté) est déjà connu, on ne l'ajoute pas une deuxième fois.
      if (side < 4 && is_int_pair_in_list(tiles_sides, tile_count, tile, side) == 0) {
	struct int_pair_t p = {.a = tile, .b = side};
	tiles_sides[tile_count]->a = tile;
	tiles_sides[tile_count]->b = side;
	tile_count++;

	// On traite ce côté
	switch(side) {
	case 0: // North
	  if (compare_tile(board_get(board.board, board.list[tile].x, board.list[tile].y - 1), CARC_TILE_EMPTY) == 1)
	    is_finished = 0;
	  break;

	case 1: // East
	  if (compare_tile(board_get(board.board, board.list[tile].x + 1, board.list[tile].y), CARC_TILE_EMPTY) == 1)
	    is_finished = 0;
	  break;

	case 2: // South
	  if (compare_tile(board_get(board.board, board.list[tile].x, board.list[tile].y + 1), CARC_TILE_EMPTY) == 1)
	    is_finished = 0;
	  break;

	case 3: // West
	  if (compare_tile(board_get(board.board, board.list[tile].x - 1, board.list[tile].y), CARC_TILE_EMPTY) == 1)
	    is_finished = 0;
	  break;
	}
      }

      if (is_finished) {
	// TODO: calculate the points
      }
    }
  }

  free(vertices);
  free(tiles);

  return 0;
}

int vector_extract_component(igraph_vector_int_t *components, int component_id, int* result) {
  int j = 0;
  for (igraph_integer_t i =0; i < igraph_vector_int_size(&components)) {
    if (igraph_vector_get(&components, i) == component_id) {
      result[j] = i;
      j++;
    }
  }
  return j;
}

/* Counts the number of vertices in a component that correspond to a centre vertex in the tile representation. */
int count_center_vertices(igraph_vector_int_t *component) {
  /* TODO: iterate over every vertices of the component and check if index%12 == 0.
   * If that is the case, then increase a counter.
   * This counter will then be returned.
   */
  return 0;
}


void free_graph(igraph_t graph)
{
	igraph_destroy(&graph);
}

/* int main(void) {

	struct tile_t tile1 = CARC_TILE_INIT;

	igraph_t uni;
	igraph_t graph1 = transform_tile_to_graph(tile1);
	igraph_t graph2;
	igraph_empty(&graph2, 0, IGRAPH_UNDIRECTED);
	
	igraph_disjoint_union(&uni, &graph1, &graph2);

	create_dot_igraph(uni);
	igraph_destroy(&graph1);
	igraph_destroy(&graph2);
	igraph_destroy(&uni);

	return 0;
} */

	// gcc graph.c -std=gnu99 -Wall -Wextra -Wno-trampolines -fPIC -g3 -I/home/gael/Documents/lib/igraph/include/igraph -c
	// gcc graph.o -ligraph -lm -ldl -L/home/gael/Documents/lib/igraph/lib -Wl,--rpath=/home/gael/Documents/lib/igraph/lib -o tst_igraph
	// ./tst_igraph 
	// dot -Tx11 graph.dot 
