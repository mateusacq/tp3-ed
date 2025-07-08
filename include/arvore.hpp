//---------------------------------------------------------------------
// Arquivo      : arvore.hpp
// Conteudo     : implementação da estrutura de dados Árvore AVL 
// Autor        : Mateus Antinossi Cordeiro Queiroz (mateusacq@ufmg.br)
// Historico    : 2025-07-01 - arquivo criado
//---------------------------------------------------------------------

#ifndef ARVORE_HPP
#define ARVORE_HPP

#include <iostream>

int max(int a, int b) {
    return a > b ? a : b;
}

template<typename Chave, typename Item>
class ArvoreAVL
{
    // Definicao interna de no para usar valores dos templates
    class No {
    public:
        Chave chave;
        Item valor;
        No *esq;
        No *dir;
        int altura;

        // Construtor padrao
        No() : chave(), valor(), 
        esq(nullptr), dir(nullptr), altura(0) {}

        // Construtor 
        No(const Chave& c, const Item& v) : chave(c), valor(v), 
        esq(nullptr), dir(nullptr), altura(0) {}

        No(const No&) = delete; // Nao sera usado, arvore ja gerencia a memoria

        // Destrutor padrao de No ja chama os destrutores dos tipos Item e Chave. 
        // Ponteiros esq e dir gerenciados nos metodos internos.
    };
    
    public:
        ArvoreAVL() : raiz(nullptr), tamanho(0) {}
        ~ArvoreAVL() { Limpa(); };
        // Construtor de copia
        ArvoreAVL(const ArvoreAVL& outra) : raiz(nullptr), tamanho(0) {
            this->raiz = CopiaRecursiva(outra.raiz);
            this->tamanho = outra.tamanho;
        }
        // Sobrecarga do operador de atribuicao
        ArvoreAVL& operator=(const ArvoreAVL& outra) {
            if (this == &outra) {
                return *this;
            }

            Limpa();

            this->raiz = CopiaRecursiva(outra.raiz);
            this->tamanho = outra.tamanho;

            return *this;
        }

        Item* Pesquisa(Chave chave) const { return PesquisaRecursivo(raiz, chave); }
        void Insere(Chave chave, Item item) { 
            bool inseriu = false;
            raiz = InsereRecursivo(raiz, chave, item, inseriu); 
            if (inseriu) {
                tamanho++;
            }
        }
        void Remove(Chave chave) { 
            bool removeu = false;
            raiz = RemoveRecursivo(raiz, chave, removeu); 
            if (removeu) {
                tamanho--;
            }
        }
        Item GetProxima() {
            if (Vazia()) {
                std::cerr << "ERRO: arvore vazia ao tentar obter o prox elemento";
                return Item();
            }
            // Encontra no com a menor chave (proximo no)
            No * menor = raiz;
            while (menor->esq != nullptr) {
                menor = menor->esq;
            }
            // Guarda raiz (proximo no)
            Item prox = menor->valor;
            Chave chaveMenor = menor->chave;

            // Remove o no com a menor chave
            bool removeu = false;
            raiz = RemoveRecursivo(raiz, chaveMenor, removeu);
            if (removeu) {
                tamanho--;
            }

            return prox;
        }

        void Limpa() {
            ApagaRecursivo(raiz);
            raiz = nullptr;
            tamanho = 0;
        }

        bool Vazia() { return tamanho == 0; }

        int GetTamanho() const { return tamanho; }

    private:
        No * raiz;
        int tamanho;

        int GetAltura(No * p) const { return p ? p->altura : 0; }
        // Getter do fator de Balanceamento
        int GetFB(No * p) const { return p ? ( GetAltura(p->dir) - GetAltura(p->esq) ) : 0; } 

        void AtualizaAltura(No * &p) {
            p->altura = 1 + max(GetAltura(p->esq), GetAltura(p->dir));
        }

        No * RotacionaDireita(No * p) {
            No * y = p->esq;
            No * A2 = y->dir;

            // Rotaciona para a direita em torno de p
            // (subarvore direita de y vira subarvore esquerda de p)
            y->dir = p;
            p->esq = A2;

            // Atualiza alturas
            AtualizaAltura(y);
            AtualizaAltura(p);

            return y; // Retorna a nova raiz da subarvore (novo 'p')
        }
        No * RotacionaEsquerda(No * p) {
            No * y = p->dir;
            No * A2 = y->esq;

            // Rotaciona para a esquerda em torno de p
            // (subarvore esquerda de y vira subarvore direita de p)
            y->esq = p;
            p->dir = A2;

            // Atualiza alturas
            AtualizaAltura(y);
            AtualizaAltura(p);

            return y; // Retorna a nova raiz da subarvore (novo 'p')
        }

