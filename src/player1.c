#include <stdio.h>
#include <stdlib.h>

#include "move.h"
#include "extended_player.h"
#include "struct_board.h"
#include "graph.h"
#include "deck.h"
#include "board.h"
#include "super_board.h"
#include "meeple.h"
#include "common.h"
#include "score.h"


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
	board_1.meeple=init_meeple(7);
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
	//add_meeple_to_board( &pm, board_1, config_1.mode);
	update_board_bounds(pm);
	add_meeple(&pm, board_1,  config_1.mode);

	struct move_t current_move={};
	//printf("QJFAÙOIHZF TAILLE DU BOARD %d \n\n", board_1.size);
	current_move.meeple= (13*(board_1.size)+rand()%13); 

	int previous_x = previous_move.x;
	int previous_y = previous_move.y;
	printf("Previous move in player : (%d, %d)\n", previous_x, previous_y);
	current_move.player_id=id_player;
	int flag = 0; // 
	int max=0;
	struct int_pair_t coordonnate_max={previous_x,previous_y};
	for (int i = p1_board_min_x - 1; i < p1_board_max_x + 2; i++) {
		if (flag == 1)
			break;
		for (int j = p1_board_min_y - 1; j < p1_board_max_y + 2; j++) {
			if (flag == 1)
				break;
			// TO DO : check placement of tile (coordonnee)
			if(compare_tile(board_get(board_1.board, i, j), CARC_TILE_EMPTY)){
				 if(board_add_check(board_1.board, tile, i, j)){
					struct super_board_t copie_super_board= copy_super_board(board_1);
					add_tile_to_super_board(current_move.tile, &copie_super_board, i, j);
					int score=calculate_points(&copie_super_board, config_1.mode, id_player).b;
					free_super_board(&copie_super_board);
					if (score> max && (board_add_check(board_1.board, tile, i, j))){
						max=score;
						coordonnate_max.a=i;
						coordonnate_max.b=j;	
					}
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
	current_move.x=coordonnate_max.a;
	current_move.y=coordonnate_max.b;

	current_move.tile=tile;
	// tile_display(current_move.tile);
	// printf("Va l'ajouter au coordonnée (%d, %d)\n", current_move.x, current_move.y);
	add_tile_to_super_board(current_move.tile, &board_1, current_move.x, -current_move.y);
	add_meeple(&current_move, board_1, config_1.mode);
	//add_meeple_to_board(&current_move, board_1,config_1.mode);
	//create_neato(&board_1);
	return current_move;
}

// 

/* 
int is_there_a_connection_beetween_tiles(struct board_t *board, struct tile_t tile, struct tile_t tile_to_add){
	for (enum card_point p=0; p<4;p++ ){
		if (tile_check(tile, tile_to_add , p)){
			return 1;
		}
	}
	return 0;
}
 */


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
	//free_meeple(board_1.meeple);
}