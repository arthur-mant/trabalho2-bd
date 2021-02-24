#include <stdlib.h>
#include "grafo.h"
#include "transacao.h"

Grafo build_grafo(int vertices) {

    Grafo g;

    g.arestas = 0;
    g.vertices = vertices;
    //g.adjacencia = (int **)malloc(vertices*vertices*sizeof(int))
    g.adjacencia = (int **)malloc(vertices*sizeof(int *));

    for(int i=0; i<vertices; i++)
        g.adjacencia[i] = (int *)malloc(vertices*sizeof(int *));

    return g;
}

int max_id(std::vector<struct Operacao> operacoes) {

    int max = 0;

    for (struct Operacao x: operacoes)
        if (max < x.id)
            max = x.id;

    return max;

}
