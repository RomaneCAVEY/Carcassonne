#include "super_board.h"
#include "board.h"
#include "meeple.h"
#include "graph.h"
#include "move.h"
#include "score.h"
#include <igraph.h>
#include <stdlib.h>
#include <stdio.h>

const char * str_color[8] = {
  "GREEN", "GRAY", "BLUE", "BROWN",
  "PURPLE", "GOLD", "AZURE", "MAHOGANY",
};

/*Init the super_board
* @param: the size of colors, the first tile to init the board, the super_board
* @return:the super_board with the first tile in the board
*/
void init_super_board(struct tile_t tile, struct super_board_t *super_board){
  super_board->board = board_init(tile);
  super_board->graph = init_graph(tile);
  super_board->colors = malloc(sizeof(enum color_t)*MAX_CONNECTIONS);
  super_board->list = malloc(sizeof(struct utils_graph_t)*1);
  struct utils_graph_t ugraph = {.x=0, .y=0, .center=12};
  super_board->list[0] = ugraph;
  super_board->capacite = 1;
  super_board->size = 0;
  add_color_to_super_board(tile, super_board);
  super_board->size++;
  super_board->meeple = init_meeple(7);

  super_board->finished_structures.count = 0;
  super_board->finished_structures.size = 1;
  super_board->finished_structures.list = malloc(sizeof(int));
}

/*Add the colors of a vertice of the graph in the board
* @param: sommet=numero du sommet, c= sa couleur, super_board
* @return:the super_board with the color of the new tile
*/
void add_color_to_super_board(struct tile_t tile, struct super_board_t* super_board)
{
	for (int i = 0; i < MAX_CONNECTIONS; ++i) {
		super_board->colors[MAX_CONNECTIONS*super_board->size + i] = tile.c[i];
	}
}


int size_super_board(struct super_board_t * super_board)
{
	return super_board->size;
}


void add_to_utils_graph(struct super_board_t * super_board, int x, int y, int center)
{
	struct utils_graph_t u = {.x=x, .y=y, .center=center};
	super_board->list[size_super_board(super_board)] = u;
}


// return 1 if success else return 0 
int add_tile_to_super_board(struct tile_t tile, struct super_board_t * super_board, int x, int y)
{
	for (int i = 0; i < super_board->size; ++i) {
		if ((super_board->list[i].x == x) && (super_board->list[i].y == y))
			return 0;
	}
	// printf("------ You want to add a tile on a super board ------\n");
	if (super_board->capacite == super_board->size) {
		// printf("Capacity double\n");
		super_board->capacite *= 2;
		super_board->colors = realloc(super_board->colors, 13*super_board->capacite*sizeof(enum color_t));
		super_board->list = realloc(super_board->list, super_board->capacite*sizeof(struct utils_graph_t));
	}
	// printf("add tile to board \n");
	board_add(super_board->board, tile, x, y);
	// printf("add in utils graph\n");
	add_to_utils_graph(super_board, x, y, (size_super_board(super_board)*13-1)+13); // n° of the last vertice in the tile
	// printf("add colors\n");
	add_color_to_super_board(tile, super_board);
	// printf("add tile to a graph\n");
	super_board->graph = add_tile_to_graph(tile, super_board->graph, super_board, x, y);
	super_board->size += 1;
	// printf("------ End add a tile on a super board ------\n");
	return 1;
}


