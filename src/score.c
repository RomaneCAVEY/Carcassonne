#include <stdio.h>
#include "score.h"
#include "deck.h"
#include "super_board.h"


struct int_pair_t calculate_points(struct super_board_t *board, enum gamemode_t mode, int current_player) {  
  igraph_vector_int_t components;
  igraph_vector_int_t csize;
  igraph_integer_t count = 0;

  igraph_vector_int_init(&csize, 0);
  igraph_vector_int_init(&components, 0);

  igraph_connected_components(&board->graph, &components, &csize, &count, IGRAPH_WEAK);

  igraph_integer_t nb_vertices = igraph_vector_int_size(&components);

  int *vertices = malloc(nb_vertices * sizeof(int));
  int size;
  struct int_pair_t *tiles_sides = malloc(nb_vertices * sizeof(struct int_pair_t));

  struct int_pair_t total = {.a = 0, .b = 0};

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

    // Skip already evaluted components (the points awarded by a finished structure should only be counted once).
    // Components of size 1 are excluded, since they should not be considered as structures (this should only happen on the center vertex of CARC_TILE_XROAD).
    if (size < 2 || is_int_in_list(board->finished_structures.list, board->finished_structures.count, vertices[0])) { 
      //printf("[score] Component %ld too small or already evaluted (size: %d)\n", i, size);
      continue;
    }
    
    int is_finished = 1;

    int tile_count = 0;
    // On détermine la liste des (tuile, côté) sur laquelle la composante s'étend
    
    // TODO: skip les composantes terminées déjà évaluées à un tour précédent
    // (pour ça, on peut lister les premiers sommets de chaque composante déjà évaluée dans super_board par exemple)
    for (int j = 0; j < size; j++) {
      //printf("[score] comp[%ld] : %d\n", i, vertices[j]);
      // On détermine quel côtés est concerné
      int tile = vertices[j] / 13;
      int side = (vertices[j] % 13) / 3; // 0 = nord, 1 = est, etc.

      // Si le couple (tuile, côté) est déjà connu, on ne l'ajoute pas une deuxième fois.
      if (side < 4 && is_int_pair_in_list(tiles_sides, tile_count, tile, side) == 0) {
        tiles_sides[tile_count].a = tile;
        tiles_sides[tile_count].b = side;
        tile_count++;

        // On traite ce côté
        switch(side) {
            case 0: // North
            if (compare_tile(board_get(board->board, board->list[tile].x, board->list[tile].y - 1), CARC_TILE_EMPTY) == 1) {
                is_finished = 0;
            }
            break;

            case 1: // East
            if (compare_tile(board_get(board->board, board->list[tile].x + 1, board->list[tile].y), CARC_TILE_EMPTY) == 1) {
                is_finished = 0;
            }
            break;

            case 2: // South
            if (compare_tile(board_get(board->board, board->list[tile].x, board->list[tile].y + 1), CARC_TILE_EMPTY) == 1) {
                is_finished = 0;
            }
            break;

            case 3: // West
            if (compare_tile(board_get(board->board, board->list[tile].x - 1, board->list[tile].y), CARC_TILE_EMPTY) == 1) {
                is_finished = 0;
            }
            break;
        }
      }
    }

    if (is_finished == 1) {
        // Add the structure to the list of already evaluted structures
        add_finished_structure(board, vertices[0]);
        

        // Calculate the corresponding score
        int factor = color_score_factor(board->colors[vertices[0]]); 
        int center_vertices = count_center_vertices(vertices, size);

        // We add the number of center vertices to the vertices count, since all vertices are duplicated,
        // except for the center ones. We then divide by two to get the correct amount of non-duplicate vertices.
        int score = (size + center_vertices) / 2 * factor;
        
        //printf("[score] Score for structure nb %ld (vertex %d): %d\n", i, vertices[0], score);
        if (mode == NO_MEEPLE) {
            // TODO: when playing with meeples, determine which player wins the points
            if (current_player == 0) {
                total.a = total.a + score;
            } else if (current_player == 1) {
                total.b = total.b + score;
            } else {
                printf("[score] Finished structure (id=%d) was attributed to an unknown player (id=%d). Points will not be counted.", vertices[0], current_player);
            }
        }
        
        if (mode != NO_MEEPLE) {
            struct int_pair_t nb_meeple = {.a = 0, .b = 0};
            
            for (int i = 0; i<size; i++) {
	      for (int p1 = 0; p1 < board->meeple.size1; p1++) {
                if (board->meeple.player1[p1] == vertices[i]) {
		  nb_meeple.a += 1;
                }
	      }
	      for (int p2 = 0; p2 < board->meeple.size2; p2++) {
                if (board->meeple.player2[p2] == vertices[i]) {
		  nb_meeple.b +=1;
                }
	      }
            }

            if (nb_meeple.a > nb_meeple.b) {
                total.a += score;
            }
            if (nb_meeple.a < nb_meeple.b) {
                total.b += score;
            }
            get_back_meeple(&board->meeple, vertices, size);
        }
        
        //printf("[score] Total: %d\n", total);
    } /*else {
      printf("[score] Structure nb %ld (vertex %d) is not finished\n", i, vertices[0]);
      }*/
  }

  free(vertices);
  free(tiles_sides);
  igraph_vector_int_destroy(&components);
  igraph_vector_int_destroy(&csize);

  return total;
}

void add_finished_structure(struct super_board_t *sb, int first_vertex) {
  // If the list is full, realloc it to be twice as big
  if (sb->finished_structures.size <= sb->finished_structures.count) {
    sb->finished_structures.list = realloc(sb->finished_structures.list, 2 * sb->finished_structures.size * sizeof(int));
    sb->finished_structures.size = 2 * sb->finished_structures.size;
  }

  // Add the first vertex to the list
  sb->finished_structures.list[sb->finished_structures.count] = first_vertex;
  sb->finished_structures.count++;
}

int color_score_factor(enum color_t color) {
  switch (color) {
    case GREEN:
        return 1;

    case GRAY:
        return 4;

    case BROWN:
        return 8;

    default:
        return 0;
  }
}

int vector_extract_component(igraph_vector_int_t components, int component_id, int* result) {
  int j = 0;
  for (igraph_integer_t i =0; i < igraph_vector_int_size(&components); i++) {
    if (VECTOR(components)[i] == component_id) {
      result[j] = i;
      j++;
    }
  }
  return j;
}

/* Counts the number of vertices in a component that correspond to a centre vertex in the tile representation. */
int count_center_vertices(int *component, int size) {
  int count = 0;
  for (int i = 0; i < size; i++) {
    if ((component[i] + 1) % 13 == 0) // This checks that it is the middle of a tile
      count++;
  }
  return count;
}
