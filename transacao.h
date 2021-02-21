#ifndef transacao
#define transacao

struct Transacao {

    int id;
    char operacao;
    char atributo;

} transacao;

struct Escalonamento {

    int *id_transacoes;
    char serial;
    char visao;

} escalonamento;

struct ListaEscalonamentos {

    struct Escalonamento *esc;
    int tam_lista_escalonamento;
    int tam_id_transacoes;

} lista_escalonamento;

#endif
