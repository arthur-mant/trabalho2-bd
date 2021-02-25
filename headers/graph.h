typedef struct Grafo{
  int **m_adjacencia;
  int n_vertices;

  Grafo(int n){
    n_vertices = n;
    m_adjacencia = new int*[n];
    for(int i = 0; i < n; i++)
      m_adjacencia[i] = new int[n];

    for(int i = 0; i < n; i++)
      for(int j = 0; j < n; j++)
        m_adjacencia[i][j] = 0;
  }

}Grafo;

int detectaCiclo_util(Grafo *G, int* visitados, int atual);
int detectaCiclo(Grafo *G);
