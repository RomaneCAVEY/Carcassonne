#include "../tile.h" // pour la fonction qui compare les tuiles elle include move.h qui include deck.h

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void test_deck_empty()
{
  printf(" • %s", __func__);
  
  size_t capa = 10;
  struct deck_t *deckG = deck_empty(capa);

  assert(deck_size(deckG) == 0);

  deck_free(deckG);
  
  printf("\t\e[1;102m SUCCESS \e[0m\n");
}

void test_deck_add()
{
  printf(" • %s", __func__);
  
  size_t capa = 10;
  struct deck_t *deckG = deck_empty(capa);

  
  deck_add(deckG, CARC_TILE_INIT);

  assert(deck_size(deckG) == 1);
  assert(compare_tile(deck_get(deckG , 0), CARC_TILE_INIT));

  
  deck_add(deckG, CARC_TILE_GREEN);

  assert(deck_size(deckG) == 2);
  assert(compare_tile(deck_get(deckG, 0), CARC_TILE_INIT));

  deck_free(deckG);
  
  printf("\t\e[1;102m SUCCESS \e[0m\n");
}

void test_deck_size()
{
  printf(" • %s", __func__);
  // is used in other test, so if previous tests works, it's tested !
  
  printf("\t\e[1;102m SUCCESS \e[0m\n");
}

void test_deck_get()
{
  printf(" • %s", __func__);
  
  size_t capa = 10;
  struct deck_t *deckG = deck_empty(capa);

  
  deck_add(deckG, CARC_TILE_INIT);
  deck_add(deckG, CARC_TILE_GREEN);
  deck_add(deckG, CARC_TILE_TINI);
  deck_add(deckG, CARC_TILE_XROAD);

  assert(compare_tile(deck_get(deckG, 2), CARC_TILE_TINI));
  assert(compare_tile(deck_get(deckG, 0), CARC_TILE_INIT));
  assert(compare_tile(deck_get(deckG, 1), CARC_TILE_GREEN));
  assert(compare_tile(deck_get(deckG, 3), CARC_TILE_XROAD));

  deck_free(deckG);
  
  printf("\t\e[1;102m SUCCESS \e[0m\n");
}

void test_deck_copy()
{
  printf(" • %s", __func__);
  
  size_t capa = 10;
  struct deck_t *deckG = deck_empty(capa);

  
  deck_add(deckG, CARC_TILE_INIT);
  deck_add(deckG, CARC_TILE_GREEN);
  deck_add(deckG, CARC_TILE_TINI);
  deck_add(deckG, CARC_TILE_XROAD);

  struct deck_t *deckC = deck_copy(deckG);
  
  assert(compare_tile(deck_get(deckG, 2), deck_get(deckC, 2)));
  assert(compare_tile(deck_get(deckG, 0), deck_get(deckC, 0)));
  assert(compare_tile(deck_get(deckG, 1), deck_get(deckC, 1)));
  assert(compare_tile(deck_get(deckG, 3), deck_get(deckG, 3)));

  deck_free(deckG);
  
  printf("\t\e[1;102m SUCCESS \e[0m\n");
}

int deck_tests()
{
  printf("\n\e[30;47mFile %s\e[0m\n", __FILE__);
  
  test_deck_empty();
  test_deck_add();
  test_deck_size();
  test_deck_get();
  test_deck_copy();

  return 0;
}
