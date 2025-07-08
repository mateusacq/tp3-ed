//---------------------------------------------------------------------
// Arquivo      : main.cpp
// Conteudo     : processamento da entrada e realização de consultas nos eventos
// Autor        : Mateus Antinossi Cordeiro Queiroz (mateusacq@ufmg.br)
// Historico    : 2025-07-01 - arquivo criado
//---------------------------------------------------------------------

#include "heap.hpp"
#include "arvore.hpp"
#include "evento.hpp"
#include "lista_eventos.hpp"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>

// Funcao auxiliar para ler arquivo de entrada
TipoEvento stringParaEvento(std::string s) {
    if (s == "RG") {
        return RG;
    } else if (s == "AR") {
        return AR;
    } else if (s == "RM") {
        return RM;
    } else if (s == "UR") {
        return UR;
    } else if (s == "TR") {
        return TR;
    } else if (s == "EN") {
        return EN;
    } else {
        std::cerr << "ERRO: tipo de evento"
                  << s << "invalido.\n";
        return INVALIDO;
    }
}

int main(int argc, char* argv[]) {
    // Le arquivo de entrada
    if (argc != 2) { // Arquivo nao passado na linha de comando
        std::cerr << "Uso: " << argv[0] << " <nome_do_arquivo>\n";
        return 1;    
    }
    std::ifstream in(argv[1]);
    if (!in) {
        std::cerr << "ERRO: falha ao abrir o arquivo." << std::endl;
        return 1;
    }
    
    /* Arvores usadas nas consultas:
    Arvore com eventos por pacote: */
    ArvoreAVL<int, ListaEventos> IndicePacotes; // Principal, gerencia eventos
    // Arvore com pacotes por cliente (pacotes ordenados pelo tempo em que chegaram)
    ArvoreAVL< std::string, ArvoreAVL<int, int> > IndiceClientes;

    std::string linha;
    while (std::getline(in, linha)) {
        // Le elementos da linha
        std::istringstream iss(linha);
        int dataHora;
        std::string comando;

        iss >> dataHora >> comando;

        if (comando == "EV") { // EV: Adiciona evento
            std::string tipoTemp;
            TipoEvento tipo;
            iss >> tipoTemp;
            tipo = stringParaEvento(tipoTemp);
            
            std::string remetente, destinatario;
            int idPacote, origem, destino, secao;

            switch (tipo) {
                case (0): { // RG
                    iss >> idPacote
                        >> remetente >> destinatario
                        >> origem >> destino;
                    
                    Evento * e = new Evento(
                        dataHora, tipo, idPacote, remetente, destinatario,
                        origem, destino, -1);
                    
                    // Registra novo evento
                    ListaEventos * l = IndicePacotes.Pesquisa(idPacote);
                    if (!l) {
                        ListaEventos nova;
                        nova.InsereEvento(e);
                        IndicePacotes.Insere(idPacote, nova);
                    } else {
                        std::cerr << "ERRO: pacote " << idPacote << " ja registrado\n";
                    }

                    // Registra novo pacote para o cliente (remetente ou destinatario)
                    // Remetente:
                    ArvoreAVL<int, int> * r = IndiceClientes.Pesquisa(remetente);

                    if (!r) { // Remetente do evento nao registrado
                        
                        ArvoreAVL<int, int> novaArvore;
                        
                        novaArvore.Insere(dataHora, idPacote);
                        IndiceClientes.Insere(remetente, novaArvore);

                    } else { // Remetente registrado
                        int * teste = r->Pesquisa(idPacote);

                        if (!teste) { // Remetente nao tem esse pacote registrado ainda
                            // Acrescenta novo pacote à posse do cliente
                            r->Insere(dataHora, idPacote);

                        } else { 
                            std::cerr << "ERRO: pacote " << idPacote 
                                      << idPacote << " ja registrado no nome de "
                                      << remetente << std::endl;
                        }
                    }

                    // Destinatario:
                    ArvoreAVL<int, int> * d = IndiceClientes.Pesquisa(destinatario);

                    if (!d) { // Destinatario do evento nao registrado
                        
                        ArvoreAVL<int, int> novaArvore;
                        
                        novaArvore.Insere(dataHora, idPacote);
                        IndiceClientes.Insere(destinatario, novaArvore);
                    } else { // Destinatario registrado
                        int * teste = d->Pesquisa(idPacote);

                        if (!teste) { // Destinatario nao tem esse pacote registrado ainda
                            // Acrescenta novo pacote à posse do cliente
                            d->Insere(dataHora, idPacote);
                        } else { 
                            std::cerr << "ERRO: pacote " << idPacote 
                                      << idPacote << " ja registrado no nome de "
                                      << destinatario << std::endl;
                        }
                    }

                    break;
                }
                case (1): // AR
                case (2): // RM
                case (3): { // UR
                    iss >> idPacote
                        >> destino
                        >> secao;

                    Evento * e = new Evento(
                        dataHora, tipo, idPacote, "", "",
                        -1, destino, secao);
                    
                    // Acrescenta novo evento
                    ListaEventos * l = IndicePacotes.Pesquisa(idPacote);
                    if (l) {
                        // Modifica item da arvore diretamente
                        l->InsereEvento(e);
                    } else {
                        std::cerr << "ERRO: pacote " << idPacote << " nao encontrado na lista de eventos\n";
                    }

                    break;
                }
                case (4): { // TR
                    iss >> idPacote
                        >> origem
                        >> destino;

                    Evento * e = new  Evento(
                        dataHora, tipo, idPacote, "", "",
                        origem, destino, -1);
                    
                    // Acrescenta novo evento
                    ListaEventos * l = IndicePacotes.Pesquisa(idPacote);
                    if (l) {
                        l->InsereEvento(e);
                    } else {
                        std::cerr << "ERRO: pacote " << idPacote << " nao encontrado na lista de eventos\n";
                    }

                    break;
                }
                case (5): { // EN
                    iss >> idPacote
                        >> destino;

                    Evento * e = new Evento(
                        dataHora, tipo, idPacote, "", "",
                        -1, destino, -1);

                    // Acrescenta novo evento
                    ListaEventos * l = IndicePacotes.Pesquisa(idPacote);
                    if (l) {
                        l->InsereEvento(e);
                    } else {
                        std::cerr << "ERRO: pacote " << idPacote 
                        << " nao encontrado na lista de eventos\n";
                    }

                    break;
                }
                case INVALIDO:
                default:
                    break;
            }
        } else if (comando == "CL") { // CL: Consulta pacotes do cliente
            std::string cliente;
            iss >> cliente;

            std::cout << std::setfill('0') << std::setw(6) << dataHora
                          << " CL " << cliente << std::endl;

            ArvoreAVL<int, int> * pacotes = IndiceClientes.Pesquisa(cliente);
            if (pacotes) {
                // Obtem eventos final e inicial
                ArvoreAVL<int, int> copia = *pacotes;
                int n_pacotes = copia.GetTamanho();
                
                // 2 eventos para cada pacote serao exibidos (inicial e final)
                std::cout << n_pacotes * 2 << std::endl;
                
                Heap Escalonador = Heap(n_pacotes * 2);

                // Escalona eventos final e inicial
                while (!copia.Vazia()) {
                    int proxId = copia.GetProxima();
                    ListaEventos * l = IndicePacotes.Pesquisa(proxId);
                    if (l) {
                        CelulaE * primeira_celula = l->GetPrimeira();
                        if (primeira_celula) {
                            Evento * inicio = primeira_celula->GetItem().GetEvento();
                            if (inicio) {
                                Escalonador.InsereEvento(*inicio);
                            } else {
                                std::cerr << "ERRO: evento inicial nulo para pacote " 
                                << proxId << std::endl;
                            }
                        } else {
                            std::cerr << "ERRO: primeira celula nula para pacote "
                             << proxId << std::endl;
                        }
                        CelulaE * ultima_celula = l->GetUltima();
                        if (ultima_celula) {
                            Evento * fim = ultima_celula->GetItem().GetEvento();
                            if (fim) {
                                Escalonador.InsereEvento(*fim);
                            }
                        } // Nao precisa de erros de ponteiros nulos, pois
                        // pacote pode apenas estar registrado e inalterado
                    } else {
                        std::cerr << "ERRO: pacote " << proxId 
                        << " nao encontrado nos registros\n";
                    }
                }

                // Imprime eventos obtidos
                while (!Escalonador.Vazio()) {
                    Escalonador.RetiraProximoEvento().imprimeEvento();
                }
            } else {
                std::cout << 0 << std::endl;
            }
        } else if (comando == "PC") { // PC: Consulta eventos do pacote
            int idPacoteConsultado;
            iss >> idPacoteConsultado;

            std::cout << std::setfill('0') << std::setw(6) << dataHora
                      << " PC " 
                      << std::setfill('0') << std::setw(3) << idPacoteConsultado
                      << std::endl;

            ListaEventos * l = IndicePacotes.Pesquisa(idPacoteConsultado);

            if (l) {
                std::cout << l->GetTamanho() << std::endl;
                l->Imprime();
            } else {
                std::cout << 0 << std::endl;
            }
        } else {
            std::cerr << "ERRO: comando " << comando << " invalido.\n"
                      << "Comandos possiveis: EV, CL, PC\n";
        }
    }

    return 0;
}