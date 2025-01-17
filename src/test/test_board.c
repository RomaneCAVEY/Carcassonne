#include "../struct_board.h"
#include "../tile.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void test_board_init()
{
  printf(" • %s", __func__);
  
    struct board_t *board = board_init(CARC_TILE_INIT);

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (i == BOARD_CENTER && j == BOARD_CENTER)
                assert(compare_tile(board->tiles[i][j], CARC_TILE_INIT));
            else
                assert(compare_tile(board->tiles[i][j], CARC_TILE_EMPTY));
        }
    }

    board_free(board);

    printf("\t\e[1;102m SUCCESS \e[0m\n");
}

void test_board_add()
{
  printf(" • %s", __func__);
  
    struct board_t *board = board_init(CARC_TILE_INIT);
    assert(board_add(board, CARC_TILE_GREEN, 1,0));

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (i == BOARD_CENTER && j == BOARD_CENTER)
                assert(compare_tile(board->tiles[j][i], CARC_TILE_INIT));
            else if (i == BOARD_CENTER + 1 && j == BOARD_CENTER + 0)
                assert(compare_tile(board->tiles[j][i], CARC_TILE_GREEN));
            else
                assert(compare_tile(board->tiles[j][i], CARC_TILE_EMPTY));
        }
    }

    board_free(board);

    printf("\t\e[1;102m SUCCESS \e[0m\n");
}


void test_board_add_check_specific(){
  printf(" • %s", __func__);
  
  struct board_t *board = board_init(CARC_TILE_INIT);
  assert(board_add_check(board, CARC_TILE_INIT, 1, 0));
  assert(board_add(board, CARC_TILE_INIT, 1, 0));
  assert(board_add_check(board, CARC_TILE_INIT, 2, 0));

  board_free(board);
  
  printf("\t\e[1;102m SUCCESS \e[0m\n");
}




void test_board_add_check()
{
  printf(" • %s", __func__);
  
  struct board_t *board = board_init(CARC_TILE_INIT);
  assert(board_add(board, CARC_TILE_INIT, 1, 0));
  
  assert(board_add_check(board, CARC_TILE_GREEN, 3,1) ==  0);
  assert(board_add_check(board, CARC_TILE_GREEN, 2,1) == 0);
  assert(board_add_check(board, CARC_TILE_GREEN, 0,0) == 0);
  assert(board_add_check(board, CARC_TILE_GREEN, 0,1) == 1);

  board_free(board);
  
  printf("\t\e[1;102m SUCCESS \e[0m\n");
}

void test_board_add_check_specific2()
{
  printf(" • %s", __func__);
  
  struct board_t *board = board_init(CARC_TILE_INIT);
  assert(board_add(board, CARC_TILE_XROAD, 1, 0));
  assert(board_add(board, CARC_TILE_GREEN, 0, 1));
  
  assert(board_add_check(board, CARC_TILE_XROAD, 1, 1) == 0);

  board_free(board);
  
  printf("\t\e[1;102m SUCCESS \e[0m\n");  
}

void test_board_get()
{
  printf(" • %s", __func__);
  
  struct board_t *board = board_init(CARC_TILE_INIT);
  board_add(board, CARC_TILE_GREEN, 1, 1);
  board_add(board, CARC_TILE_GREEN, 2, 1);
  
  assert(compare_tile(board_get(board, 2, 1), CARC_TILE_GREEN));
  assert(compare_tile(board_get(board, 3, 3), CARC_TILE_EMPTY));

  board_free(board);
  
  printf("\t\e[1;102m SUCCESS \e[0m\n");
}
/*
void test_board_free(){

  printf("%s", __func__);

  struct board_t* board = board_init(CARC_TILE_INIT);
  board_add(board,CARC_TILE_GREEN , 1, 1 );
  board_add(board,CARC_TILE_GREEN , 2, 1 );

  board_free(board);

  assert( board == NULL );

  printf("\t\033[34;01mSUCCESS\033[00m\n");
  }*/

int board_tests()
{
  printf("\n\e[30;47mFile %s\e[0m\n", __FILE__);
  
  test_board_init();
  test_board_add();
  test_board_add_check();
  test_board_get();
  test_board_add_check_specific();
  test_board_add_check_specific2();
	
  return 0;
}
