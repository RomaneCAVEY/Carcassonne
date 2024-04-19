#include "move.h"
#include "extended_player.h"
#include "struct_board.h"
#include "board.h"
#include <stdlib.h>
#include "graph.h"
#include "deck.h"
#include "board.h"
#include "super_board.h"
#include "meeple.h"

#ifndef BOARD_SIZE
#define BOARD_SIZE 201
#endif



//VARIABLE GLOBALE
struct super_board_t board_2={};
struct gameconfig_t config_2={};
struct meeple_t meeple_player2={};



int p1_board_min_x = 0;
int p1_board_max_x = 0;
int p1_board_min_y = 0;
int p1_board_max_y = 0;

char const* get_player_name(){
	return "Player_2";
}

void initialize(unsigned int player_id, const struct move_t first_move, struct gameconfig_t config) {
	init_super_board(first_move.tile, &board_2);
	meeple_player2=init_meeple(7);
	create_dot_igraph2((board_2.graph));
	config_2 = config;
}


void update_board_bounds(struct move_t move) {
  if (move.x > p1_board_max_x && p1_board_max_x < BOARD_CENTER - 1)
    p1_board_max_x = move.x;

  if (move.y > p1_board_max_y && p1_board_max_y < BOARD_CENTER - 1)
    p1_board_max_y = move.y;

  if (move.x < p1_board_min_x && p1_board_min_x > 2 - BOARD_CENTER)
    p1_board_min_x = move.x;

  if (move.y < p1_board_min_y && p1_board_min_y > 2 - BOARD_CENTER)
    p1_board_min_y = move.y;
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
struct move_t play(const struct move_t previous_move, const struct tile_t tile)
{
	struct move_t pm = previous_move;
	pm.y = -pm.y;
	add_tile_to_super_board(previous_move.tile, &board_2, previous_move.x, -previous_move.y);
	
	update_board_bounds(pm);
	add_meeple(&meeple_player2, previous_move);
	struct move_t current_move={};
	current_move.meeple= (13*(board_2.size)+rand()%13); 
	int previous_x = previous_move.x;
	int previous_y = previous_move.y;
	printf("Previous move in player : (%d, %d)\n", previous_x, previous_y);
	current_move.player_id=2;
	int flag = 0; // 
	for (int i = p1_board_min_x - 1; i < p1_board_max_x + 2; i++) {
		if (flag == 1)
			break;
		for (int j = p1_board_min_y - 1; j < p1_board_max_y + 2; j++) {
			if (flag == 1)
				break;
			// TO DO : check placement of tile (coordonnee)
			if(compare_tile(board_get(board_2.board, i, j), CARC_TILE_EMPTY)){
				if(is_place_available(board_2.board, i, j, tile)){
					printf("----------- Place trouvé ! ----------- (%d, %d)\n", i, -j);
					current_move.x=i;
					current_move.y=-j;
					flag = 1;
				}
			}
			if (i == (BOARD_SIZE-1) && j == (BOARD_SIZE-1)) {
				printf("No placment found !\n");
				current_move.x=previous_x;
				current_move.y=previous_y;
			}
		}
	}
	current_move.tile=tile;
	

	add_meeple(&meeple_player2, current_move);
	// tile_display(current_move.tile);
	// printf("Va l'ajouter au coordonnée (%d, %d)\n", current_move.x, current_move.y);
	add_tile_to_super_board(current_move.tile, &board_2, current_move.x, -current_move.y);
	create_dot_igraph2(board_2.graph);
	return current_move;
}

int is_place_available(struct board_t *board,int i, int j,struct tile_t tile){

	if (!compare_tile(board_get(board, i-1, j),CARC_TILE_EMPTY) && !tile_check(board_get(board, i-1, j), tile, WEST)) {
		// printf("Can't place here : %d, %d\n", i, j);
		return 0;
	}
	if (!compare_tile(board_get(board, i, j+1),CARC_TILE_EMPTY) && !tile_check(board_get(board, i, j+1), tile, NORTH)) {
		// printf("Can't place here : %d, %d\n", i, j);
		return 0;
	}
	if (!compare_tile(board_get(board, i, j-1),CARC_TILE_EMPTY) && !tile_check(board_get(board, i, j-j), tile, SOUTH)){
		// printf("Can't place here : %d, %d\n", i, j);
		return 0;
	}
	if (!compare_tile(board_get(board, i+1, j),CARC_TILE_EMPTY) && !tile_check(board_get(board, i+1, j), tile, EAST)){
		// printf("Can't place here : %d, %d\n", i, j);
		return 0;
	}
	if (compare_tile(board_get(board, i-1, j),CARC_TILE_EMPTY) && 
	compare_tile(board_get(board, i+1, j),CARC_TILE_EMPTY) && 
	compare_tile(board_get(board, i, j-1),CARC_TILE_EMPTY) &&
	compare_tile(board_get(board, i, j+1),CARC_TILE_EMPTY)) {
		// printf("Can't place here : %d, %d\n", i, j);
		return 0;
	}
	return 1;

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
void finalize()
{
	board_free(board_2.board);
	deck_free(config_2.deck);
	free_super_board(&board_2);
	free_meeple(meeple_player2);
}
