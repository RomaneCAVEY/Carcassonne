#include "move.h"
#include "extended_player.h"
#include "super_board.h"
#include <stdlib.h>
#include"graph.h"
#include "deck.h"
#include "board.h"
#include "tile.h"


#ifndef BOARD_SIZE
#define BOARD_SIZE 201
#endif



//VARIABLE GLOBALE
struct super_board_t board_2={};
igraph_t graph_2={};
struct gameconfig_t config_2={};

char const* get_player_name(){
	return "Player_2";
}

void initialize(unsigned int player_id, const struct move_t first_move, struct gameconfig_t config) {
	init_super_board(first_move.tile,&board_2);
	graph_2=init_graph(first_move.tile);
	config_2=config;
}


/* Computes next move
 * PARAM:
 * - previous_move: the move from the previous player. If this is the
 *                  first move of the game, `previous_move` is equal to
                    a starting tile played by player_id == -2
 * - tile:          the tile that the player is supposed to play
 * RETURNS:
 * - the next move for the player.
 */



struct move_t play(const struct move_t previous_move, const struct tile_t tile){
	board_add(board_2.board,previous_move.tile,previous_move.x,previous_move.y);
	add_tile_to_graph(tile, graph_2, board_2, previous_move.x, previous_move.y);
	struct move_t current_move={};
	current_move.player_id=2;
	for (int i=0;i<BOARD_SIZE;i++){
		for (int j=0;j<BOARD_SIZE;j++){
			if(compare_tile(board_get(board_2.board, i, j), CARC_TILE_EMPTY)){
				if(is_place_available(board_2.board, i, j, tile)){
						current_move.x=i;
						current_move.y=j;
				}
			}
		}
	}
	

	current_move.tile=tile;
	board_add( board_2.board,current_move.tile,current_move.x,current_move.y);
	add_tile_to_graph(tile, graph_2, board_2, current_move.x, current_move.y);
	return current_move;
}

int is_place_available(struct board_t *board,int i, int j,struct tile_t tile){

	if (!(compare_tile(board_get(board, i-1, j),CARC_TILE_EMPTY) && tile_check(board_get(board, i-1, j), tile, WEST)))
		return 1;
	if (!(compare_tile(board_get(board, i, j+1),CARC_TILE_EMPTY) && tile_check(board_get(board, i-1, j), tile, NORTH)))
		return 1;
	if (!(compare_tile(board_get(board, i, j-1),CARC_TILE_EMPTY) && tile_check(board_get(board, i-1, j), tile, SOUTH)))
		return 1;
	if (!(compare_tile(board_get(board, i+1, j),CARC_TILE_EMPTY) && tile_check(board_get(board, i-1, j), tile, EAST)))
		return 1;
	return 0;

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
	board_free(board_2.board);
	free_graph(graph_2);
	deck_free(config_2.deck);
	free_super_board(&board_2);
	}