/*Create a file dot to display the graph
* @param: super_board, name of the file with .dot
* @return: void but created a file_name.dot in root
*/
void create_neato(struct super_board_t * super_board, char * file_name)
{
	FILE* out = fopen(file_name, "w+");
	fprintf(out, "graph {\n");
	fprintf(out, "  node[style=filled];\n");
	// Vertices & meeples :
	for (int i=0; i < super_board->size; ++i) {
		// coordonate in the dot file
		float x = (float) super_board->list[i].x * 5;
		float y = (float) - super_board->list[i].y * 5;
		// n° of the vertex in the center of the tile
		int center = super_board->list[i].center;
		// take the tile at this position
		struct tile_t tile = board_get(super_board->board, x/5, -y/5);
		// place all vertices of the tile in dot file (can be improved)
		for (int j=(center-12); j < center+1; ++j) {
			switch (j%13)
			{
			case 0:
				fprintf(out, "  %d [fillcolor=%s,shape=\"circle\",pos=\"%f, %f!\"];\n", j, str_color[tile.c[j%13]], x-1, y+2);
				for (int i = 0; i < super_board->meeple.size1; ++i) {
					if (j == super_board->meeple.player1[i])
						fprintf(out, "  %d [fillcolor=RED,shape=\"circle\",pos=\"%f, %f!\"];\n", -j, x-0.5, y+2.5);
				}
				for (int i = 0; i < super_board->meeple.size2; ++i) {
					if (j == super_board->meeple.player2[i])
						fprintf(out, "  %d [fillcolor=BLUE,shape=\"circle\",pos=\"%f, %f!\"];\n", -j, x-0.5, y+2.5);
				}
				break;
			case 1:
				fprintf(out, "  %d [fillcolor=%s,shape=\"circle\",pos=\"%f, %f!\"];\n", j, str_color[tile.c[j%13]], x, y+2);
				for (int i = 0; i < super_board->meeple.size1; ++i) {
					if (j == super_board->meeple.player1[i])
						fprintf(out, "  %d [fillcolor=RED,shape=\"circle\",pos=\"%f, %f!\"];\n", -j, x+0.5, y+2.5);
				}
				for (int i = 0; i < super_board->meeple.size2; ++i) {
					if (j == super_board->meeple.player2[i])
						fprintf(out, "  %d [fillcolor=BLUE,shape=\"circle\",pos=\"%f, %f!\"];\n", -j, x+0.5, y+2.5);
				}
				break;
			case 2:
				fprintf(out, "  %d [fillcolor=%s,shape=\"circle\",pos=\"%f, %f!\"];\n", j, str_color[tile.c[j%13]], x+1, y+2);
				for (int i = 0; i < super_board->meeple.size1; ++i) {
					if (j == super_board->meeple.player1[i])
						fprintf(out, "  %d [fillcolor=RED,shape=\"circle\",pos=\"%f, %f!\"];\n", -j, x+1.5, y+2.5);
				}
				for (int i = 0; i < super_board->meeple.size2; ++i) {
					if (j == super_board->meeple.player2[i])
						fprintf(out, "  %d [fillcolor=BLUE,shape=\"circle\",pos=\"%f, %f!\"];\n", -j, x+1.5, y+2.5);
				}
				break;
			case 3:
				fprintf(out, "  %d [fillcolor=%s,shape=\"circle\",pos=\"%f, %f!\"];\n", j, str_color[tile.c[j%13]], x+2, y+1);
				for (int i = 0; i < super_board->meeple.size1; ++i) {
					if (j == super_board->meeple.player1[i])
						fprintf(out, "  %d [fillcolor=RED,shape=\"circle\",pos=\"%f, %f!\"];\n", -j, x+2.5, y+1.5);
				}
				for (int i = 0; i < super_board->meeple.size2; ++i) {
					if (j == super_board->meeple.player2[i])
						fprintf(out, "  %d [fillcolor=BLUE,shape=\"circle\",pos=\"%f, %f!\"];\n", -j, x+2.5, y+1.5);
				}
				break;
			case 4:
				fprintf(out, "  %d [fillcolor=%s,shape=\"circle\",pos=\"%f, %f!\"];\n", j, str_color[tile.c[j%13]], x+2, y);
				for (int i = 0; i < super_board->meeple.size1; ++i) {
					if (j == super_board->meeple.player1[i])
						fprintf(out, "  %d [fillcolor=RED,shape=\"circle\",pos=\"%f, %f!\"];\n", -j, x+2.5, y+0.5);
				}
				for (int i = 0; i < super_board->meeple.size2; ++i) {
					if (j == super_board->meeple.player2[i])
						fprintf(out, "  %d [fillcolor=BLUE,shape=\"circle\",pos=\"%f, %f!\"];\n", -j, x+2.5, y+0.5);
				}
				break;
			case 5:
				fprintf(out, "  %d [fillcolor=%s,shape=\"circle\",pos=\"%f, %f!\"];\n", j, str_color[tile.c[j%13]], x+2, y-1);
				for (int i = 0; i < super_board->meeple.size1; ++i) {
					if (j == super_board->meeple.player1[i])
						fprintf(out, "  %d [fillcolor=RED,shape=\"circle\",pos=\"%f, %f!\"];\n", -j, x+2.5, y-0.5);
				}
				for (int i = 0; i < super_board->meeple.size2; ++i) {
					if (j == super_board->meeple.player2[i])
						fprintf(out, "  %d [fillcolor=BLUE,shape=\"circle\",pos=\"%f, %f!\"];\n", -j, x+2.5, y-0.5);
				}
				break;
			case 6:
				fprintf(out, "  %d [fillcolor=%s,shape=\"circle\",pos=\"%f, %f!\"];\n", j, str_color[tile.c[j%13]], x+1, y-2);
				for (int i = 0; i < super_board->meeple.size1; ++i) {
					if (j == super_board->meeple.player1[i])
						fprintf(out, "  %d [fillcolor=RED,shape=\"circle\",pos=\"%f, %f!\"];\n", -j, x+1.5, y-1.5);
				}
				for (int i = 0; i < super_board->meeple.size2; ++i) {
					if (j == super_board->meeple.player2[i])
						fprintf(out, "  %d [fillcolor=BLUE,shape=\"circle\",pos=\"%f, %f!\"];\n", -j, x+1.5, y-1.5);
				}
				break;
			case 7:
				fprintf(out, "  %d [fillcolor=%s,shape=\"circle\",pos=\"%f, %f!\"];\n", j, str_color[tile.c[j%13]], x, y-2);
				for (int i = 0; i < super_board->meeple.size1; ++i) {
					if (j == super_board->meeple.player1[i])
						fprintf(out, "  %d [fillcolor=RED,shape=\"circle\",pos=\"%f, %f!\"];\n", -j, x+0.5, y-1.5);
				}
				for (int i = 0; i < super_board->meeple.size2; ++i) {
					if (j == super_board->meeple.player2[i])
						fprintf(out, "  %d [fillcolor=BLUE,shape=\"circle\",pos=\"%f, %f!\"];\n", -j, x+0.5, y-1.5);
				}
				break;
			case 8:
				fprintf(out, "  %d [fillcolor=%s,shape=\"circle\",pos=\"%f, %f!\"];\n", j, str_color[tile.c[j%13]], x-1, y-2);
				for (int i = 0; i < super_board->meeple.size1; ++i) {
					if (j == super_board->meeple.player1[i])
						fprintf(out, "  %d [fillcolor=RED,shape=\"circle\",pos=\"%f, %f!\"];\n", -j, x-0.5, y-1.5);
				}
				for (int i = 0; i < super_board->meeple.size2; ++i) {
					if (j == super_board->meeple.player2[i])
						fprintf(out, "  %d [fillcolor=BLUE,shape=\"circle\",pos=\"%f, %f!\"];\n", -j, x-0.5, y-1.5);
				}
				break;
			case 9:
				fprintf(out, "  %d [fillcolor=%s,shape=\"circle\",pos=\"%f, %f!\"];\n", j, str_color[tile.c[j%13]], x-2, y-1);
				for (int i = 0; i < super_board->meeple.size1; ++i) {
					if (j == super_board->meeple.player1[i])
						fprintf(out, "  %d [fillcolor=RED,shape=\"circle\",pos=\"%f, %f!\"];\n", -j, x-1.5, y-0.5);
				}
				for (int i = 0; i < super_board->meeple.size2; ++i) {
					if (j == super_board->meeple.player2[i])
						fprintf(out, "  %d [fillcolor=BLUE,shape=\"circle\",pos=\"%f, %f!\"];\n", -j, x-1.5, y-0.5);
				}
				break;
			case 10:
				fprintf(out, "  %d [fillcolor=%s,shape=\"circle\",pos=\"%f, %f!\"];\n", j, str_color[tile.c[j%13]], x-2, y);
				for (int i = 0; i < super_board->meeple.size1; ++i) {
					if (j == super_board->meeple.player1[i])
						fprintf(out, "  %d [fillcolor=RED,shape=\"circle\",pos=\"%f, %f!\"];\n", -j, x-1.5, y+0.5);
				}
				for (int i = 0; i < super_board->meeple.size2; ++i) {
					if (j == super_board->meeple.player2[i])
						fprintf(out, "  %d [fillcolor=BLUE,shape=\"circle\",pos=\"%f, %f!\"];\n", -j, x-1.5, y+0.5);
				}
				break;
			case 11:
				fprintf(out, "  %d [fillcolor=%s,shape=\"circle\",pos=\"%f, %f!\"];\n", j, str_color[tile.c[j%13]], x-2, y+1);
				for (int i = 0; i < super_board->meeple.size1; ++i) {
					if (j == super_board->meeple.player1[i])
						fprintf(out, "  %d [fillcolor=RED,shape=\"circle\",pos=\"%f, %f!\"];\n", -j, x-1.5, y+1.5);
				}
				for (int i = 0; i < super_board->meeple.size2; ++i) {
					if (j == super_board->meeple.player2[i])
						fprintf(out, "  %d [fillcolor=BLUE,shape=\"circle\",pos=\"%f, %f!\"];\n", -j, x-1.5, y+1.5);
				}
				break;
			case 12:
				fprintf(out, "  %d [fillcolor=%s,shape=\"circle\",pos=\"%f, %f!\"];\n", j, str_color[tile.c[j%13]], x, y);
				for (int i = 0; i < super_board->meeple.size1; ++i) {
					if (j == super_board->meeple.player1[i])
						fprintf(out, "  %d [fillcolor=RED,shape=\"circle\",pos=\"%f, %f!\"];\n", -j, x+0.5, y+0.5);
				}
				for (int i = 0; i < super_board->meeple.size2; ++i) {
					if (j == super_board->meeple.player2[i])
						fprintf(out, "  %d [fillcolor=BLUE,shape=\"circle\",pos=\"%f, %f!\"];\n", -j, x+0.5, y+0.5);
				}
				break;
			default:
				break;
			}
		}
	}
	fprintf(out, "\n");
	// Display edges :
	igraph_es_t es;
	igraph_eit_t eit;
	igraph_es_all(&es, IGRAPH_EDGEORDER_ID);
	igraph_eit_create(&super_board->graph, es, &eit);
	IGRAPH_EIT_RESET(eit);
	while (!IGRAPH_EIT_END(eit)) {
		igraph_integer_t e = IGRAPH_EIT_GET(eit);
		igraph_integer_t u = IGRAPH_FROM(&super_board->graph, e);
		igraph_integer_t v = IGRAPH_TO(&super_board->graph, e);
		fprintf(out, " %" IGRAPH_PRId " -- %"  IGRAPH_PRId ";\n", u, v);
		IGRAPH_EIT_NEXT(eit);
	}
	igraph_eit_destroy(&eit);
	igraph_es_destroy(&es);
	fprintf(out, "}\n");
	fclose(out);
	// command used : neato -Tx11 file_name.dot &
}

