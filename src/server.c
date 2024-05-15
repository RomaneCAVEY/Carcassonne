#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <getopt.h>
#include <string.h>
#include <time.h>
#include "deck.h"
#include "board.h"
#include "meeple.h"
#include "super_board.h"
#include "score.h"
#include "tile.h"
#include "move.h"

#define DECK_SIZE 15

char const* (*get_player_name0)();
void (*initialize0)(unsigned int, const struct move_t, struct gameconfig_t);
struct move_t (*play0)(const struct move_t, const struct tile_t );
void (*finalize0)();

char const* (*get_player_name1)();
void (*initialize1)(unsigned int, const struct move_t, struct gameconfig_t);
struct move_t (*play1)(const struct move_t, const struct tile_t);
void (*finalize1)();

int deck_pos;
struct gameconfig_t make_config() {
  struct gameconfig_t cfg = {
    .mode = NO_MEEPLE,
    .meeples = 0,
    .deck = deck_empty(DECK_SIZE)
  };

  deck_add(cfg.deck, CARC_TILE_INIT);
  for (int i = 1; i < DECK_SIZE; i++) {
    deck_add(cfg.deck, random_tile());
  }

  return cfg;
}

struct gameconfig_t copy_config(struct gameconfig_t template) {
  struct gameconfig_t cfg = {
    .mode = template.mode,
    .meeples = template.meeples,
    .deck = deck_copy(template.deck)
  };
  return cfg;
}

int next_player(int current_player) {
  return (current_player + 1) % 2;
}

struct tile_t draw_tile(const struct deck_t* d) {
  struct tile_t t = deck_get(d, deck_pos);
  deck_pos++;
  return t;
}

int is_invalid(struct board_t *board, struct move_t move){
  if (board_add_check(board, move.tile, move.x, -move.y)) // y axis is inverted in our implementation
      return 0;
  return 1;
}

int is_game_over() {
  if (deck_pos + 1 >= DECK_SIZE)
    return 1;
  return 0;
}

