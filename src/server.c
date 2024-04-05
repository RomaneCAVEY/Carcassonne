#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

char const* get_player_name1();
void initialize1(unsigned int, const struct move_t, struct gameconfig_t);
struct move_t play1(const struct move_t, const struct tile_t );
void finalize1();

char const* get_player_name2();
void initialize2(unsigned int, const struct move_t, struct gameconfig_t);
struct move_t play2(const struct move_t, const struct tile_t );
void finalize2();

int main(){
  
  void *pj0 = dlopen("./playerRomane", RTLD_LAZY);
  get_player_name0 = dlsym(pj0, "get_player_name");
  initialize0 =dlsym(pj0, "initialize");
  play0 = dlsym(pj0, "play");
  finalize0 = dlsym(pj0, "finalize");

  void *pj1 = dlopen("./playerRomane", RTLD_LAZY);
  get_player_name1 = dlsym(pj1, "get_player_name");
  initialize1 =dlsym(pj1, "initialize");
  play1 = dlsym(pj1, "play");
  finalize1 = dlsym(pj1, "finalize");

  ///////////// BOUCLE DE JEU //////////////

  int start_player = 1; // random(0,2);
  struct gameconfig_t gameconfig = make_config();

  struct move_t first_move = make_move(config.deck.first_card);

  // init pj1
  enum player_color_t pcol1 = (start_player == 0) ? BLACK : WHITE;
  initialize0(pcol, first_move, gameconfig);
  
  // init pj2
  enum player_color_t pcol = (start_player == 1) ? BLACK : WHITE;
  initialize1(pcol, first_move, gameconfig);
  
  current_move = first_move;
  while (1){
    current_player = next_player(current_player);
    tile = draw_tile(gameconfig.deck);
    
    if (current_player == 0){
      current_move = play0(current_move, tile);
    }
    else{
      current_move = play1(current_move, tile);
    }

    
    if is_invalid(current_move){
	break;
      }

    if is_game_over(){
	break;
      }
  }
  

  finalize0();
  finalize1();
  
  ///////////// FIN BOUCLE DE JEU //////////////
  dlclose(pj1);
  dlclose(pj2);
  
}
