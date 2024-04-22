#ifndef __SCORE_H__
#define __SCORE_H__

#include <igraph.h>
#include "super_board.h"
#include "move.h"
#include "common.h"

/** Calculates all points for the current state of the game
 *
 * @param board A pointer to the super board
 * @param mode The selected game mode
 * @param current_player The index of the current player (0 or 1)
 * @return An int_pair_t containing the amount of points should receive (pair.a for player 0 and pair.b for player 1)
 *
 * TODO(): Add support for all game modes (currently, only NO_MEEPLE is supported).
 */
struct int_pair_t calculate_points(struct super_board_t *board, enum gamemode_t mode, int current_player);

/** Adds a finished structure to the list of already evaluted structures
 *
 * @param sb A pointer to the super board
 * @param first_vertex The smallest vertex id for the connected component corresponding to that structure
 */
void add_finished_structure(struct super_board_t *sb, int first_vertex);

/** Returns the score factor associated to a color
 *
 * @param color A color
 * @return The score factor (1 for fields, 4 for roads, 8 for castles, etc.)
 */
int color_score_factor(enum color_t color);

/** Extracts the vertex indices of all vertices that are in a component.
 *
 * @param components An igraph vector associating to each vertex its component id
 * @param component_id The id of the component we want to extract
 * @param results A pointer to an array of integers that will contain the vertices ids
 * @return The vertex count of the component
 */
int vector_extract_component(igraph_vector_int_t components, int component_id, int* result);

/** Counts the number of vertices inside a component that correspond to a tile's center
 *
 * @param component A pointer to the component (array of vertex indices)
 * @param size The amount of vertices in the component
 * @return The amount of center vertices
 */
int count_center_vertices(int *component, int size);


#endif // __SCORE_H__
