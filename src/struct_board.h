#ifndef __STRUCT_BOARD_H__
#define __STRUCT_BOARD_H__

#ifndef BOARD_SIZE
#define BOARD_SIZE 201
#endif 

#ifndef BOARD_CENTER
#define BOARD_CENTER 101
#endif

#include "board.h"

struct board_t {
    struct tile_t *tiles[BOARD_SIZE];
};


#endif