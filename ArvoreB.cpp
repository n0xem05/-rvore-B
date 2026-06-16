#include <bits/stdc++.h>
#include "ArvoreB.h"

using namespace std;

ArvoreB::ArvoreB(int grau){
    this->grau = grau;
    this->raiz = ArvoreB::criar_no_vazio();
    this->altura = 0;
}

ArvoreB::No* ArvoreB::criar_no_vazio(){
    No* no = new No();

    no->numero_chaves = 0;
    no->folha = true;
    no->chaves.resize(2 * this->grau - 1);
    no->filhos.resize(2 * this->grau);
    return no;
}

pair<ArvoreB::No*, int> ArvoreB::busca_Arvore_B(ArvoreB::No* raiz, int chave){
    int i = 0;

    while (i < raiz->numero_chaves && chave > raiz->chaves[i]) {
        i++;
    }
    
    if (i < raiz->numero_chaves && chave == raiz->chaves[i]) {
        return pair<ArvoreB::No*, int> (raiz, i);
    } else if (raiz->folha) {
        return pair<ArvoreB::No*, int> (nullptr, -1);
    } // se eu fosse realmente acessar o disco, teria um método que leria o disco atrás do nó filho

    return ArvoreB::busca_Arvore_B(raiz->filhos[i], chave);
}

pair<ArvoreB::No*, int> ArvoreB::busca_Arvore_B(int chave){
    if (this->raiz->numero_chaves == 0) {
        return pair<ArvoreB::No*, int> (nullptr, -1);
    }

    return ArvoreB::busca_Arvore_B(this->raiz, chave);
}

void ArvoreB::inserir_chave_nao_cheio(int nova_chave, ArvoreB::No* no){
    int i = no->numero_chaves - 1;

    if (no->folha) {
        while (i >= 0 && nova_chave < no->chaves[i]){
            no->chaves[i+1] = no->chaves[i];
            i--;
        }

        no->chaves[i + 1] = nova_chave;
        no->numero_chaves++;
    } else {
        while (i >= 0 && nova_chave < no->chaves[i]) {
            i--;
        }
        i++;

        if (no->filhos[i]->numero_chaves == 2 * this->grau - 1) {
            ArvoreB::dividir_no(no, i);

            if (nova_chave > no->chaves[i]){
                i++;
            }
        }

        ArvoreB::inserir_chave_nao_cheio(nova_chave, no->filhos[i]);
    }
}

bool ArvoreB::inserir_chave(int nova_chave){
    if (this->raiz->numero_chaves == 0) {
        this->raiz->chaves[0] = nova_chave;
        this->raiz->numero_chaves = 1;
        return true;
    }

    if (ArvoreB::busca_Arvore_B(this->raiz, nova_chave).second == -1) {
        if (this->raiz->numero_chaves == 2 * this->grau - 1) {
            No* no = ArvoreB::criar_no_vazio();
            no->folha = false;
            no->filhos[0] = this->raiz;
            this->raiz = no;
            this->altura++;

            ArvoreB::dividir_no(this->raiz, 0);
            ArvoreB::inserir_chave_nao_cheio(nova_chave, this->raiz);
        } else {
            ArvoreB::inserir_chave_nao_cheio(nova_chave, this->raiz);
        }

        return true;
    }

    return false;
}

void ArvoreB::pegar_esquerda(ArvoreB::No* pai, int ind){
    No* filho = pai->filhos[ind];
    No* irmao = pai->filhos[ind - 1];

    // abrir espaço pra nova chave
    for (int j = filho->numero_chaves - 1; j >= 0; j--) {
        filho->chaves[j + 1] = filho->chaves[j];
    }

    // abrir espaço pro novo filho
    if (!filho->folha){
        for (int j = filho->numero_chaves; j >= 0; j--) {
            filho->filhos[j + 1] = filho->filhos[j];
        }
    }

    filho->chaves[0] = pai->chaves[ind - 1];

    // pega o ultimo filho do irmao por ser maior que a ultima chave e menor que a chave que vai descer
    if (!filho->folha) {
        filho->filhos[0] = irmao->filhos[irmao->numero_chaves];
    }

    // desce a chave pro filho
    pai->chaves[ind - 1] = irmao->chaves[irmao->numero_chaves - 1];

    filho->numero_chaves++;
    irmao->numero_chaves--;
}

void ArvoreB::pegar_direita(ArvoreB::No* pai, int ind){
    No* filho = pai->filhos[ind];
    No* irmao = pai->filhos[ind + 1];

    // no final não precisa mover
    filho->chaves[filho->numero_chaves] = pai->chaves[ind];

    if (!filho->folha) {
        filho->filhos[filho->numero_chaves + 1] = irmao->filhos[0]; 
    }

    pai->chaves[ind] = irmao->chaves[0];

    // reorganiza as chaves do irmao
    for (int j = 0; j < irmao->numero_chaves - 1; j++) {
        irmao->chaves[j] = irmao->chaves[j + 1];
    }

    // reorganiza os filhos do irmao
    if (!irmao->folha) {
        for (int j = 0; j <= irmao->numero_chaves - 1; j++) {
            irmao->filhos[j] = irmao->filhos[j + 1];
        }
    }

    filho->numero_chaves++;
    irmao->numero_chaves--;
}