/*Free the memory allocated by the super_board
* @param:the super_board
* @return: nothing
*/
void free_super_board(struct super_board_t* super_board){
  free(super_board->colors);
  free(super_board->list);
  free(super_board->finished_structures.list);

  free_graph(super_board->graph);
  free_meeple(super_board->meeple);
  board_free(super_board->board);
}


struct super_board_t copy_super_board(struct super_board_t super_board){
	struct super_board_t copy={};
	copy.board=board_init(board_get(super_board.board, 0, 0));
	copy_board(super_board.board, copy.board);

	copy.size=super_board.size;
	copy.capacite=super_board.capacite;
	copy.meeple=copy_meeple(super_board.meeple);

	copy.colors=malloc((super_board.capacite+1)*MAX_CONNECTIONS*sizeof(enum color_t));
	for (int i=0; i<((super_board.size)*13);i++){
		copy.colors[i]=super_board.colors[i];
	}
	
	copy.list=malloc((super_board.capacite+1)*sizeof(struct utils_graph_t));
	for (int i=0; i<super_board.size;i++){
		copy.list[i]=super_board.list[i];
	}

	copy.finished_structures.list=malloc( super_board.finished_structures.size*sizeof(int));

	copy.finished_structures.count=super_board.finished_structures.count;
	copy.finished_structures.size=super_board.finished_structures.size;
	for (int i=0; i<super_board.finished_structures.count;i++){
		copy.finished_structures.list[i]=super_board.finished_structures.list[i];
	}

