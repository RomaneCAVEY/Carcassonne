#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <igraph.h>

#include "../score.h"
#include "../super_board.h"
#include "../deck.h"



void test_vector_extract_component()
{
  printf(" • %s", __func__);

  const igraph_integer_t ar[10] = {0, 1, 2, 0, 2, 2, 0, 1, 0, 0};
  igraph_vector_int_t v;
  igraph_vector_int_init_array(&v, ar, 10);
  
  int vertices[10];

  assert(vector_extract_component(v, 0, vertices) == 5);
  assert(vertices[0] == 0);
  assert(vertices[1] == 3);
  assert(vertices[2] == 6);
  assert(vertices[3] == 8);
  assert(vertices[4] == 9);

  assert(vector_extract_component(v, 1, vertices) == 2);
  assert(vertices[0] == 1);
  assert(vertices[1] == 7);

  assert(vector_extract_component(v, 2, vertices) == 3);
  assert(vertices[0] == 2);
  assert(vertices[1] == 4);
  assert(vertices[2] == 5);

  assert(vector_extract_component(v, 3, vertices) == 0);
  assert(vector_extract_component(v, 4, vertices) == 0);
  assert(vector_extract_component(v, -12, vertices) == 0);

  igraph_vector_int_destroy(&v);

  printf("\t\e[1;102m SUCCESS \e[0m\n");
}

void test_count_center_vertices()
{
  printf(" • %s", __func__);

  int c[6] = {1, 7, 12, 14, 20, 25};

  assert(count_center_vertices(c, 3) == 1);
  assert(count_center_vertices(c, 6) == 2);

  c[2] = 11;

  assert(count_center_vertices(c, 3) == 0);
  assert(count_center_vertices(c, 6) == 1);
  
  printf("\t\e[1;102m SUCCESS \e[0m\n");  
}

void test_caculate_points()
{
  printf(" • %s", __func__);

  struct super_board_t sb;
  init_super_board(CARC_TILE_INIT, &sb);
  assert(add_tile_to_super_board(CARC_TILE_XROAD, &sb, 1, 0) == 1);
  assert(add_tile_to_super_board(CARC_TILE_XROAD, &sb, -1, 0) == 1);
  assert(add_tile_to_super_board(CARC_TILE_TINI, &sb, 0, -1) == 1);

  /** Note - The board looks like this:
   *
   *              G G G
   *             G     G
   *             R  R  R
   *             G     G
   *              C C C
   *      G R G   C C C  G R G
   *     G     G G     GG     G
   *     R  O  R R  R  RR  O  R
   *     G     G G     GG     G
   *      G R G   G G G  G R G
   *
   * We have 2 finished components: the horizontal road that spans from (-1, 0)
   * to (1, 0) and the castle between tiles (0, 0) and (0, -1).
   *
   * We should gain 3*8 + 3*4 = 36 points
   */

  int points = calculate_points(&sb);
  //printf("points: %d\n", points);
  assert(points == 6); // TODO(): Update with correct amount of points

  free_super_board(&sb);
  
  printf("\t\e[1;102m SUCCESS \e[0m\n");  
}

int score_tests()
{
  printf("\n\e[30;47mFile %s\e[0m\n", __FILE__);
  
  test_vector_extract_component();
  test_count_center_vertices();
  test_caculate_points();
	
  return 0;
}
