#include "graph.h"
#include "deck.h"


igraph_vector_int_t init_vector(){
	igraph_vector_int_t v1;
	for (int i=0;i<13;i++){
		VECTOR(v1)[i] = 0;
	}
	return v1;
}

igraph_t transform_tile_to_graph(struct tile_t tile){
	igraph_t g=igraph_create(&g, NULL, 0, 0);
	for (int component=0;component<13;component++){ // pour chaque composante du graphe
		int previous=-1;
		int c=0;
		igraph_vector_int_t v1=init_vector();
	
		for (int j=0;j<13;j++){ // on parcourt chaque sommet du la tuile.t
			if (tile.t[j]==component){
				if (previous==-1){
					previous==j;
				}
				else{
					VECTOR(v1)[c] = previous ;
					VECTOR(v1)[c+1] = j ;
					c+=1;
					previous=j;
				}
			}
		}
		igraph_add_edges(&g, &v1,NULL);
	}
	igraph_create(&g, &v1, 0, 0);
	return g;
}