	igraph_copy(&copy.graph, &super_board.graph); 
	return copy;
}


void free_copy_super_board(struct super_board_t* copy){
  free(copy->colors);
  free(copy->list);
  free(copy->finished_structures.list);
  free_graph(copy->graph);
  free_meeple(copy->meeple);
  board_free(copy->board);
}



int add_meeple(struct move_t* move, struct super_board_t *sboard, enum gamemode_t gt)
{
  move->meeple=14;
  if (gt==NO_MEEPLE){
    return 0;
  }
  if(gt == FINITE_MEEPLE){	
    if (move->player_id == 1 && sboard->meeple.size2 == 7 ){
      return 0;
    }
    if (move->player_id == 0 && sboard->meeple.size1 == 7 ){
      return 0;
    }
  }
	 // printf("&&&&&&&&&&&&&&&&&&&&&why not&&&&&&&&&&&&&&&&&&&&&&&&&&");

  if( gt == INFINITE_MEEPLE){
    if (sboard->meeple.capacity1 <= sboard->meeple.size1 ){
      sboard->meeple.capacity1 *= 2;
      sboard->meeple.player1 = realloc(sboard->meeple.player1, sizeof(int)*sboard->meeple.capacity1);
    }

    if (sboard->meeple.capacity2 <= sboard->meeple.size2 ){
      sboard->meeple.capacity2 *= 2;
      sboard->meeple.player2 = realloc(sboard->meeple.player2, sizeof(int)*sboard->meeple.capacity2);
    }
  }


  for (int i= 1; i <12; i++){
    if (check_add_meeple(*sboard, i, &sboard->meeple)){
        if (move->player_id == 0 ){
        move->meeple = i;
        sboard->meeple.player1[sboard->meeple.size1] = ((sboard->size-1) * 13) + move->meeple ;
        sboard->meeple.size1 +=1;
		//printf("CA MARCHE");
        return 1;
        } else if (move->player_id == 1 ){
        move->meeple = i;
        sboard->meeple.player2[sboard->meeple.size2] = ((sboard->size-1) * 13) + move->meeple ;
        sboard->meeple.size2 +=1;
        return 1;
        }
    }
  }
  //printf("&&&&&&&&&&&&&&&&&&&&&why not&&&&&&&&&&&&&&&&&&&&&&&&&&");
  return 0;
}

