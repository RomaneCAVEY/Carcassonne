#ifndef __TILE_H__
#define __TILE_H__

#include "move.h"

enum card_point{
  NORTH,
  EAST,
  SOUTH,
  WEST,
};

int compare_tile(struct tile_t t1, struct tile_t t2);

int tile_check(struct tile_t t0, struct tile_t t1, enum card_point nesw);

struct tile_t random_tile();

#endif //__TILE_H__
