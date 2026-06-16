#include <bits/stdc++.h>
#include "ArvoreB.h"

using namespace std;

int main(){
    ArvoreB arvB(2);
    arvB.inserir_chave(4);
    //arvB.inserir_chave(1);
    //arvB.inserir_chave(5);
    //arvB.inserir_chave(7);
    //arvB.inserir_chave(25);
    //arvB.inserir_chave(11);

    cout << arvB.remover_chave(4);
    cout << arvB.busca_Arvore_B(4).second;
    
    arvB.destruir_arvore();
    cout << arvB.busca_Arvore_B(4).second;
    return 0;
}