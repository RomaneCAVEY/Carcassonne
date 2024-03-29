#include "../board.c"
#include "../board.h"
#include "../tile.c"

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

int main()
{
  test_compare_tile();
  test_tile_check();

    return 0;
}
