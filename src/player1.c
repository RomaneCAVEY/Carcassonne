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
int p1_score=0;
char const* get_player_name(){
    return "Player_1";
}

void initialize(unsigned int player_id, const struct move_t first_move, struct gameconfig_t config) {
	init_super_board(first_move.tile,&board_1);
	id_player = player_id;
	config_1=config;
	//board_1.meeple=init_meeple(7);
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
	add_meeple_to_board(&pm, &board_1,config_1.mode);
    struct move_t current_move={};
    int previous_x = previous_move.x;
    int previous_y = previous_move.y;
    printf("Previous move in player : (%d, %d)\n", previous_x, previous_y);
    current_move.player_id=id_player;
	int max=-1;
	current_move.x=previous_x;
    current_move.y=previous_y;
    struct tile_t ptile = copy_tile(tile); 
	struct move_t best_positions[200];
	int nb_max=0;
    for (int i = p1_board_min_x - 1; i < p1_board_max_x + 2; i++) {
        for (int j = p1_board_min_y - 1; j < p1_board_max_y + 2; j++) {
            if(compare_tile(board_get(board_1.board, i, j), CARC_TILE_EMPTY)){
				// try with flip tile :
                for (int flip=0; flip<4; ++flip) {
                    struct tile_t ftile = flip_tile(ptile);
                    replace_tile(&ftile, &ptile);
                    if(board_add_check(board_1.board, ptile, i, j)){
						struct super_board_t copy= copy_super_board(board_1);
						add_tile_to_super_board(ptile, &copy, i, j);
						int score=calculate_points(&copy, config_1.mode, id_player).a;
						free_copy_super_board(&copy);
						if (score >max){
							max=score;
							struct move_t pos={.x=i,.y=-j,.tile=ptile};
							nb_max=1;
							best_positions[0]=pos;
							printf("PLAYER 1 SCORE = %d \n",score);
						}
						if (score == max){
							struct move_t pos={.x=i, .y=-j, .tile=ptile};
							best_positions[nb_max] = pos;
							nb_max += 1;
						}
						current_move.x=i;
    					current_move.y=j;
						current_move.tile=ptile;
                    }
                }
            }
        } 
    }
	if (nb_max!=0){
	int a=rand()%nb_max;
	struct move_t choice= best_positions[a];
	current_move.x=choice.x;
    current_move.y=choice.y;
	current_move.tile=choice.tile;
	}
	else{
		current_move.tile=tile;
	}

	if (current_move.x==previous_x && current_move.y==previous_y){
		printf("=========================No placement found===================");
	}

    tile_display(current_move.tile);
    add_tile_to_super_board(current_move.tile, &board_1, current_move.x, -current_move.y);
	add_meeple(&current_move, &board_1, config_1.mode);
    //create_neato(&board_1, "player1_graph.dot"); // création du .dot
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
	deck_free(config_1.deck);
	free_super_board(&board_1);
}
