#include <stdio.h>
#include "test_board.h"
#include "test_deck.h"
#include "test_tile.h"
#include "test_common.h"
#include "test_score.h"

int main() {
  int test_count = 5;
  
  int sum = board_tests()
    + deck_tests()
    + tile_tests()
    + common_tests()
    + score_tests();

  if (sum == 0)
    printf("\n\e[0;30;107mTests suites passed:\e[1;37;102m %d/%d \e[0m\n\n", test_count - sum, test_count);
  else if (sum < test_count)
    printf("\n\e[0;30;107mTests suites passed:\e[1;37;103m %d/%d \e[0m\n\n", test_count - sum, test_count);
  else
    printf("\n\e[0;30;107mTests suites passed:\e[1;37;101m %d/%d \e[0m\n\n", test_count - sum, test_count);
    
  return sum;
}
