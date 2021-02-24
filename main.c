#include <stdio.h>
#include <stdlib.h>
#include "transacao.h"
#include "grafo.h"

int max_id(struct Operacao *operacoes, int num_operacoes) {

    int max = 0;

    for (int i=0; i<num_operacoes; i++)
        if (max < operacoes[i].id)
            max = operacoes[i].id;

    return max;

}

struct Grafo build_grafo(int vertices) {

    Grafo g;

    g.arestas = 0;
    g.vertices = vertices;
    g.adjacencia = (int **)malloc(vertices*sizeof(int *));

    for(int i=0; i<vertices; i++)
        g.adjacencia[i] = (int *)malloc(vertices*sizeof(int *));

    for(int i=0; i < vertices; i++)
        for(int j=0; j < vertices; j++)
            g.adjacencia[i][j] = 0;
    return g;
}

int *converte_indice_vetor(int *v, int tam_v, int num) {

    int *u, j;

    u = (int *)malloc(num*(sizeof(int)));

    j=0;
    for (int i=1; i<=tam_v; i++) {
        if (v[i] == 1) {
            u[j] = i;
            j++;
        }
    }
    return u;
}

struct ListaEscalonamentos encontra_escalonamento(struct Operacao *operacoes, int num_operacoes) {

    int num_abertos, *abertos, tam_abertos, i, aux, j, *v_aux;

    struct ListaEscalonamentos e;

    tam_abertos = max_id(operacoes, num_operacoes)+1;
    abertos = (int *)malloc(tam_abertos*sizeof(int));

    for (int i=0; i<tam_abertos; i++)
        abertos[i] = 0;

    e.esc = (struct Escalonamento *)malloc(num_operacoes*sizeof(struct Escalonamento));
    e.tam_lista_escalonamento = 0;

    num_abertos = 0;
    i = 0;
    j = 0;
    v_aux = (int *)malloc(tam_abertos*sizeof(int));

    while (i < num_operacoes) {

        e.esc[j].num_transacoes = 0;
        e.esc[j].num_operacoes = 0;
        e.esc[j].operacoes = (struct Operacao *)malloc(num_operacoes*sizeof(struct Operacao));

        e.tam_lista_escalonamento += 1;

        for(int k=0; k<tam_abertos; k++)
            v_aux[k] = 0;

        do {

            e.esc[j].operacoes[e.esc[j].num_operacoes] = operacoes[i];
            e.esc[j].num_operacoes++;

            //abre transacao do id tal
            if ((abertos[operacoes[i].id] != 1) && (operacoes[i].op != 'C')) {
                abertos[operacoes[i].id] = 1;
                num_abertos++;
                v_aux[operacoes[i].id] = 1;
                e.esc[j].num_transacoes++;
            }
            //fecha transacao do id tal
            if ((abertos[operacoes[i].id] == 1) && (operacoes[i].op == 'C')) {
                abertos[operacoes[i].id] = 0;
                num_abertos--;
            }
            i++;

        } while ((num_abertos != 0) && (i < num_operacoes));

        e.esc[j].id_transacoes = converte_indice_vetor(v_aux, tam_abertos, e.esc[j].num_transacoes);

        j++;

        aux = 0;
        for (int k=0; k<=tam_abertos; k++)
            aux += abertos[k];

        if (aux != 0) {
            printf("PANIC!!!! Alguma transação não foi commitada\n");
        }

    }

    return e;

}

void imprime_grafo(struct Grafo *g){
    for(int i = 0;i < g->vertices; i++){
        for(int j = 0; j < g->vertices; j++){
            printf("%d ", g->adjacencia[i][j]);
        }
        printf("\n");
    }
}

int teste_seriabilidade(struct Escalonamento *e, struct Grafo *g) {

    // operações auxiliares i e j
    struct Operacao op_i, op_j;

    // Cria as arestas no grafo
    for(int i = 0; i < e->num_operacoes - 1; i++){
        op_i = e->operacoes[i];
        for(int j = 0; j < e->num_operacoes; j++){
            op_j = e->operacoes[j];

            // caso no qual transações diferentes acessam o mesmo atributo
            int transacao_conflito = (op_j.id != op_i.id) && (op_j.atributo == op_i.atributo) && (op_j.op != 'C' && op_i.op != 'C')
            if(transacao_conflito){

                // caso no qual existe um Write antes de um Read
                // Read antes de um Write
                // e Write antes de um Write.
                int cria_aresta = (op_j.op != op_i.op) || (op_j.op == 'W' && op_i.op == 'W');

                // cria uma aresta entre i e j na matriz adjacente
                if(cria_aresta){
                    g->adjacencia[op_i.id - 1][op_j.id - 1] = 1;
                }
            }
        }
    }


    imprime_grafo(g);

}

void heapPermutation(int *a, int size, int n){
    int aux;
    // if size becomes 1 then prints the obtained
    // permutation
    if (size == 1)
        return a;

    for (int i = 0; i < size; i++) {
        heapPermutation(a, size - 1, n);

        if (size % 2 == 1){
            aux = a[0];
            a[0] = a[size - 1];
            a[size - 1] = aux;

        }

        else{
            aux = a[i];
            a[i] = a[size - 1];
            a[size - 1] = aux;

        }
    }
}

int teste_equivalencia_visao(struct Escalonamento *e){

}

void main() {

    int pos, num_operacoes;
    struct Operacao operacoes[1000], operacao_aux;
    struct ListaEscalonamentos e;

    while (
        scanf("%d %d %c %c",
            &pos,
            &operacao_aux.id,
            &operacao_aux.op,
            &operacao_aux.atributo
        ) != EOF)
    {
        operacoes[pos-1] = operacao_aux;
    }

    num_operacoes = pos;

    printf("entrada\n");
      for (int i=0; i<num_operacoes; i++) {
          printf("%d %d %c %c\n", i+1, operacoes[i].id, operacoes[i].op, operacoes[i].atributo);
      }
    e = encontra_escalonamento(operacoes, num_operacoes);
/*
//começo de debbuger
    printf("estrutura de dados:\n");
    printf("ListaEscalonamentos(tamanho = %d):\n", e.tam_lista_escalonamento);

    for (int i=0; i<e.tam_lista_escalonamento; i++) {
        printf("Escalonamento %d possui %d transacoes: ", i+1, e.esc[i].num_transacoes);
        for (int j=0; j<e.esc[i].num_transacoes; j++)
            printf("%d, ", e.esc[i].id_transacoes[j]);

        printf("Com %d operacoes:\n", e.esc[i].num_operacoes);
        for (int j=0; j<e.esc[i].num_operacoes; j++)
            printf("%d %d %c %c\n", i+1, e.esc[i].operacoes[j].id, e.esc[i].operacoes[j].op, e.esc[i].operacoes[j].atributo);

    }
//fim do debugger
*/
//printa saida (WIP)

    // monta o grafo global para testar seriabilidade
    int n_transacoes_global = 0;
    for (int i=0; i < e.tam_lista_escalonamento; i++)
        n_transacoes_global += e.esc[i].num_transacoes;
    Grafo g = build_grafo(n_transacoes_global);


    for (int i=0; i < e.tam_lista_escalonamento; i++) {
        teste_seriabilidade(&e.esc[i], &g);

        for (int j=0; j < e.esc[i].num_transacoes; j++)
            printf("%d,", e.esc[i].id_transacoes[j]);
        //if
        printf("\n");
    }
}
