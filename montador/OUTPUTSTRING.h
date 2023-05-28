#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

// AQUIVO CRIADOR DA SAÍDA

// Cria linha atual do arquivo de saída
void create_object_code(string token1, string code, int arg_adress, int arg_adress2, ofstream& outputFile, bool link)
{
    if (link==false){
            if (token1 == "COPY"){                              // Se a operação foi COPY 
            outputFile << code << "\t";                         // Escreve o código e realiza espaçamento
            outputFile << arg_adress << "\t";                   // Escreve o endereço e realiza espaçamento
            outputFile << arg_adress2 << "\t";                  // Escreve o endereço e quebra de linha
            }
            else if (token1 == "SPACE" || token1 == "CONST"){   // Se a operação foi SPACE ou CONST
                outputFile << code << "\t";                     // Escreve o código e quebra de linha
            }
            else if (token1 == "STOP"){                         // Se a operação foi STOP
                outputFile << code << "\t";                     // Escreve o código e quebra de linha
            }
            else{                                               // Em outras operações
                outputFile << code << "\t";                     // Escreve o código e realiza espaçamento
                outputFile << arg_adress << "\t";               // Escreve o endereço e quebra de linha
            }        
    }
    else{
            if (token1 == "COPY"){                              // Se a operação foi COPY 
            outputFile << code << "\t";                         // Escreve o código e realiza espaçamento
            outputFile << arg_adress << "\t";                   // Escreve o endereço e realiza espaçamento
            outputFile << arg_adress2 << "\n";                  // Escreve o endereço e quebra de linha
            }
            else if (token1 == "SPACE" || token1 == "CONST"){   // Se a operação foi SPACE ou CONST
                outputFile << code << "\n";                     // Escreve o código e quebra de linha
            }
            else if (token1 == "STOP"){                         // Se a operação foi STOP
                outputFile << code << "\n";                     // Escreve o código e quebra de linha
            }
            else{                                               // Em outras operações
                outputFile << code << "\t";                     // Escreve o código e realiza espaçamento
                outputFile << arg_adress << "\n";               // Escreve o endereço e quebra de linha
            }
    }
}

// Possível erro de simulação: tab após última escrita