void ArvoreB::unir_no(ArvoreB::No* pai, int ind){
    No* esquerda = pai->filhos[ind];
    No* direita = pai->filhos[ind + 1];
    esquerda->chaves[this->grau - 1] = pai->chaves[ind];

    for (int j = 0; j < direita->numero_chaves; j++) {
        esquerda->chaves[j + this->grau] = direita->chaves[j];
    }

    if (!esquerda->folha) {
        for (int j = 0; j <= direita->numero_chaves; j++) {
            esquerda->filhos[j + this->grau] = direita->filhos[j];
        }
    }

    esquerda->numero_chaves += direita->numero_chaves + 1;

    for (int j = ind; j < pai->numero_chaves - 1; j++) {
        pai->chaves[j] = pai->chaves[j + 1];
    }

    for (int j = ind + 1; j <= pai->numero_chaves; j++) {
        pai->filhos[j] = pai->filhos[j + 1];
    }

    pai->numero_chaves--;

    delete direita;
}

int ArvoreB::estar_no_no(ArvoreB::No* no, int chave) {
    int i = 0;
    while (i < no->numero_chaves && no->chaves[i] < chave) {
        i++;
    }

    return i;
}

void ArvoreB::remover_chave(ArvoreB::No* pai, int chave) {
    int i = ArvoreB::estar_no_no(pai, chave);
    if (i < pai->numero_chaves && pai->chaves[i] == chave) {
        if (pai->folha) {
            for (int j = i; j < pai->numero_chaves - 1; j++) {
                pai->chaves[j] = pai->chaves[j + 1]; 
            }

            pai->numero_chaves--;
        } else {
            if (pai->filhos[i]->numero_chaves >= this->grau){
                No* aux = pai->filhos[i];
                while (!aux->folha) {
                    aux = aux->filhos[aux->numero_chaves];
                }

                int predecessor = aux->chaves[aux->numero_chaves - 1];

                pai->chaves[i] = predecessor;

                ArvoreB::remover_chave(pai->filhos[i], predecessor);
            } else if (pai->filhos[i + 1]->numero_chaves >= this->grau) {
                No* aux = pai->filhos[i + 1];
                while (!aux->folha) {
                    aux = aux->filhos[0];
                }

                int sucessor = aux->chaves[0];

                pai->chaves[i] = sucessor;

                ArvoreB::remover_chave(pai->filhos[i + 1], sucessor);
            } else {
                ArvoreB::unir_no(pai, i);

                ArvoreB::remover_chave(pai->filhos[i], chave);
            }
        }

        return;
    } // e se a chave não tiver no nó

    if (pai->folha) {
        return;
    }

    bool ultimo = false;
    if (i == pai->numero_chaves) {
        ultimo = true;
    }

    No* filho = pai->filhos[i];

    if (filho->numero_chaves == this->grau - 1){

        if (i > 0 && pai->filhos[i - 1]->numero_chaves >= this->grau) {
            ArvoreB::pegar_esquerda(pai, i);
        } else if (i < pai->numero_chaves && pai->filhos[i + 1]->numero_chaves >= this->grau) {
            pegar_direita(pai, i);
        } else { 
            if (i < pai->numero_chaves)
                unir_no(pai, i);
            else{
                unir_no(pai, i - 1);
                i--; // pq ele se move pra frente
            }
        }
    }
    
    if (ultimo && i > pai->numero_chaves) {
        ArvoreB::remover_chave(pai->filhos[i - 1], chave);
    } else {
        ArvoreB::remover_chave(pai->filhos[i], chave);
    }
}

bool ArvoreB::remover_chave(int chave){
    if (ArvoreB::busca_Arvore_B(this->raiz, chave).second == -1) {
        return false;
    }

    ArvoreB::remover_chave(this->raiz, chave);

    return true;
}

void ArvoreB::dividir_no(ArvoreB::No* pai, int ind){
    No* no_esquerda = pai->filhos[ind];

    No* no_direita = ArvoreB::criar_no_vazio();

    no_direita->folha = no_esquerda->folha;
    no_direita->numero_chaves = this->grau - 1;

    for (int i = 0; i < this->grau - 1; i++) {
        no_direita->chaves[i] = no_esquerda->chaves[i + this->grau];
    }
    
    if (!no_esquerda->folha) {
        for (int i = 0; i < this->grau; i++) {
            no_direita->filhos[i] = no_esquerda->filhos[i + this->grau];
        }
    }

    no_esquerda->numero_chaves = this->grau - 1;

    for (int i = pai->numero_chaves; i >= ind + 1; i--) {
        pai->filhos[i + 1] = pai->filhos[i];
    }

    pai->filhos[ind + 1] = no_direita;

    for (int i = pai->numero_chaves - 1; i >= ind; i--) {
        pai->chaves[i + 1] = pai->chaves[i];
    }

    pai->chaves[ind] = no_esquerda->chaves[this->grau - 1];
    pai->numero_chaves++;
}

void ArvoreB::destruir_arvore(ArvoreB::No* no){
    if (no == nullptr)
        return;

    if (!no->folha){
        for (int i = 0; i <= no->numero_chaves; i++){
            destruir_arvore(no->filhos[i]);
        }
    }

    delete no;
}

void ArvoreB::destruir_arvore(){
    destruir_arvore(this->raiz);
}