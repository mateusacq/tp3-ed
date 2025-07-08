//---------------------------------------------------------------------
// Arquivo      : evento.hpp
// Conteudo     : header do TAD Evento
// Autor        : Mateus Antinossi Cordeiro Queiroz (mateusacq@ufmg.br)
// Historico    : 2025-07-01 - arquivo criado
//---------------------------------------------------------------------

#include "evento.hpp"

// Construtor padrao
Evento::Evento() : dataHora(-1), tipo(INVALIDO), idPacote(-1),
remetente(), destinatario(), origem(-1), destino(-1), secao(-1) {}

/* Construtor que garante que apenas os atributos
correspondentes ao tipo do evento sao lidos */
Evento::Evento(int dh, TipoEvento t, int idP, 
std::string r, std::string d, int org, int dst, int s) 
: dataHora(dh), tipo(t), idPacote(idP), 
remetente(""), destinatario(""), 
origem(-1), destino(-1), secao(-1) {
    switch (t) {
        case (0): // RG  - REGISTRO
            // Unico caso com remetente e destinatario
            remetente = r; 
            destinatario = d;
            origem = org; 
            destino = dst;
            // Sem secao de destino
            break;
        case (1): // AR - ARMAZENAMENTO
        case (2): // RM - REMOCAO
        case (3): // UR - REARMAZENAMENTO
            // Sem armazem de origem
            destino = dst;
            secao = s;
            break;
        case (4): // TR - TRANSPORTE
            origem = org; destino = dst;
            // Sem secao de destino
            break;
        case (5): // EN - ENTREGA
            // Sem armazem de origem
            destino = dst;
            // Sem secao de destino
            break;
        default:
            std::cerr << "Tipo de evento invalido ou desconhecido\n";
            break;
    }
}

// Construtor de copia
Evento::Evento(const Evento& outro) :
    dataHora(outro.dataHora),
    tipo(outro.tipo),
    idPacote(outro.idPacote),
    remetente(outro.remetente),
    destinatario(outro.destinatario),
    origem(outro.origem),
    destino(outro.destino),
    secao(outro.secao) {}

// funcao auxiliar para imprimeEvento()
std::string Evento::eventoParaString(TipoEvento t) { 
    switch (t) {
        case RG: return "RG";
        case AR: return "AR";
        case RM: return "RM";
        case UR: return "UR";
        case TR: return "TR";
        case EN: return "EN";
        default: return "??";
    }
}

void Evento::imprimeEvento() {
    std::cout << std::setfill('0') << std::setw(7) << dataHora
              << " EV " << eventoParaString(tipo) << " "
              << std::setfill('0') << std::setw(3) << idPacote;
    switch (tipo) {
        case (0): // RG  - REGISTRO
            std::cout << " " << remetente << " " << destinatario
                      << " " << std::setfill('0') << std::setw(3) << origem
                      << " " << std::setfill('0') << std::setw(3) << destino;
            break;
        case (1): // AR - ARMAZENAMENTO
        case (2): // RM - REMOCAO
        case (3): // UR - REARMAZENAMENTO
            std::cout << " " << std::setfill('0') << std::setw(3) << destino
                      << " " << std::setfill('0') << std::setw(3) << secao;
            break;
        case (4): // TR - TRANSPORTE
            std::cout << " " << std::setfill('0') << std::setw(3) << origem
                      << " " << std::setfill('0') << std::setw(3) << destino;
            break;
        case (5): // EN - ENTREGA
            std::cout << " " << std::setfill('0') << std::setw(3) << destino;
            break;
        default:
            std::cout << "EVENTO INVALIDO";
            break;
    } std::cout << std::endl;
}

int Evento::getDataHora() {
    return dataHora;
}

int Evento::getIdPacote() {
    return idPacote;
}

/* Sobrecarga de operador de comparacao para tratar
possiveis casos onde eh necessario desempate no
heap que servira de fila de prioridade
(simula chave) */
bool Evento::operator<(const Evento& outro) const {
    if (this->dataHora != outro.dataHora) {
        return this->dataHora <= outro.dataHora;
    }

    // Desempate
    if (this->idPacote != outro.idPacote) {
        return this->idPacote <= outro.idPacote;
    }
    
    
    return false;
}