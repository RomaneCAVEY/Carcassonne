
#include "../board.h"
#include "../tile.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void test_compare_tile()
{
  printf("%s", __func__);

  assert(compare_tile(CARC_TILE_INIT, CARC_TILE_INIT) == 1);
  assert(compare_tile(CARC_TILE_GREEN, CARC_TILE_GREEN) == 1);
  assert(compare_tile(CARC_TILE_EMPTY, CARC_TILE_EMPTY) == 1);

  assert(compare_tile(CARC_TILE_EMPTY, CARC_TILE_GREEN) == 0);
  assert(compare_tile(CARC_TILE_GREEN, CARC_TILE_INIT) == 0);
  assert(compare_tile(CARC_TILE_EMPTY, CARC_TILE_INIT) == 0);

  printf("\t\033[32;01mSUCCESS\033[00m\n");
}

void test_tile_check()
{
  printf("%s", __func__);

  assert(tile_check(CARC_TILE_INIT,CARC_TILE_GREEN, NORTH) == 0);
  assert(tile_check(CARC_TILE_INIT,CARC_TILE_GREEN, EAST) == 0);
  assert(tile_check(CARC_TILE_INIT,CARC_TILE_GREEN, SOUTH));
  assert(tile_check(CARC_TILE_INIT,CARC_TILE_GREEN,WEST) == 0);

  assert(tile_check(CARC_TILE_INIT,CARC_TILE_XROAD,EAST));

  printf("\t\033[32;01mSUCCESS\033[00m\n");

}
void test_tile_display(struct tile_t tile){
	tile_display(tile);
} 


int main()
{
  test_compare_tile();
  test_tile_check();
  struct tile_t tile_all_routes={
  .t={0,0,0,1,2,3,3,3,3,3,2,1,2},
  .c={BROWN,GRAY,BROWN,GREEN,GRAY,GREEN,
      GREEN,GRAY,GREEN,GREEN,GRAY,GREEN,GRAY}
} ; 
  test_tile_display(tile_all_routes);
	struct tile_t tile_without_routes={
  .t={0,0,0,1,2,3,3,3,3,3,2,1,2},
  .c={BROWN,GREEN,BROWN,GREEN,GREEN,GREEN,
      GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN}
} ; 
printf("\n");
  test_tile_display(tile_without_routes);
    return 0;
}
