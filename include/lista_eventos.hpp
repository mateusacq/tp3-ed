//---------------------------------------------------------------------
// Arquivo      : lista_eventos.hpp
// Conteudo     : header da classe filha de lista ListaEventos
// Autor        : Mateus Antinossi Cordeiro Queiroz (mateusacq@ufmg.br)
// Historico    : 2025-07-01 - arquivo criado
//---------------------------------------------------------------------

#ifndef LISTA_EVENTOS_HPP
#define LISTA_EVENTOS_HPP

/*  LISTA PARA ACESSO AOS EVENTOS DE UM PACOTE

    Chave da lista de eventos: ChaveE
    Item da lista de eventos: ItemE
    Celula da lista de eventos: CelulaE 
*/

#include "lista.hpp"
#include "evento.hpp"

using ChaveE = ChaveL;

class ItemE {
public: 
    ItemE();
    ItemE(ChaveE t, Evento * e);
    ItemE(const ItemE& outro);
    ItemE& operator=(const ItemE& outro);
    ~ItemE();

    void SetChave(ChaveE tempo);
    
    ChaveE GetChave();
    Evento * GetEvento() const;

private:
    ChaveE tempo; // Chave = data e hora que o evento ocorreu
    Evento * evento;

    friend class CelulaE;
};

class CelulaE {
public:
    CelulaE();
    const ItemE& GetItem() const;

private:
    ItemE item;
    CelulaE * prox;

    template<typename, typename> friend class Lista; 
    friend class ListaEventos;
};

class ListaEventos : public Lista<CelulaE, ItemE> {
public:
    void Imprime() const;
    void InsereEvento(Evento * e);
};

#endif