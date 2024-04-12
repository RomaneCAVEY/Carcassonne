#include "board.h"
#include "deck.h"
#include "extended_player.h"
#include "move.h"
#include "player.h"
#include "struct_board.h"
#include "tile.h"
#include <stdio.h>
#include <stdlib.h>

// VARIABLE GLOBALE
struct board_t *board_2 = NULL;
struct gameconfig_t config_2 = {};

int p2_board_min_x = 0;
int p2_board_max_x = 0;
int p2_board_min_y = 0;
int p2_board_max_y = 0;

char const *get_player_name() { return "Player_0b"; }

void initialize(unsigned int player_id, const struct move_t first_move,
                struct gameconfig_t config) {
  board_2 = board_init(first_move.tile);
  config_2 = config;
  printf("Player %u initialized!\n", player_id);
}

void update_board_bounds(struct move_t move) {
  if (move.x > p2_board_max_x && p2_board_max_x < BOARD_CENTER - 1)
    p2_board_max_x = move.x;

  if (move.y > p2_board_max_y && p2_board_max_y < BOARD_CENTER - 1)
    p2_board_max_y = move.y;

  if (move.x < p2_board_min_x && p2_board_min_x > 2 - BOARD_CENTER)
    p2_board_min_x = move.x;

  if (move.y < p2_board_min_y && p2_board_min_y > 2 - BOARD_CENTER)
    p2_board_min_y = move.y;
}

struct move_t play(const struct move_t previous_move,
                   const struct tile_t tile) {
  board_add(board_2, previous_move.tile, previous_move.x, previous_move.y);
  update_board_bounds(previous_move);

  struct move_t current_move = {};
  current_move.player_id = 1;

  for (int i = p2_board_min_x - 1; i < p2_board_max_x + 2; i++) {
    for (int j = p2_board_min_y - 1; j < p2_board_max_y + 2; j++) {
      //printf("Trying slot(%d, %d)", i, j);
      if (board_add_check(board_2, tile, i, j)) {
	//printf(" Valid pos\n");
        current_move.x = i;
        current_move.y = j;
        // Note: We're using the last of all possible placements. This can be
        // further optimized.
      }
      //printf("\n");
    }
  }

  current_move.tile = tile;

  board_add(board_2, current_move.tile, current_move.x, current_move.y);
  update_board_bounds(current_move);

  return current_move;
}

int is_place_available(struct board_t *board, int i, int j,
                       struct tile_t tile) {
  if (!(compare_tile(board_get(board, i - 1, j), CARC_TILE_EMPTY) &&
        tile_check(board_get(board, i - 1, j), tile, WEST)))
    return 1;

  if (!(compare_tile(board_get(board, i, j + 1), CARC_TILE_EMPTY) &&
        tile_check(board_get(board, i - 1, j), tile, NORTH)))
    return 1;

  if (!(compare_tile(board_get(board, i, j - 1), CARC_TILE_EMPTY) &&
        tile_check(board_get(board, i - 1, j), tile, SOUTH)))
    return 1;

  if (!(compare_tile(board_get(board, i + 1, j), CARC_TILE_EMPTY) &&
        tile_check(board_get(board, i - 1, j), tile, EAST)))
    return 1;

  return 0;
}

/* Clean up the resources the player has been using. Is called once at
   the end of the game.
 * POSTCOND:
 * - every allocation done during the calls to initialize and play
 *   functions must have been freed
 */
void finalize() {
  board_free(board_2);
  deck_free(config_2.deck);
}
