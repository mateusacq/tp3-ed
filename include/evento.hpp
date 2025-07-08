//---------------------------------------------------------------------
// Arquivo      : evento.cpp
// Conteudo     : implementação do TAD Evento
// Autor        : Mateus Antinossi Cordeiro Queiroz (mateusacq@ufmg.br)
// Historico    : 2025-07-01 - arquivo criado
//---------------------------------------------------------------------

#ifndef EVENTO_HPP
#define EVENTO_HPP

#include <string>   // para remetente e destinatario
#include <iostream> // para imprimeEvento()
#include <iomanip> // para imprimeEvento()

enum TipoEvento {
    INVALIDO = -1, // Evento invalido
    RG, // REGISTRO
    AR, // ARMAZENAMENTO
    RM, // REMOCAO
    UR, // REARMAZENAMENTO
    TR, // TRANSPORTE
    EN  // ENTREGA
};

class Evento {
public:
    // Construtor padrao
    Evento();

    // Construtor
    Evento(int dh, TipoEvento t, int idP, std::string r, std::string d,
           int org, int dst, int s);

    // Construtor de copia
    Evento(const Evento& outro);

    // Sobrecarga do operador <
    bool operator<(const Evento& outro) const;
    
    // Metodo de impressao usaado em consultas
    void imprimeEvento();

    // Getters
    int getDataHora();
    int getIdPacote();


private:
    int dataHora;
    TipoEvento tipo;
    static std::string eventoParaString(TipoEvento t);

    int idPacote;

    std::string remetente;
    std::string destinatario;

    int origem;
    int destino;
    int secao;

};

#endif