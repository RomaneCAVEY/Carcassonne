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


/*Add the meeple played in the move
 *@param: the meeple and the move 
 *@return: the meeple with the new meeple according to the player who played 
 * */
void add_meeple(struct meeple_t *meeple, struct move_t move, int infinit);




/*free the tabs player1 and player2 of the meeple
 * */
void free_meepple(struct meeple_t meeple);
#endif
