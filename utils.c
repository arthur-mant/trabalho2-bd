#include <stdlib.h>
#include "grafo.h"

Grafo build_grafo(int vertices) {

    Grafo g;

    g.arestas = 0;
    g.vertices = vertices;
    //g.adjacencia = (int **)malloc(vertices*vertices*sizeof(int))
    g.adjacencia = (int **)malloc(vertices*sizeof(int *));

    for(int i=0; i<vertices; i++)
        g.adjacencia[i] = (int *)malloc(vertices*sizeof(int *));

    for(int i=0; i < vertices; i++)
        for(int j=0; j < vertices; j++)
            g.adjacencia[i][j] = 0;
    return g;
}
