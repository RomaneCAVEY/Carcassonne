#include "board.h"
#include "tile.h"
#include <stdio.h>

#define BOARD_SIZE 201
#define BOARD_CENTER 101

struct board_t {
    struct tile_t *tiles[BOARD_SIZE];
};

struct board_t *board_init(struct tile_t tile)
{
    struct board_t *board = malloc(sizeof(struct board_t));

    for (int i = 0; i < BOARD_SIZE; i++) {
        board->tiles[i] = malloc(sizeof(struct tile_t) * BOARD_SIZE);
        for (int j = 0; j < BOARD_SIZE; j++) {
            board->tiles[i][j] = CARC_TILE_EMPTY;
        }
    }

    board->tiles[BOARD_CENTER][BOARD_CENTER] = tile;

    return board;
}

int board_add(struct board_t *board, struct tile_t tile, int x, int y)
{
    if (x + BOARD_CENTER < 0 || y + BOARD_CENTER < 0
        || x + BOARD_CENTER >= BOARD_SIZE || y + BOARD_CENTER >= BOARD_SIZE
        || compare_tile(board->tiles[BOARD_CENTER + y][BOARD_CENTER + x],
               CARC_TILE_EMPTY)
            == 0)
        return 0;

    board->tiles[BOARD_CENTER + y][BOARD_CENTER + x] = tile;

    return 1;
}

int board_add_check(struct board_t *board, struct tile_t tile, int x, int y)
{
    if (compare_tile(
            board->tiles[BOARD_CENTER + y][BOARD_CENTER + x], CARC_TILE_EMPTY)
        == 0)
        return 0;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i != j && i != 2 + j && i != j - 2
                && compare_tile(
                       board->tiles[BOARD_CENTER + y + i][BOARD_CENTER + x + j],
                       CARC_TILE_EMPTY)
                    == 0
                && tile_check(tile,
                    board->tiles[BOARD_CENTER + y + i][BOARD_CENTER + x + j],
                    i - 2 * j))
                return 0;
        }
    }

    return 1;
}

struct tile_t board_get(struct board_t *board, int x, int y)
{
    if (x + BOARD_CENTER < 0 || y + BOARD_CENTER < 0
        || x + BOARD_CENTER >= BOARD_SIZE || y + BOARD_CENTER >= BOARD_SIZE)
        return CARC_TILE_EMPTY;
    return board->tiles[BOARD_CENTER + y][BOARD_CENTER + x];
}

void board_free(struct board_t *board)
{
    for (int i = 0; i < BOARD_SIZE; i++) {
        free(board->tiles[i]);
    }
    free(board);
}

void board_display(struct board_t *board){
	for (int i=0; i<BOARD_SIZE;i++){
		for (int j=0;j<BOARD_SIZE;j++){
			if (!compare_tile(CARC_TILE_EMPTY, board_get(board,i,j))){
				tile_display(board_get(board,i,j));
				printf("			(%d,%d) \n",i,j);
			}
		}
	}
}
