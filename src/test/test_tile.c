
#include "../board.h"
#include "../tile.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


#ifndef BOARD_SIZE
#define BOARD_SIZE 201
#endif 

#ifndef BOARD_CENTER
#define BOARD_CENTER 101
#endif

void test_compare_tile()
{
  printf(" • %s", __func__);

  assert(compare_tile(CARC_TILE_INIT, CARC_TILE_INIT) == 1);
  assert(compare_tile(CARC_TILE_GREEN, CARC_TILE_GREEN) == 1);
  assert(compare_tile(CARC_TILE_EMPTY, CARC_TILE_EMPTY) == 1);

  assert(compare_tile(CARC_TILE_EMPTY, CARC_TILE_GREEN) == 0);
  assert(compare_tile(CARC_TILE_GREEN, CARC_TILE_INIT) == 0);
  assert(compare_tile(CARC_TILE_EMPTY, CARC_TILE_INIT) == 0);

  printf("\t\e[1;102m SUCCESS \e[0m\n");
}

void test_tile_check()
{
  printf(" • %s", __func__);

  assert(tile_check(CARC_TILE_INIT,CARC_TILE_GREEN, NORTH) == 0);
  assert(tile_check(CARC_TILE_INIT,CARC_TILE_GREEN, EAST) == 0);
  assert(tile_check(CARC_TILE_INIT,CARC_TILE_GREEN, SOUTH));
  assert(tile_check(CARC_TILE_INIT,CARC_TILE_GREEN,WEST) == 0);

  assert(tile_check(CARC_TILE_INIT,CARC_TILE_XROAD,EAST));

  printf("\t\e[1;102m SUCCESS \e[0m\n");
}

void test_tile_display(){
  printf(" • %s\n", __func__);
  
  struct tile_t tile_all_routes={
    .t={0,0,0,1,2,3,3,3,3,3,2,1,2},
    .c={BROWN,GRAY,BROWN,GREEN,GRAY,GREEN,
      GREEN,GRAY,GREEN,GREEN,GRAY,GREEN,GRAY}
  } ;
  
  tile_display(tile_all_routes);
  printf("\n");
  
  struct tile_t tile_without_routes={
    .t={0,0,0,1,2,3,3,3,3,3,2,1,2},
    .c={BROWN,GREEN,BROWN,GREEN,GREEN,GREEN,
      GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN}
  } ;

  tile_display(tile_without_routes);

  printf("\n\e[1;103m UNKNOWN RESULT \e[0m\n");
} 


int tile_tests()
{
  printf("\n\e[30;47mFile %s\e[0m\n", __FILE__);
  test_compare_tile();
  test_tile_check();
  test_tile_display();
  
  return 0;
}
