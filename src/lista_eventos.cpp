//---------------------------------------------------------------------
// Arquivo      : lista_eventos.cpp
// Conteudo     : implementação da classe ListaEventos, filha da classe Lista
// Autor        : Mateus Antinossi Cordeiro Queiroz (mateusacq@ufmg.br)
// Historico    : 2025-07-01 - arquivo criado
//---------------------------------------------------------------------

#include "lista_eventos.hpp"

ItemE::ItemE() {
    tempo = -1;
    evento = nullptr;
}

ItemE::ItemE(ChaveE t, Evento * e) : tempo(t) {
    if (e) {
        this->evento = new Evento(*e); // Copia profunda
    } else {
        this->evento = nullptr;
    }
}

ItemE::ItemE(const ItemE& outro) : tempo(outro.tempo) {
    if (outro.evento) {
        this->evento = new Evento(*(outro.evento));
    } else {
        this->evento = nullptr;
    }
}

ItemE& ItemE::operator=(const ItemE& outro) {
    if (this == &outro) { return *this; } // Previne autoatribuicao

    delete this->evento;
    this->evento = nullptr; // Evita dangling pointer

    this->tempo = outro.tempo;
    if (outro.evento) {
        this->evento = new Evento(*(outro.evento));
    } else {
        this->evento = nullptr;
    }

    return *this;
}

ItemE::~ItemE() {
    delete evento;
}

void ItemE::SetChave(ChaveE t) {
    tempo = t;
}

ChaveL ItemE::GetChave() {
    return tempo;
}

Evento * ItemE::GetEvento() const {
    return evento;
}

CelulaE::CelulaE() {
    item = ItemE();
    prox = nullptr;
}

const ItemE& CelulaE::GetItem() const {
    return item; // Retorna referencia constante
}

// Imprime todos os eventos na lista
void ListaEventos::Imprime() const {
    CelulaE * atual = primeiro->prox;
    while (atual) {
        atual->item.GetEvento()->imprimeEvento();
        atual = atual->prox;
    }
}

// Metodo de insercao especializado
void ListaEventos::InsereEvento(Evento * e) {
    ItemE novoItem(e->getDataHora(), e);
    InsereFinal(novoItem);
}