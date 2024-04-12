#include "move.h"
#include "extended_player.h"
#include "board.h"
#include <stdlib.h>
#include "graph.h"
#include "deck.h"
#include "board.h"
#include "super_board.h"


#ifndef BOARD_SIZE
#define BOARD_SIZE 201
#endif



//VARIABLE GLOBALE
struct super_board_t board_1={};
struct gameconfig_t config_1={};

char const* get_player_name(){
	return "Player_1";
}

void initialize(unsigned int player_id, const struct move_t first_move, struct gameconfig_t config) {
	init_super_board(first_move.tile,&board_1);
	config_1=config;
}


struct move_t play(const struct move_t previous_move, const struct tile_t tile){
  add_tile_to_super_board(previous_move.tile, &board_1, previous_move.x, previous_move.y);
  struct move_t current_move={};
  current_move.player_id=1;
  for (int i=0;i<BOARD_SIZE;i++){
    for (int j=0;j<BOARD_SIZE;j++){
      if(compare_tile(board_get(board_1.board, i, j), CARC_TILE_EMPTY)){
	if(is_place_available(board_1.board, i, j, tile)){
	  current_move.x=i;
	  current_move.y=j;
	}
      }
    }
  }
  
  current_move.tile=tile;
  add_tile_to_super_board(current_move.tile, &board_1, current_move.x, current_move.y);
  return current_move;
}

int is_there_a_connection_beetween_tiles(struct board_t *board, struct tile_t tile, struct tile_t tile_to_add){
	for (enum card_point p=0; p<4;p++ ){
		if (tile_check(tile, tile_to_add , p)){
			return 1;
		}
	}
	return 0;
}

/* Clean up the resources the player has been using. Is called once at
   the end of the game.
 * POSTCOND:
 * - every allocation done during the calls to initialize and play
 *   functions must have been freed
 */
void finalize(){
	board_free(board_1.board);
	deck_free(config_1.deck);
	free_super_board(&board_1);
}
