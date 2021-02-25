#include <stdlib.h>
#include <bits/stdc++.h>
#include "../headers/transacao.h"

int max_id(std::vector<struct Operacao> operacoes) {

    int max = 0;

    for (struct Operacao x: operacoes)
        if (max < x.id)
            max = x.id;

    return max;

}
