#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <bits/stdc++.h>
#include "../headers/utils.h"
#include "../headers/transacao.h"
#include "../headers/graph.h"

using namespace std;

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

void print_grafo(Grafo *g){
    for(int i = 0; i < g->n_vertices; i++){
        for(int j = 0; j < g->n_vertices; j++){
            cout << g->m_adjacencia[i][j] << " ";
        }
        cout << "\n";
    }
}

int teste_seriabilidade(struct Escalonamento *e, Grafo *g) {

    // operações auxiliares i e j
    struct Operacao op_i, op_j;

    // Cria as arestas no grafo

    for(struct Operacao op_i: e->operacoes){
        for(struct Operacao op_j: e->operacoes){

            // caso no qual transações diferentes acessam o mesmo atributo
            bool conflito_transacao = (op_j.id != op_i.id) && (op_j.atributo == op_i.atributo) && (op_j.op != 'C' && op_i.op != 'C');

            if(conflito_transacao){

                // caso no qual existe um Write antes de um Read
                // Read antes de um Write
                // e Write antes de um Write.
                bool cria_aresta = (op_j.op != op_i.op) || (op_j.op == 'W' && op_i.op == 'W');

                // cria uma aresta entre i e j na matriz adjacente
                if(cria_aresta) g->m_adjacencia[op_i.id - 1][op_j.id - 1] = 1;
            }
        }
    }
    // verifica se o grafo é cíclico.

    return !detectaCiclo(g);
}

int fatorial(int n){
    if(n == 0) return 1;
    return fatorial(n - 1) * n;
}

bool compara_operacao(struct Operacao op1, struct Operacao op2){
    bool igual = true;
    igual &= op1.pos == op2.pos;
    igual &= op1.id == op2.id;
    igual &= op1.op == op2.op;
    igual &= op1.atributo == op2.atributo;
    return igual;
}

void print(struct Operacao op){
    cout << op.pos << " T"<< op.id << " " << op.op << " "<<op.atributo <<"\n";
}

bool verifica_visao(std::vector<struct Operacao> v1, std::vector<struct Operacao> v2){

    vector<struct Operacao> ultimas_escritas;

    struct Operacao op2_i, op2_j;
    struct Operacao opi, opj, opk;
    struct Operacao ultima_escrita, ultima_escrita_v2;
    bool found = true, escrita_anterior;

    // operações em ordem inversa
    vector<struct Operacao> inverse_v1(v1);
    vector<struct Operacao> inverse_v2(v2);
    reverse(inverse_v1.begin(), inverse_v1.end());
    reverse(inverse_v2.begin(), inverse_v2.end());

    // para cada operação na primeira visão

    for(int i = v1.size() - 1; i >= 0; i--){
        opi = v1[i];
        if(opi.op == 'R'){
            ultima_escrita = opi;
            for(int j = i - 1; j >= 0; j--){
                opj = v1[j];

                escrita_anterior = opj.id != opi.id && opj.atributo == opi.atributo && opj.op == 'W';
                if(escrita_anterior) ultima_escrita = opj;
            }

            for(int k = v2.size() - 1; k >= 0; k--){
                opk = v2[k];
                if(compara_operacao(opi, opk)){
                    ultima_escrita_v2 = opk;
                    for(int j = k - 1; j >= 0; j--){
                        opj = v2[j];
                        escrita_anterior = opj.id != opk.id && opk.atributo == opj.atributo && opj.op == 'W';

                        if(escrita_anterior) ultima_escrita_v2 = opj;
                    }
                }
            }

            if(!compara_operacao(ultima_escrita, ultima_escrita_v2)) return false;
        }
    }
    // guarda os atributos já vistos
    vector<char> visitados;

    // iteramos sobre todas as operações da visão 1 na ordem invertida (final->começo)
    // de forma que sabemos qual foi o último WRITE do atributo
    for(struct Operacao op_i: inverse_v1){

        // caso aonde achamos uma operação de WRITE de um atributo qualquer
        bool found_write = (op_i.op == 'W') && (!count(visitados.begin(), visitados.end(), op_i.atributo));
        if(found_write){

            // procuramos uma outra operação WRITE no mesmo atributo mas em transações diferentes
            // também iteramos em ordem invertida
            for(struct Operacao op_j: inverse_v2){
                if(op_j.op == 'W' && op_j.atributo == op_i.atributo){

                    // se o primeiro WRITE achado da transação serial corresponde ao WRITE da visão 1
                    // então a ordem dos WRITES continua a mesma.
                    // retornamos false caso contrário.
                    if(!compara_operacao(op_i, op_j)) return false;
                    break;
                }
            }

        // garante que atributos iguais não se repitam
        visitados.push_back(op_i.atributo);
        }
    }

    // se a execução chegou até aqui, todas as condições foram verificadas e
    // as duas visões são iguais.
    return true;
}

bool teste_equivalencia_visao(struct Escalonamento *e){
    vector<int> permutacao(e->id_transacoes);

    int n_transacoes = e->id_transacoes.size();

    vector<struct Operacao> operacoes_seriais;
    // computa todas as permutações
    for(int i = 1; i <= fatorial(n_transacoes); i++){
        // adiciona as operações em série
        for(int id_transacao: permutacao){
            for(struct Operacao op: e->operacoes){
                if(op.id == id_transacao) operacoes_seriais.push_back(op);
            }
        }

        // verifica se existe uma permutação serial que possui mesma visão

        if(verifica_visao(e->operacoes, operacoes_seriais)) return true;

        next_permutation(permutacao.begin(), permutacao.end());
        operacoes_seriais.clear();
    }
    return false;
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
/*
    printf("entrada\n");
    for (struct Operacao x: operacoes) {
          printf("%d %d %c %c\n", x.pos, x.id, x.op, x.atributo);
    }
*/
    escalonamentos = encontra_escalonamento(operacoes);


//começo de debbuger
printf("------------------------------------------------------------\n");
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
printf("------------------------------------------------------------\n");
//fim do debugger


    // monta o grafo global
    int tamanho_problema = 0;
    for (struct Escalonamento esc: escalonamentos){
        tamanho_problema += esc.id_transacoes.size();
    }
    Grafo *g = new Grafo(tamanho_problema);

    for (int k=0; k < escalonamentos.size(); k++) {
//        teste_seriabilidade(&e.esc[i]);
        //cout << teste_seriabilidade(&esc, g);
        //int i = teste_seriabilidade(&esc, g);
        teste_equivalencia_visao(&escalonamentos[k]);
        //teste_seriabilidade(&escalonamentos[k], g);

        printf("%d %d", k+1, escalonamentos[k].id_transacoes[0]);
        for (int i=1; i<escalonamentos[k].id_transacoes.size(); i++)
            printf(",%d", escalonamentos[k].id_transacoes[i]);

        if (teste_seriabilidade(&escalonamentos[k], g))
            printf(" SS");
        else
            printf(" NS");

        if (teste_equivalencia_visao(&escalonamentos[k]))
            printf(" SV");
        else
            printf(" NV");

        printf("\n");

        printf("%d %d\n", teste_equivalencia_visao(&escalonamentos[k]), teste_seriabilidade(&escalonamentos[k], g));
        cout << "\n";
        print_grafo(g);
        cout << "\n";
        reinicia_grafo(g);
    }
}
