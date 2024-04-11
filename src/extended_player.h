#include "board.h"
#include "player.h"
#include "tile.h"




/*Return 1 if we wan play, else 0
* @param: the board, and the coordonate of a neighburght tile, the tile we want to play
* @return: int , 1 if we are allowed to put the tile here, else 0
*/
int is_place_available(struct board_t *board,int i, int j,struct tile_t tile);


/*Return 1 if we wan play, else 0
* @param: the board, the tile we want to play , a tile neighbourgth  of the tile
* @return: int , 1 if we are allowed to put the tile here ie if there are connections between the 2 tiles, else 0
*/
int is_there_a_connection_beetween_tiles(struct board_t *board, struct tile_t tile, struct tile_t tile_to_add);