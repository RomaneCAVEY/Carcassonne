#ifndef __TILE_H__
#define __TILE_H__

#include "move.h"

enum card_point{
  NORTH = -2,
  EAST = 1,
  SOUTH = 2,
  WEST = -1,
};


// return 1 if the tiles are the same, else 0
int compare_tile(struct tile_t t1, struct tile_t t2);


int tile_check(struct tile_t t0, struct tile_t t1, enum card_point nesw);


//create a random tile
struct tile_t random_tile();

//display the tile
void tile_display(struct tile_t tile);


#endif //__TILE_H__
