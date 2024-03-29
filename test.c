/*#include <igraph.h>
#include <stdio.h>

int main(void) {

    igraph_t g;
    igraph_vector_int_t v1, v2;

	simple use
    igraph_vector_int_init(&v1, 10);
    VECTOR(v1)[0] = 0;
    VECTOR(v1)[1] = 1;
    VECTOR(v1)[2] = 7;
    VECTOR(v1)[3] = 2;
    VECTOR(v1)[4] = 2;
    VECTOR(v1)[5] = 3;
    VECTOR(v1)[6] = 3;
    VECTOR(v1)[7] = 5;
	VECTOR(v1)[8] = 3;
	VECTOR(v1)[9] = 17;
    igraph_create(&g, &v1, 0, 0);
	print

	printf("number of vertices : %ld \n",igraph_vcount(&g));
	printf("number of edges : %ld \n",igraph_ecount(&g));
    if (igraph_vcount(&g) != 4) {
        return 1;
    }
    igraph_vector_int_init(&v2, 0);
    igraph_get_edgelist(&g, &v2, 0);
    igraph_vector_int_sort(&v1);
    igraph_vector_int_sort(&v2);
    if (!igraph_vector_int_all_e(&v1, &v2)) {
        return 2;
    }
    igraph_destroy(&g);

    higher number of vertices
    igraph_create(&g, &v1, 10, 0);
    if (igraph_vcount(&g) != 10) {
        return 1;
    }
    igraph_get_edgelist(&g, &v2, 0);
    igraph_vector_int_sort(&v1);
    igraph_vector_int_sort(&v2);
    if (!igraph_vector_int_all_e(&v1, &v2)) {
        return 3;
    }
    igraph_destroy(&g);
    igraph_vector_int_destroy(&v1);
    igraph_vector_int_destroy(&v2);

    return 0; */
	#include <igraph.h>
#include <string.h>
#include <stdlib.h>

/* Prints graph, vertex and edge attributes stored in a graph. */
void print_attributes(const igraph_t *g) {
    igraph_vector_int_t gtypes, vtypes, etypes;
    igraph_strvector_t gnames, vnames, enames;
    igraph_integer_t i, j;

    igraph_vector_int_init(&gtypes, 0);
    igraph_vector_int_init(&vtypes, 0);
    igraph_vector_int_init(&etypes, 0);
    igraph_strvector_init(&gnames, 0);
    igraph_strvector_init(&vnames, 0);
    igraph_strvector_init(&enames, 0);

    igraph_cattribute_list(g,
                           &gnames, &gtypes,
                           &vnames, &vtypes,
                           &enames, &etypes);

    /* graph attributes */
    for (i = 0; i < igraph_strvector_size(&gnames); i++) {
        printf("%s=", igraph_strvector_get(&gnames, i));
        if (VECTOR(gtypes)[i] == IGRAPH_ATTRIBUTE_NUMERIC) {
            igraph_real_printf(GAN(g, igraph_strvector_get(&gnames, i)));
            putchar(' ');
        } else {
            printf("\"%s\" ", GAS(g, igraph_strvector_get(&gnames, i)));
        }
    }
    printf("\n");

    /* vertex attributes */
    for (i = 0; i < igraph_vcount(g); i++) {
        printf("Vertex %" IGRAPH_PRId ": ", i);
        for (j = 0; j < igraph_strvector_size(&vnames); j++) {
            printf("%s=", igraph_strvector_get(&vnames, j));
            if (VECTOR(vtypes)[j] == IGRAPH_ATTRIBUTE_NUMERIC) {
                igraph_real_printf(VAN(g, igraph_strvector_get(&vnames, j), i));
                putchar(' ');
            } else {
                printf("\"%s\" ", VAS(g, igraph_strvector_get(&vnames, j), i));
            }
        }
        printf("\n");
    }

    /* edge attributes */
    for (i = 0; i < igraph_ecount(g); i++) {
        printf("Edge %" IGRAPH_PRId " (%" IGRAPH_PRId "-%" IGRAPH_PRId "): ", i, IGRAPH_FROM(g, i), IGRAPH_TO(g, i));
        for (j = 0; j < igraph_strvector_size(&enames); j++) {
            printf("%s=", igraph_strvector_get(&enames, j));
            if (VECTOR(etypes)[j] == IGRAPH_ATTRIBUTE_NUMERIC) {
                igraph_real_printf(EAN(g, igraph_strvector_get(&enames, j), i));
                putchar(' ');
            } else {
                printf("\"%s\" ", EAS(g, igraph_strvector_get(&enames, j), i));
            }
        }
        printf("\n");
    }

    igraph_strvector_destroy(&enames);
    igraph_strvector_destroy(&vnames);
    igraph_strvector_destroy(&gnames);
    igraph_vector_int_destroy(&etypes);
    igraph_vector_int_destroy(&vtypes);
    igraph_vector_int_destroy(&gtypes);
}

int main(void) {
    igraph_t graph;
    igraph_vector_t y;

    /* Turn on attribute handling. */
    igraph_set_attribute_table(&igraph_cattribute_table);

    igraph_small(&graph, 3, IGRAPH_DIRECTED, 0,1, 1,2, -1);

    /* Set graph attributes. */
    /* numeric */
    SETGAN(&graph, "id", 10);
    /* string */
    SETGAS(&graph, "name", "toy");
    /* boolean */
    SETGAB(&graph, "is_regular", false);

    /* Set edge string attribute. */
    SETEAS(&graph, "color", 1, "RED");

    /* Set vertex attributes as vector. */
    igraph_vector_init(&y, igraph_vcount(&graph));
    igraph_vector_fill(&y, 1.23);
    SETVANV(&graph, "y", &y);
    igraph_vector_destroy(&y);

    /* Set single vertex numeric attribute. */
    SETVAN(&graph, "y", 0, -1);

    /* Delete graph attribute. */
    DELGA(&graph, "is_regular");

    /* Print the final result. */
    print_attributes(&graph);

    /* Delete all remaining attributes. */
    DELALL(&graph);

    /* Destroy the graph. */
    igraph_destroy(&graph);

    return 0;
}