int main(int argc, char *argv[]) {

  ///////////// COMMAND LINE OPTIONS //////////////
  int debug = 0;
  enum gamemode_t game_mode = NO_MEEPLE;
  char p1def[] = "./install/player1.so";
  char p2def[] = "./install/player2.so";
  char *player_1_path = p1def;
  char *player_2_path = p2def;

  int seed = time(NULL);

  int opt;
  
  while ((opt = getopt(argc, argv, "m:s:d")) != -1) {
    printf("%c > %s\n", opt, optarg);
    switch (opt) {
    case 'm':
      game_mode = atoi(optarg);
      break;

    case 's':
      seed = atoi(optarg);
      break;

    case 'd':
      debug = 1;
      break;

    default:
      printf("Usage: %s [-s seed] [-m game_mode] [-d] path_to_player.so path_to_player.so", argv[0]);
      break;
    }
  }
  if (optind < argc) {
    player_1_path = argv[optind];
    if (debug)
      printf("[server] Custom player 1 provided!\n");
  }
  if (optind+1 < argc) {
    player_2_path = argv[optind+1];
    if (debug)
      printf("[server] Custom player 2 provided!\n");
  }
    

	//printf("GAME MODE : %d", game_mode);
  ///////////// CHARGEMENT DES LIBRAIRIES //////////////
  void *pj0 = dlopen(player_1_path, RTLD_LAZY);
  get_player_name0 = dlsym(pj0, "get_player_name");
  initialize0 =dlsym(pj0, "initialize");
  play0 = dlsym(pj0, "play");
  finalize0 = dlsym(pj0, "finalize");

  void *pj1 = dlopen(player_2_path, RTLD_LAZY);
  get_player_name1 = dlsym(pj1, "get_player_name");
  initialize1 =dlsym(pj1, "initialize");
  play1 = dlsym(pj1, "play");
  finalize1 = dlsym(pj1, "finalize");


  ///////////// INITIALISATION DE LA PARTIE //////////////
  printf("[server] Game seed: %d\n", seed);
  srand(seed);
  int current_player = rand() % 2;
  struct gameconfig_t config = make_config();
  config.mode = game_mode;
  deck_pos = 0;
  
    
  struct move_t current_move = {.player_id=SERVER, .x=0, .y=0, .tile=deck_get(config.deck, 0), .meeple=NO_CONNECTION};
  struct super_board_t super_board;
  init_super_board(current_move.tile, &super_board);

  struct meeple_t meeple=init_meeple(7);
  
  if (debug) {
    printf("-------\nInitial tile (0, 0):\n");
    tile_display(current_move.tile);
	//tile_display_with_meeple(current_move);
  }

  
  // init pj1
  enum player_color_t pcol0 = (current_player == 0) ? BLACK : WHITE;
  struct gameconfig_t cfg2 = copy_config(config);
  initialize0(pcol0, current_move, cfg2);
  
  // init pj2
  enum player_color_t pcol1 = (current_player == 1) ? BLACK : WHITE;
  initialize1(pcol1, current_move, copy_config(config));

  struct tile_t tile;
  struct int_pair_t points = {.a = 0, .b = 0};
  struct int_pair_t new_points;
  
  ///////////// BOUCLE DE JEU //////////////
  while (1){
    tile = draw_tile(config.deck);

    if (debug) {
      if (current_player == 0) {
    printf("-------\n[server] New turn. Current player: %s\n[server] Tile to place:\n", get_player_name0());
      } else {
    printf("-------\n[server] New turn. Current player: %s\n[server] Tile to place:\n", get_player_name1());
      }
      //tile_display(tile);
    }
    printf("[server] Previous move: (%d, %d)\n", current_move.x, current_move.y);
    if (current_player == 0){
      current_move = play0(current_move, tile);
      if (!compare_tile(tile, current_move.tile)) {
        if (debug)
          printf("[server] %s try to cheat !\n", get_player_name0());
        break;
      }
    }
    else{
      current_move = play1(current_move, tile);
      if (!compare_tile(tile, current_move.tile)) {
        if (debug)
          printf("[server] %s try to cheat !\n", get_player_name1());
        break;
      }
    }

    if (debug)
      printf("[server] Current player wants to place the tile at pos (%d, %d)\n", current_move.x, current_move.y);

    
    if (is_invalid(super_board.board, current_move)) {
      if (debug)
        printf("[server] Invalid move!\n");
      break;
    }
    //tile_display_with_meeple(current_move);
    
    add_tile_to_super_board(current_move.tile, &super_board, current_move.x, -current_move.y); // y axis is inverted in our implementation

    if(game_mode != NO_MEEPLE){
      if (add_meeple_to_board(&current_move, &super_board, game_mode) == 0) {
	if (debug)
	  printf("[server] Player tried to place meeple at invalid pos (%d)\n", current_move.meeple);
	break;
      }
    }

    
    // Calculate points
    new_points = calculate_points(&super_board, config.mode, current_player);
    points.a = points.a + new_points.a;
    points.b = points.b + new_points.b;

    if (debug)
      printf("[server] New points for %s: %d (total %d); New points for %s: %d (total %d)", get_player_name0(), new_points.a, points.a, get_player_name1(), new_points.b, points.b);

    if (is_game_over()) {
      printf("[server] Game over!\n");
      break;
    }

    current_player = next_player(current_player);
  }
    
  ///////////// FIN BOUCLE DE JEU //////////////
  printf("==========\nGAME ENDED\n==========\n");
  printf("\n---[SCORE]---\n");
  printf("- %s: %d\n", get_player_name0(), points.a);
  printf("- %s: %d\n", get_player_name1(), points.b);
  
  // create_neato(&super_board, "server_graph.dot");

  if (is_game_over()) {
    if (points.a > points.b)
      printf("\n\e[0;30;107m %s won the game! \e[0m\n\n", get_player_name0());
    else if (points.a < points.b)
      printf("\n\e[0;30;107m %s won the game! \e[0m\n\n", get_player_name1());
    else
      printf("\n\e[0;30;107m Game ended with a tie! \e[0m\n\n");
  } else {
    if (current_player == 0)
      printf("\n\e[0;30;107m %s won the game! (%s made an invalid move) \e[0m\n\n", get_player_name1(), get_player_name0());
    else
      printf("\n\e[0;30;107m %s won the game! (%s made an invalid move) \e[0m\n\n", get_player_name0(), get_player_name1());
  }
  
  finalize0();
  finalize1();
  //board_free(super_board.board);
  free_super_board(&super_board);
  deck_free(config.deck);
  free_meeple(meeple);


  ///////////// OUTILISATION DES LIBRAIRIES  //////////////
  dlclose(pj0);
  dlclose(pj1);
  
}