#include "move.h"
#include "extended_player.h"
#include "struct_board.h"
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

unsigned int id_player = 0;

int p1_board_min_x = 0;
int p1_board_max_x = 0;
int p1_board_min_y = 0;
int p1_board_max_y = 0;

char const* get_player_name(){
	return "Player_1";
}

void initialize(unsigned int player_id, const struct move_t first_move, struct gameconfig_t config) {
	init_super_board(first_move.tile,&board_1);
	create_dot_igraph1(board_1.graph);
	id_player = player_id;
	config_1=config;
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

struct move_t play(const struct move_t previous_move, const struct tile_t tile)
{
	struct move_t pm = previous_move;
	pm.y = -pm.y;
	add_tile_to_super_board(previous_move.tile, &board_1, previous_move.x, -previous_move.y);
	update_board_bounds(pm);
	struct move_t current_move={};
	int previous_x = previous_move.x;
	int previous_y = previous_move.y;
	printf("Previous move in player : (%d, %d)\n", previous_x, previous_y);
	current_move.player_id=id_player;
	int flag = 0; // 
	for (int i = p1_board_min_x - 1; i < p1_board_max_x + 2; i++) {
		if (flag == 1)
			break;
		for (int j = p1_board_min_y - 1; j < p1_board_max_y + 2; j++) {
			if (flag == 1)
				break;
			// TO DO : check placement of tile (coordonnee)
			if(compare_tile(board_get(board_1.board, i, j), CARC_TILE_EMPTY)){
			  if(board_add_check(board_1.board, tile, i, j)){
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
	// tile_display(current_move.tile);
	// printf("Va l'ajouter au coordonnée (%d, %d)\n", current_move.x, current_move.y);
	add_tile_to_super_board(current_move.tile, &board_1, current_move.x, -current_move.y);
	create_neato(&board_1);
	return current_move;
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
