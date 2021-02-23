#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "transacao.h"
#include "grafo.h"

int max_id(struct Transacao *transacoes, int num_transacoes) {

    int max = 0;

    for (int i=0; i<num_transacoes; i++)
        if (max < transacoes[i].id)
            max = transacoes[i].id;

    return max;

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

struct ListaEscalonamentos encontra_escalonamento(struct Transacao *transacoes, int num_transacoes) {

    int num_abertos, *abertos, tam_abertos, i, aux, j, *v_aux;

    struct ListaEscalonamentos e;

    tam_abertos = max_id(transacoes, num_transacoes)+1;
    abertos = (int *)malloc(tam_abertos*sizeof(int));

    for (int i=0; i<=tam_abertos; i++)
        abertos[i] = 0;

    e.esc = (struct Escalonamento *)malloc(num_transacoes*sizeof(struct Escalonamento));
    e.tam_lista_escalonamento = 0;

    num_abertos = 0;
    i = 0;
    j = 0;
    v_aux = (int *)malloc(tam_abertos*sizeof(int));

    while (i < num_transacoes) {

        e.tam_lista_escalonamento += 1;
        e.esc[j].num_transacoes = 0;

        for(int k=0; k<tam_abertos; k++)
            v_aux[k] = 0;
        
        do {

            //abre transacao do id tal
            if ((abertos[transacoes[i].id] != 1) && (transacoes[i].operacao != 'C')) {
                abertos[transacoes[i].id] = 1;
                num_abertos++;
                v_aux[transacoes[i].id] = 1;
                e.esc[j].num_transacoes++;
            }
            //fecha transacao do id tal
            if ((abertos[transacoes[i].id] == 1) && (transacoes[i].operacao == 'C')) {
                abertos[transacoes[i].id] = 0;
                num_abertos--;
            }
            i++;
    
        } while ((num_abertos != 0) && (i < num_transacoes));

        e.esc[j].id_transacoes = converte_indice_vetor(v_aux, tam_abertos, e.esc[j].num_transacoes);

        j++;

        aux = 0;
        for (int k=0; k<=tam_abertos; k++)
            aux += abertos[k];        

        if (aux != 0) {
            printf("PANIC!!!! Alguma transação não foi commitada");
        }

    }

    return e;

}

void teste_seriabilidade(struct Escalonamento *e) {

//    struct Grafo g = build_graph(e->num_transacoes);


}

void main() {

    int pos, num_transacoes;
    struct Transacao transacoes[1000], transacao_aux;
    struct ListaEscalonamentos e;

    while (
        scanf("%d %d %c %c", 
            &pos, 
            &transacao_aux.id, 
            &transacao_aux.operacao,
            &transacao_aux.atributo
        ) != EOF)
    {
        transacoes[pos-1] = transacao_aux;
    }

    num_transacoes = pos;

    printf("entrada\n");
      for (int i=0; i<num_transacoes; i++) {
          printf("%d %d %c %c\n", i+1, transacoes[i].id, transacoes[i].operacao, transacoes[i].atributo);
      }

    e = encontra_escalonamento(transacoes, num_transacoes);

    for (int i=0; i < e.tam_lista_escalonamento; i++) {
//        teste_seriabilidade(&e.esc[i]);
        for (int j=0; j < e.esc[i].num_transacoes; j++)
            printf("%d,", e.esc[i].id_transacoes[j]);
        //if 
        printf("\n");
    }
}
