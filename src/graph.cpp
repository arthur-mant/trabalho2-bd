#include <bits/stdc++.h>
#include "../headers/graph.h"
/* utilidade para a função de detecção de ciclo */
int detectaCiclo_util(Grafo *G, int* visitados, int atual) {
  if (visitados[atual])
    return 1;

  visitados[atual] = 1;
  int flag = 0;
  for (int i = 0; i < G->n_vertices; ++i) {
    if (G->m_adjacencia[atual][i] == 1) {
      flag = detectaCiclo_util(G, visitados, i);
      if (flag) {
        visitados[atual] = 0;
        return 1;
      }
    }
  }
  visitados[atual] = 0;
  return 0;
}
/* detecta ciclo no grafo */
int detectaCiclo(Grafo *G) {
  int visitados[G->n_vertices];
  int flag = 0;
  for (int i = 0; i < G->n_vertices; ++i)
    visitados[i] = 0;

  for(int i = 0; i < G->n_vertices; ++i) {
    visitados[i] = 1;
    for (int j = 0; j < G->n_vertices; ++j) {
      if (G->m_adjacencia[i][j] == 1) {
        flag = detectaCiclo_util(G, visitados, j);
        if (flag)
          return 1;
      }
    }
    visitados[i] = 0;
  }
  return 0;
}

void reinicia_grafo(Grafo *g){
  for(int i = 0; i < g->n_vertices; i++){
    for(int j = 0; j < g->n_vertices; j++){
      if(i == j) g->m_adjacencia[i][j] = 1;
      else g->m_adjacencia[i][j] = 0;
    }

  }
}
