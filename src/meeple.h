#ifndef __MEEPLE_H__
#define __MEEPLE_H__

#include"move.h"


/* Struct meeple
 * @param:
 * -The tab player1 has all the vertices where the player 1 has a meeple, same for player2
 *- The size is the size of each tab
 *- The capacity is the capcity of each tab 
*/

struct meeple_t{
	int* player1;
	int* player2;
	int size1;
	int size2;
	int capacity1;
	int capacity2;
};


/*@ Init the struct meeple
 * @param: the capacity(int) of the both tab player
 * @return: a struct meeple with empty param
 */
struct meeple_t init_meeple(int capacity);


/*get meeple back :) after closing component*/
void get_back_meeple(struct meeple_t *meeple, int *vertices, int size);


/*@ Copy a meeple
 * @param: a meeple
 * @return: a scopy of a meeple
 */
struct meeple_t copy_meeple(struct meeple_t meeple);


/*free the tabs player1 and player2 of the meeple
 * */
void free_meeple(struct meeple_t meeple);
#endif
