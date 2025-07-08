//---------------------------------------------------------------------
// Arquivo      : evento.hpp
// Conteudo     : header da estrutura de dados Heap
// Autor        : Mateus Antinossi Cordeiro Queiroz (mateusacq@ufmg.br)
// Historico    : 2025-07-01 - arquivo criado
//---------------------------------------------------------------------

#ifndef HEAP_HPP
#define HEAP_HPP

#include <iostream>

#include "evento.hpp"

using TipoH = Evento;

class Heap { /* ### MINHEAP QUE SERVIRA DE ESCALONADOR DE EVENTOS ### */

    public:
        Heap();
        Heap(int maxsize);
        ~Heap();

        void InsereEvento(TipoH x);
        TipoH RetiraProximoEvento();

        //Retorna true caso o heap esteja vazio, false caso contrário.
        bool Vazio() const;

    private:
        int GetAncestral(int posicao) const;
        int GetSucessorEsq(int posicao) const;
        int GetSucessorDir(int posicao) const;

        int tamanho;
        int capacidade; // Para evitar estouro de tamanho do heap
        TipoH * data;

        /* Funções necessárias para implementar o Heapify recursivo
         * Você pode apagar elas caso decida implementar o Heapify iterativo
         */
        void HeapifyPorBaixo(int posicao);
        void HeapifyPorCima(int posicao);
};

#endif