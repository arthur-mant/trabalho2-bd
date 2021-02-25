#ifndef transacao
#define transacao

#include <vector>

struct Operacao {

    int pos;
    int id;
    char op;
    char atributo;

};

struct Escalonamento {

    std::vector<int> id_transacoes;
    std::vector<struct Operacao> operacoes;

};

#endif
