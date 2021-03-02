#include <bits/stdc++.h>
#include "../headers/graph.h"

bool detectaCiclo_util(Grafo *G, int* visitados, int atual) {
  if (visitados[atual])
    return true;

  visitados[atual] = 1;
  bool flag = false;
  for (int i = 0; i < G->n_vertices; ++i) {
    if (G->m_adjacencia[atual][i] == 1) {
      flag = detectaCiclo_util(G, visitados, i);
      if (flag) {
        visitados[atual] = 0;
        return true;
      }
    }
  }
  visitados[atual] = 0;
  return false;
}

bool detectaCiclo(Grafo *G) {
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
          return true;
      }
    }
    visitados[i] = 0;
  }
  return false;
}
