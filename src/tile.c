#include <stdio.h>
#include <stdlib.h>

#include "move.h"
#include "tile.h"
#include "deck.h"
#include "ansi_color.h"

const char * prefix_color_display[9] = {
    BHGRN,
    BHWHT,
    BHBLU,
    BHBLK,
    BHMAG,
    BHCYN,
    BHYEL,
    BHRED,
    COLOR_RESET,
};


int compare_tile(struct tile_t t1, struct tile_t t2)
{
    for (int i = 0; i < MAX_CONNECTIONS-1; i++) {
        if (t1.t[i] != t2.t[i] && t1.t[(i+3)%12] != t2.t[i] && t1.t[(i+6)%12] != t2.t[i] && t1.t[(i+9)%12] != t2.t[i])
            return 0;
        if (t1.c[i] != t2.c[i] && t1.c[(i+3)%12] != t2.c[i] && t1.c[(i+6)%12] != t2.c[i] && t1.c[(i+9)%12] != t2.c[i])
            return 0;
    }
    if (t1.t[12] != t2.t[12] || t1.c[12] != t2.c[12])
        return 0;
    return 1;
}

int tile_check(struct tile_t t0, struct tile_t t1, enum card_point nesw)
{
  switch (nesw) {
  case NORTH:
    for (int i = 0; i < 3; i++) {
        if (t0.c[i] != t1.c[8 - i])
            return 0;
    }
    return 1;
    
  case EAST:
    for (int i = 0; i < 3; i++) {
        if (t0.c[i + 3] != t1.c[11 - i])
            return 0;
    }
    return 1;
    
  case SOUTH:
    for (int i = 0; i < 3; i++) {
        if (t0.c[i + 6] != t1.c[2 - i])
            return 0;
    }
    return 1;
    
  case WEST:
    for (int i = 0; i < 3; i++) {
        if (t0.c[i + 9] != t1.c[5 - i])
            return 0;
    }
    return 1;
  }
  return 0;
}

struct tile_t random_tile() 
{
    struct tile_t tiles[4] = {CARC_TILE_INIT, CARC_TILE_GREEN, CARC_TILE_XROAD, CARC_TILE_TINI};
    return tiles[rand() % 4];
}



const char* prefix_color(enum color_t c)
{
    return prefix_color_display[c];
}

struct tile_t copy_tile(struct tile_t tile)
{
    struct tile_t cpy_tile = {.c={0}, .t={0}};
    for (int i=0; i<13; ++i) {
        cpy_tile.c[i] = tile.c[i];
        cpy_tile.t[i] = tile.t[i];
    }
    return cpy_tile;
}


struct tile_t flip_tile(struct tile_t tile)
{
    struct tile_t new_tile = {.c={0}, .t={0}};
    for (int i=0; i<12; ++i) {
        new_tile.c[(i+3)%12] = tile.c[i];
        new_tile.t[(i+3)%12] = tile.t[i];
    }
    new_tile.c[12] = tile.c[12];
    new_tile.t[12] = tile.t[12];
    return new_tile;
}


int replace_tile(struct tile_t * tile_in, struct tile_t * tile_out)
{
    for (int i=0; i<13; ++i) {
        tile_out->c[i] = tile_in->c[i];
        tile_out->t[i] = tile_in->t[i];
    }
    return 0;
}

void tile_display(struct tile_t tile)
{
    if( compare_tile(CARC_TILE_EMPTY, tile)){
        printf("THE TILE IS EMPTY");
        return ;
    }
    printf("\n");
    printf( "  ");
    for (int i=0; i<3; i++){
        printf( " %sO %s",prefix_color(tile.c[i]),COLOR_RESET  );
    }
    printf( "\n");

    printf( " %sO%s ",prefix_color(tile.c[11]),COLOR_RESET );
    if (tile.c[1]==GRAY){
        printf( "   |");
    }
    else 
        printf( "    ");
    printf( "   ");
    printf( " %sO%s ",prefix_color(tile.c[3]),COLOR_RESET  );
    printf( "\n");
    

    printf(" %sO%s ", prefix_color(tile.c[10]),COLOR_RESET  );
    if (tile.c[10]==GRAY){
        printf( "__");
    }
    else 
        printf( "  ");
    printf( " %sO%s ",prefix_color(tile.c[12]),COLOR_RESET   );
    if (tile.c[4]==GRAY){
        printf( "__");
    }
    else 
        printf( "  ");
    
    printf( " %sO%s ",prefix_color(tile.c[4]),COLOR_RESET   );	
    printf( "\n");

    printf( " %sO%s ",prefix_color(tile.c[9]),COLOR_RESET   );
    if (tile.c[7]==GRAY){
        printf( "   |	");
    }
    else 
        printf( "     ");
    printf( "   %sO%s ",prefix_color(tile.c[5]),COLOR_RESET   );
    printf( "\n");
    printf( "  ");
    for (int i=06; i<9; i++){
        printf(" %sO%s ",prefix_color(tile.c[i]),COLOR_RESET   );
    }
    printf( "\n");
}

void tile_display_with_meeple(struct move_t move){
	tile_display(move.tile);
	if ( move.meeple>13){
		printf(URED"===>There is Meeple in  %d \n"COLOR_RESET, move.meeple);
	}
}
