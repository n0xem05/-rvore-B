#ifndef ARVORE_B_H
#define ARVORE_B_H

#include <bits/stdc++.h>

using namespace std;

class ArvoreB
{
private:
    int grau;
    int altura;
    
    struct No
    {
        int numero_chaves;
        vector<int> chaves;
        vector<No*> filhos;
        bool folha;
    };

    No* raiz;
    
public:
    ArvoreB(int grau);
    pair<No*, int> busca_Arvore_B(int chave);
    bool inserir_chave(int nova_chave);
    bool remover_chave(int chave);
    void destruir_arvore();
    // fazer uns getter básicos
    
    protected:
    pair<No*, int> busca_Arvore_B(No* raiz, int chave);
    void inserir_chave_nao_cheio(int nova_chave, No* no);
    No* ArvoreB::criar_no_vazio();
    void dividir_no(No* pai, int ind);
    void remover_chave(No* pai, int chave);
    int estar_no_no(No* no, int chave);
    void unir_no(No* no, int ind);
    void pegar_esquerda(No* pai, int ind);
    void pegar_direita(No* pai, int ind);
    void destruir_arvore(No* no);
};

#endif