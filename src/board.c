#include "board.h"
#include <igraph.h>

struct board_t init_board(struct tile_t tile)
{
  igraph_t graph;
  struct board_t board; /* TODO: init baord*/

  igraph_empty(&graph, 13, IGRAPH_UNDIRECTED);

  /*TODO: add tile at pos (0, 0) */

  return board;
}

void update_board(struct board_t *board, struct move_t move)
{
  ;
}


void destroy_board(struct board_t *board)
{
  igraph_destroy(board->graph);
}
