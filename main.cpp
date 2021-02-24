#include <stdio.h>
#include <algorithm>
#include "utils.h"
#include "transacao.h"
#include "grafo.h"

std::vector<struct Escalonamento> encontra_escalonamento(std::vector<struct Operacao> operacoes) {

    int num_abertos, i, aux;
    struct Escalonamento e_aux;
    std::vector<int> abertos, v_aux;
    std::vector<struct Escalonamento> escalonamentos;

    abertos.resize(max_id(operacoes)+1);

    i = 0;

    while (i < operacoes.size()) {
        num_abertos = 0;
        e_aux = {};
        
        do {

            e_aux.operacoes.push_back(operacoes[i]);

            //abre transacao do id tal
            if ((abertos[operacoes[i].id] != 1) && (operacoes[i].op != 'C')) {
                abertos[operacoes[i].id] = 1;
                num_abertos++;
                e_aux.id_transacoes.push_back(operacoes[i].id);
            }
            //fecha transacao do id tal
            if ((abertos[operacoes[i].id] == 1) && (operacoes[i].op == 'C')) {
                abertos[operacoes[i].id] = 0;
                num_abertos--;
            }
            i++;
    
        } while ((num_abertos != 0) && (i < operacoes.size()));

        std::sort(e_aux.id_transacoes.begin(), e_aux.id_transacoes.end());
        e_aux.id_transacoes.erase(std::unique(
            e_aux.id_transacoes.begin(), e_aux.id_transacoes.end()
        ), e_aux.id_transacoes.end());
        escalonamentos.push_back(e_aux);

        aux = 0;
        for (int x: abertos)
            aux += x;        

        if (aux != 0) {
            printf("PANIC!!!! Alguma transação não foi commitada\n");
        }

    }

    return escalonamentos;

}

int teste_seriabilidade(struct Escalonamento *e) {

    //struct Grafo g = build_graph(e->num_transacoes);

    

}

int main() {

    struct Operacao operacao_aux;
    std::vector<struct Operacao> operacoes;
    std::vector<struct Escalonamento> escalonamentos;

    while (
        scanf("%d %d %c %c", 
            &operacao_aux.pos, 
            &operacao_aux.id, 
            &operacao_aux.op,
            &operacao_aux.atributo
        ) != EOF)
    {
        operacoes.push_back(operacao_aux);
    }

    printf("entrada\n");
      for (struct Operacao x: operacoes) {
          printf("%d %d %c %c\n", x.pos, x.id, x.op, x.atributo);
      }

    escalonamentos = encontra_escalonamento(operacoes);


//começo de debbuger
    printf("estrutura de dados:\n");
    printf("ListaEscalonamentos(tamanho = %d):\n", (int)escalonamentos.size());

    for (struct Escalonamento esc: escalonamentos) {
        printf("O seguinte escalonamento possui %d transacoes: ", (int)esc.id_transacoes.size());
        for (int x: esc.id_transacoes)
            printf("%d, ", x);

        printf("Com %d operacoes:\n", (int)esc.operacoes.size());
        for (struct Operacao o: esc.operacoes)
            printf("%d %d %c %c\n", o.pos, o.id, o.op, o.atributo);

    }
//fim do debugger


//printa saida (WIP)
    for (struct Escalonamento esc: escalonamentos) {
//        teste_seriabilidade(&e.esc[i]);
        for (int t: esc.id_transacoes)
            printf("%d,", t);
        //if 
        printf("\n");
    }
}
