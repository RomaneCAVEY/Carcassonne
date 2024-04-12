#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <getopt.h>
#include <string.h>
#include "deck.h"
#include "board.h"
#include "tile.h"
#include "move.h"

#define DECK_SIZE 100

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
  if (board_add_check(board, move.tile, move.x, move.y))
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
  char player_1_path[] = "./install/player0a.so";
  char player_2_path[] = "./install/player0b.so";

  char opt;
  
  while ((opt = getopt(argc, argv, "m:d")) != -1) {
    printf("%c > %s\n", opt, optarg);
    switch (opt) {
    case 'm':
      game_mode = atoi(optarg);
      break;

    case 'd':
      debug = 1;
      break;

    default:
      printf("Usage: %s [-m game_mode] [-d] path_to_player.so path_to_player.so", argv[0]);
      break;
    }
  }
  if (optind < argc) {
    strcpy(player_1_path, argv[optind]);
    if (debug)
      printf("Custom player 1 provided!\n");
  }
  if (optind+1 < argc) {
    strcpy(player_2_path, argv[optind+1]);
    if (debug)
      printf("Custom player 2 provided!\n");
  }
    

  // temporaire > TODO(implémenter les autres gamemode)
  if (game_mode != NO_MEEPLE) {
    printf("WARNING: Provided game mode is not yet supported.\nDefaulting to NO_MEEPLE game mode.\n");
    game_mode = NO_MEEPLE;
  }

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
  int current_player = 1; // random(0,2);
  struct gameconfig_t config = make_config();
  deck_pos = 0;
  
    
  struct move_t current_move = {.player_id=SERVER, .x=0, .y=0, .tile=deck_get(config.deck, 0), .meeple=NO_CONNECTION};
  struct board_t *board = board_init(current_move.tile);
  
  if (debug) {
    printf("-------\nInitial tile (0, 0):\n");
      tile_display(current_move.tile);
  }

  
  // init pj1
  enum player_color_t pcol0 = (current_player == 0) ? BLACK : WHITE;
  initialize0(pcol0, current_move, copy_config(config));
  
  // init pj2
  enum player_color_t pcol1 = (current_player == 1) ? BLACK : WHITE;
  initialize1(pcol1, current_move, copy_config(config));

  struct tile_t tile;
  
  ///////////// BOUCLE DE JEU //////////////
  while (1){
    current_player = next_player(current_player);
    tile = draw_tile(config.deck);

    if (debug) {
      printf("-------\nNew turn. Current player: %d\nTile to place:\n", current_player);
      tile_display(tile);
    }
    
    if (current_player == 0){
      current_move = play0(current_move, tile);
    }
    else{
      current_move = play1(current_move, tile);
    }

    if (debug)
      printf("Player %d is wants to place the tile at pos (%d, %d)\n", current_player, current_move.x, current_move.y);

    
    if (is_invalid(board, current_move)) {
      if (debug)
	printf("Invalid move!\n");
      break;
    }
    
    if (is_game_over()) {
      printf("Game over!\n");
      break;
    }

    board_add(board, current_move.tile, current_move.x, current_move.y);
    // TODO: calculer les nouveaux points
  }
    
  ///////////// FIN BOUCLE DE JEU //////////////
  printf("==========\nGAME ENDED\n==========\n");
  finalize0();
  finalize1();
  board_free(board);
  deck_free(config.deck);


  ///////////// OUTILISATION DES LIBRAIRIES  //////////////
  dlclose(pj0);
  dlclose(pj1);
  
}
