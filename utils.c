#include "grafo.h"

Grafo build_grafo(int vertices) {

    Grafo g;

    g.arestas = 0;
    g.vertices = vertices;
    g.adjacencia = (int **)malloc(vertices*vertices*sizeof(int))

    return g
}
