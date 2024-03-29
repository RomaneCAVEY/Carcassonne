#include <stdio.h>
#include <stdlib.h>

#include "tile.h"

int compare_tile(struct tile_t t1, struct tile_t t2)
{
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        if (t1.t[i] != t2.t[i])
            return 0;
        if (t1.c[i] != t2.c[i])
            return 0;
    }
    return 1;
}

int tile_check(struct tile_t t0, struct tile_t t1, enum card_point nesw)
{
  switch (nesw) {
  case NORTH:
    for (int i = 0; i < 3; i++) {
      if (t0.c[i] != t1.c[8 - i])
	  return 0;
    }
    return 1;
    
  case EAST:
    for (int i = 0; i < 3; i++) {
      if (t0.c[i + 3] != t1.c[11 - i])
	return 0;
    }
    return 1;
    
  case SOUTH:
    for (int i = 0; i < 3; i++) {
      if (t0.c[i + 6] != t1.c[2 - i])
	return 0;
    }
    return 1;
    
  case WEST:
    for (int i = 0; i < 3; i++) {
      if (t0.c[i + 9] != t1.c[5 - i])
	return 0;
    }
    return 1;
  }
  return 0;
}
