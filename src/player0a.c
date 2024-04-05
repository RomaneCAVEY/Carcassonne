#include "board.h"
#include "deck.h"
#include "move.h"
#include "player.h"
#include "tile.h"
#include <stdlib.h>
#include <stdio.h>

#ifndef BOARD_SIZE
#define BOARD_SIZE 201
#endif

// VARIABLE GLOBALE
struct board_t *board_1 = NULL;
struct gameconfig_t config_1 = {};

char const *get_player_name() { return "Player_0a"; }

void initialize(unsigned int player_id, const struct move_t first_move,
    struct gameconfig_t config)
{
    board_1 = board_init(first_move.tile);
    config_1 = config;
    printf("Player %u initialized!\n", player_id);
}

/* Computes next move
 * PARAM:
 * - previous_move: the move from the previous player. If this is the
 *                  first move of the game, `previous_move` is equal to
                    a starting tile played by player_id == -1
 * - tile:          the tile that the player is supposed to play
 * RETURNS:
 * - the next move for the player.
 */

struct move_t play(const struct move_t previous_move, const struct tile_t tile)
{
    board_add(board_1, previous_move.tile, previous_move.x, previous_move.y);
    struct move_t current_move = {};
    current_move.player_id = 1;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (compare_tile(board_get(board_1, i, j), CARC_TILE_EMPTY) == 0) {
                if (compare_tile(board_get(board_1, i - 1, j), CARC_TILE_EMPTY)
                        == 0
                    || compare_tile(
                           board_get(board_1, i, j + 1), CARC_TILE_EMPTY)
                        == 0
                    || compare_tile(
                           board_get(board_1, i + 1, j), CARC_TILE_EMPTY)
                        == 0
                    || compare_tile(
                           board_get(board_1, i, j - 1), CARC_TILE_EMPTY)
                        == 0) {
                    current_move.x = i;
                    current_move.y = j;
                }
            }
        }
    }

    current_move.tile = tile;
    board_add(board_1, current_move.tile, current_move.x, current_move.y);
    return current_move;
}

/* Clean up the resources the player has been using. Is called once at
   the end of the game.
 * POSTCOND:
 * - every allocation done during the calls to initialize and play
 *   functions must have been freed
 */
void finalize()
{
    board_free(board_1);
    deck_free(config_1.deck);
}
