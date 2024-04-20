#ifndef __SCORE_H__
#define __SCORE_H__

#include <igraph.h>
#include "super_board.h"

/** Calculates all points for the current state of the game
 *
 * @param board A pointer to the super board
 * @return The sum of the points of all finished structures at this state of the game.
 *
 * TODO(): This function should only count the points for structure that haven't been evaluted before
 * TODO(): When playing with meeples, this function should return points for each player and not a global sum.
 */
int calculate_points(struct super_board_t *board);

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
