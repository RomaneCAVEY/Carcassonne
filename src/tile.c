#include <stdio.h>
#include <stdlib.h>

#include "tile.h"

int compare_tile(struct tile_t t1, struct tile_t t2)
{
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        if (t1.t[i] != t2.t[i])
            return 0;
        if (t1.c[i] != t2.c[i])
            return 0;
    }
    return 1;
}
