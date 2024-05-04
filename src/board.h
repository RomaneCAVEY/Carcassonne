#ifndef __BOARD_H__
#define __BOARD_H__

#include "tile.h"
// tile.h -> move.h -> deck.h

/* Coordonnées dans board: 
 * Centré en (0, 0)
 * x positif vers la droite
 * y positif vers le bas
 */

struct board_t;

struct board_t *board_init(struct tile_t tile);

/* Retourne 0 si ajout impossible, 1 si ajouté avec succès.
 *
 */
int board_add(struct board_t *board, struct tile_t tile, int x, int y);

int board_add_check(struct board_t *board, struct tile_t tile, int x, int y);

struct tile_t board_get(struct board_t *board, int x, int y);

void board_free(struct board_t *board);


void board_display(struct board_t *board);

/*Copy the board
* @param: a board
* @return: a pointer of a copy of the board
*/
struct board_t* copy_board(struct board_t *board);


#endif // __BOARD_H__
