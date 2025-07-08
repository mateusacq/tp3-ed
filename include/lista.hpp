//---------------------------------------------------------------------
// Arquivo      : evento.hpp
// Conteudo     : header da estrutura de dados Lista
// Autor        : Mateus Antinossi Cordeiro Queiroz (mateusacq@ufmg.br)
// Historico    : 2025-07-01 - arquivo criado
//---------------------------------------------------------------------

#ifndef LISTA_HPP
#define LISTA_HPP

#include <iostream> // Para mensagens de erro

// tipo das chaves da lista (ChaveL)
using ChaveL = int;

template<typename CelulaL, typename ItemL>
class Lista {
public:
    // Construtor padrao
    Lista() {
        primeiro = new CelulaL();
        ultimo = primeiro;
        tamanho = 0;
    }
    // Construtor de copia
    Lista(const Lista& outra) {
        primeiro = new CelulaL();
        ultimo = primeiro;
        tamanho = 0;

        CelulaL *p = outra.primeiro->prox;
        while (p != nullptr) {
            this->InsereFinal(p->item);
            p = p->prox;
        }
    }
    // Sobrecarga do operador de atribuicao
    Lista& operator=(const Lista& outra) {
        if (this == &outra) {
            return *this;
        }

        Limpa();

        CelulaL *p = outra.primeiro->prox;
        while (p != nullptr) {
            this->InsereFinal(p->item);
            p = p->prox;
        }

        return *this;
    }
    // Destrutor
    ~Lista(){ Limpa(); delete primeiro; }

    // Getters e Setters
    int GetTamanho() { return tamanho; }

    CelulaL* GetPrimeira() {
        if (Vazia()) {
            return nullptr;
        }
        return primeiro->prox;
    }
    CelulaL* GetUltima() {
        if (Vazia()) {
            return nullptr;
        }
        return ultimo;
    }

    CelulaL * Posiciona(int pos) {
        // Seguranca contra seg fault
        if ( (pos > tamanho + 1) || (pos <= 0) ) {
            std::cerr << "ERRO: Posicao Invalida!";
            return nullptr;
        }

        CelulaL *p = primeiro->prox;
        for (int i = 1; i < pos; i++) {
            p = p->prox;
        }

        return p;
    }
    
    // Metodos de insercao
    void InsereFinal(ItemL item) {
        CelulaL *nova = new CelulaL();
        nova->item = item;
        // O novo ultimo sempre aponta para nullptr
        nova->prox = nullptr;

        if (tamanho == 0) {
            primeiro->prox = nova;
        } else {
            ultimo->prox = nova;
        }
        ultimo = nova; // "nova" agora eh o ultimo elemento
        tamanho++;
    }

    // Encontra posicao de item na lista
    int Pesquisa(ChaveL c) {
        if (tamanho == 0) {
            // std::cerr << "ERRO: Lista vazia! Pesquisa\n";
            return -1;
        }

        int pos = 1;
        bool encontrou = false;
        CelulaL * p = primeiro->prox;
        while (p != nullptr) {
            if (p->item.GetChave() == c) {
                encontrou = true;
                break;
            }
            p = p->prox;
            pos++;
        }

        // Se nao encontrar nada, retorna -1, valor do construtor padrao de aux
        return encontrou ? pos : -1;
    }

    void Limpa() {
        CelulaL *atual;

        atual = primeiro->prox;
        while ( atual!= nullptr) {
            primeiro->prox = atual->prox;
            delete atual;
            atual = primeiro->prox;
        }
        primeiro->prox = nullptr; // Evita dangling pointer na sentinela
        ultimo = primeiro;
        tamanho = 0;
    }
    bool Vazia()  {
        return tamanho == 0;
    }

private:
    CelulaL * primeiro;
    CelulaL * ultimo;

protected:
    int tamanho;

    friend class ListaEventos;
};

#endif