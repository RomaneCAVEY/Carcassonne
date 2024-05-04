#ifndef __STRUCT_BOARD_H__
#define __STRUCT_BOARD_H__

#ifndef BOARD_SIZE
#define BOARD_SIZE 201
#endif 

#ifndef BOARD_CENTER
#define BOARD_CENTER 100
#endif

#include "board.h"

struct board_t {
    struct tile_t *tiles[BOARD_SIZE];
};


/*Copy the super_biard
* @param:the super_board
* @return: void
*/
struct super_board_t copy_super_board(struct super_board_t super_board);


#endif