// SERVER VERSION
int add_meeple_to_board(struct move_t* move, struct super_board_t *sboard, enum gamemode_t gt)
{
  if (gt == NO_MEEPLE || move->meeple > 12) {
    return 1;
  }
   if(gt == FINITE_MEEPLE){	
    if (move->player_id == 1 && sboard->meeple.size2 == 7 ){
      return 0;
    }
    if (move->player_id == 0 && sboard->meeple.size1 == 7 ){
      return 0;
    }
  }
  
 if (gt == INFINITE_MEEPLE) {
    if (sboard->meeple.capacity1 <= sboard->meeple.size1 ){
      sboard->meeple.capacity1 *= 2;
      sboard->meeple.player1 = realloc(sboard->meeple.player1, sizeof(int)*sboard->meeple.capacity1);
    }

    if (sboard->meeple.capacity2 <= sboard->meeple.size2) {
      sboard->meeple.capacity2 *= 2;
      sboard->meeple.player2 = realloc(sboard->meeple.player2, sizeof(int)*sboard->meeple.capacity2);
    }
  } 
  
 
  if (check_add_meeple(*sboard, move->meeple, &sboard->meeple)) {
    if (move->player_id == 0) {
      sboard->meeple.player1[sboard->meeple.size1] = ((sboard->size-1) * 13) + move->meeple ;
      sboard->meeple.size1 +=1;
      return 1;
    } else if (move->player_id == 1) {
      sboard->meeple.player2[sboard->meeple.size2] = ((sboard->size-1) * 13) + move->meeple ;
      sboard->meeple.size2 +=1;
      return 1;
    }
  }
  return 0;
}




