#include "player.h"
#include "board.h"

 board_t board_1={};

char const* get_player_name(){
	return "Player_1";
}

void initialize(unsigned int player_id, const struct move_t first_move, struct gameconfig_t config) {
	board_1= init_board(first_move);

};


/* Computes next move
 * PARAM:
 * - previous_move: the move from the previous player. If this is the
 *                  first move of the game, `previous_move` is equal to
                    a starting tile played by player_id == -1
 * - tile:          the tile that the player is supposed to play
 * RETURNS:
 * - the next move for the player.
 */
struct move_t play(const struct move_t previous_move, const struct tile_t tile){
	board_add_tile(tile, board_1);

}

/* Clean up the resources the player has been using. Is called once at
   the end of the game.
 * POSTCOND:
 * - every allocation done during the calls to initialize and play
 *   functions must have been freed
 */
void finalize(){
	destroy_board(board_1);
	}
