#ifndef transacao
#define transacao

struct Operacao {

    int id;
    char op;
    char atributo;

} operacao;

struct Escalonamento {

    int *id_transacoes;
    int num_transacoes;
    struct Operacao *operacoes;
    int num_operacoes;

} escalonamento;

struct ListaEscalonamentos {

    struct Escalonamento *esc;
    int tam_lista_escalonamento;

} lista_escalonamento;

#endif