int check_add_meeple(struct super_board_t sboard, enum conn_t indexVertex, struct meeple_t *meeple)
{
	//printf("\e[1;37;103m CHECK ADD MEEPLE:\e[0m ");
   
  igraph_vector_int_t components;
  igraph_vector_int_t csize;
  igraph_integer_t count = 0;

  igraph_vector_int_init(&csize, 0);
  igraph_vector_int_init(&components, 0);

  igraph_connected_components(&sboard.graph, &components, &csize, &count, IGRAPH_WEAK);

  igraph_integer_t nb_vertices = igraph_vector_int_size(&components);

  int *vertices = malloc(nb_vertices * sizeof(int));
  int size;
  
  //igraph_vector_int_print(&components);
  int n=find_right_component(components, (sboard.size - 1) * MAX_CONNECTIONS + indexVertex, count);
  
  size = vector_extract_component(components, n,vertices);

  for (int i=0; i<size; i++){
    for (int p1=0;p1 < sboard.meeple.size1 ; p1++){
      if(meeple->player1[p1] == vertices[i]){
	  free(vertices);
	  igraph_vector_int_destroy(&components);
	  igraph_vector_int_destroy(&csize);
	  return 0;
      }
    }
    for (int p2=0; p2 < sboard.meeple.size2 ; p2++){
      if(meeple->player2[p2] == vertices[i]){
	  free(vertices);
	  igraph_vector_int_destroy(&components);
	  igraph_vector_int_destroy(&csize);
	  return 0;
      }
    }
  }
  free(vertices);
  igraph_vector_int_destroy(&components);
  igraph_vector_int_destroy(&csize);
  //printf("\e[1;37;103m ADD A MEEPLE ON EDGE:\e[0m \n");
  return 1;
}

int find_right_component(igraph_vector_int_t components, int num_sommet, igraph_integer_t count){
  igraph_integer_t nb_vertices = igraph_vector_int_size(&components);
  for (int i=0; i<count;i++){
    int *vertices = malloc(nb_vertices * sizeof(int));
    int size = vector_extract_component(components, i,vertices);
    for (int j=0; j<size; j++){
      if (vertices[j]==num_sommet){
	free(vertices);
	return i;
      }
    }
    free(vertices);
  }
  return 666;
  //printf("\e[1;37;103m ADD A MEEPLE ON EDGE:\e[0m %d",num_tile);	
}
