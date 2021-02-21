#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "transacao.h"

int max_id(struct Transacao *transacoes, int num_transacoes) {

    int max = 0;

    for (int i=0; i<num_transacoes; i++)
        if (max < transacoes[i].id)
            max = transacoes[i].id;

    return max;

}

struct ListaEscalonamentos encontra_escalonamento(struct Transacao *transacoes, int num_transacoes) {

    int num_abertos, *abertos, tam_abertos, i, aux, j;

    struct ListaEscalonamentos e;

    tam_abertos = max_id(transacoes, num_transacoes);
    abertos = (int *)malloc((tam_abertos+1)*sizeof(int));

    for (int i=0; i<=tam_abertos; i++)
        abertos[i] = 0;

    e.esc = (struct Escalonamento *)malloc(num_transacoes*sizeof(struct Escalonamento));
    e.tam_lista_escalonamento = 0;
    e.tam_id_transacoes = tam_abertos;

    num_abertos = 0;
    i = 0;
    j = 0;
    while (i < num_transacoes) {

        e.esc[j].id_transacoes = (int *)malloc(e.tam_id_transacoes*sizeof(int));
        e.tam_lista_escalonamento += 1;
        for(int k=0; k<e.tam_id_transacoes; k++)
            e.esc[j].id_transacoes[k] = 0;
        
        do {

            //abre transacao do id tal
            if ((abertos[transacoes[i].id] != 1) && (transacoes[i].operacao != 'C')) {
                abertos[transacoes[i].id] = 1;
                num_abertos++;
                e.esc[j].id_transacoes[transacoes[i].id] = 1;
            }
            //fecha transacao do id tal
            if ((abertos[transacoes[i].id] == 1) && (transacoes[i].operacao == 'C')) {
                abertos[transacoes[i].id] = 0;
                num_abertos--;
            }
            i++;
    
        } while ((num_abertos != 0) && (i < num_transacoes));

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

void teste_seriabilidade(struct Transacao transacoes, int num_transacoes) {
    
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
        for (int j=1; j <= e.tam_id_transacoes; j++)
            if (e.esc[i].id_transacoes[j] == 1)
                printf("%d,", j);
        //if 
        printf("\n");
    }
}