        No * Balanceia(No * &p) {
            if (!p) return nullptr; // Se o no eh nulo, nao ha o que balancear

            // Atualiza a altura, por seguranca
            p->altura = 1 + max(GetAltura(p->esq), GetAltura(p->dir));

            int FB = GetFB(p);

            // Verifica cada um dos 4 casos
            if (FB < -1) { // Esquerda desbalanceada
                if (GetFB(p->esq) > 0) { 
                    p->esq = RotacionaEsquerda(p->esq); // Caso 3
                }
                return RotacionaDireita(p); // Caso 1
            }
            if (FB > 1) { // Direita desbaanceada
                if (GetFB(p->dir) < 0) {
                    p->dir = RotacionaDireita(p->dir); // Caso 4
                }
                return RotacionaEsquerda(p); // Caso 2
            }

            return p; // No balanceado
        }

        Item * PesquisaRecursivo(No *p, Chave c) const {
            if (p == nullptr) {
                return nullptr;
            }
        
            if (c < p->chave) {
                return PesquisaRecursivo(p->esq, c);
            } 
            else if (c > p->chave) {
                return PesquisaRecursivo(p->dir, c);
            } 
            else {
                return &p->valor;
            }
        }

        No* InsereRecursivo(No * &p, Chave c, Item i, bool& inseriu) {
            if (!p) {
                inseriu = true;
                return new No(c, i);
            } 

            if (c < p->chave) {
                p->esq = InsereRecursivo(p->esq, c, i, inseriu);
            } else if (c > p->chave) {
                p->dir = InsereRecursivo(p->dir, c, i, inseriu);
            } else { 
                p->valor = i; // Atualiza valor se chave ja existe
                return p;
            }

            AtualizaAltura(p);

            // Balanceia novo no inserido, caso chave nao existe
            return Balanceia(p); 
        }

        No* RemoveRecursivo(No * &p, Chave c, bool& removeu) {
            if (!p) {
                removeu = false;
                std::cerr << "Chave a ser removida nao encontrada em nenhum no da arvore\n";
                return nullptr; // Inidica que a chave não foi encontrada
            }
        
            if (c < p->chave) {
                p->esq = RemoveRecursivo(p->esq, c, removeu);
            } else if (c > p->chave) {
                p->dir = RemoveRecursivo(p->dir, c, removeu);
            } else { // Chave encontrada
                removeu = true;

                if (!p->esq || !p->dir) { // Caso simples: no com 0 ou 1 filho
                    No * aux = p->esq ? p->esq : p->dir;

                    if (!aux) { // No folha
                        delete p;
                        p = nullptr;
                    } else { // No com 1 filho
                        No * temp = p; // Copia dados
                        p = aux;
                        delete temp; // Deleta o nó original, substituido pelo filho
                    }
                } else { // Caso mais complicado
                    // Encontra o antecessor:
                    No * antecessor = p->esq;
                    while (antecessor->dir) {
                        antecessor = antecessor->dir;
                    }
                    // Copia dados do antecessor para o atual
                    p->chave = antecessor->chave;
                    p->valor = antecessor->valor;

                    // Remove o antecessor da posicao que ele estava
                    p->esq = RemoveRecursivo(p->esq, antecessor->chave, removeu);
                }
            }
            if (!p) { // Se o no foi deletado, p eh nullptr ou foi atualizado
                return p;
            }

            AtualizaAltura(p);
            return Balanceia(p);
        }

        void ApagaRecursivo(No * p) {
            if (p != nullptr) {
                ApagaRecursivo(p->esq);
                ApagaRecursivo(p->dir);
                delete p;
            }
        }

        // Metodo interno para construtor de copia
        No* CopiaRecursiva(const No* p) {
            if (p == nullptr) {
                return nullptr;
            }

            No* novoNo = new No(p->chave, p->valor);

            novoNo->esq = CopiaRecursiva(p->esq);
            novoNo->dir = CopiaRecursiva(p->dir);

            novoNo->altura = p->altura;

            return novoNo;
        }
};

#endif