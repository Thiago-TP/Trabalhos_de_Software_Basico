// g++ MONTADOR.cpp -o MONTADOR -Wall
// ./MONTADOR teste1.asm

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

//Arquivos auxiliares
#include "TABELAS.h"
#include "VERIFY.h"
#include "ELEMENTS.h"
#include "ASSIST.h"
#include "OUTPUTSTRING.h"

using namespace std;  // Evita usar "std::"

int single_pass                     (const string&);

int main(int argc, char *argv[]) 
{
    const string& source_file_name = argv[1];                           // ./MONTADOR <arquivo>         |talvez o idx mude no Linux|
    if (single_pass             (source_file_name))                     {return 1;}
    return 0;
}

// ALGORITMO DE PASSAGEM ÚNICA DE CÓDIGO FONTE PARA CÓDIGO OBJETO
int single_pass(const string& filename)
{
    cout << "Inicializado o algoritmo de passagem unica" << endl;

    // Verificação de Erro de Leitura/Escrita em arquivos
    ifstream input_file(filename);                          // Declaração da variável do tipo <arquivo de entrada> atrelada a seu (nome) 
    ofstream output_file("montagem");                       // Declaração da variável do tipo <arquivo de saída> atrelada a seu (nome)
    if (sanity_check(input_file, output_file)) {return 1;}  

    // INÍCIO
    vector <vector<string>> symbol_table;               // Tabela Símbolos -> Vetor dos endereços das array de strings 

    string token1;                                      // Token de Operação
    string token2;                                      // Token de argumento ou Constante
    string token3;                                      // Token de argumento 2 para COPY

    string code;                                        // Código da Operação do comando correspondente
    string label;                                       // Rótulo de uma linha
    string line;                                        // Linha a ser lida do código fonte

    int arg_adress;                                     // Endereço do argumento do comando -> Rótulo de diretiva 
    int arg_adress2;                                    // Endereço do argumento 2 do comando -> Rótulo de diretiva -> COPY

    int label_number;                                   // Número do índice que o vetor guardou a label 
    int line_counter = 1;                               // Contador de linha
    int location_counter = 0;                           // Contador da posição de endereço

    bool link = true;                                   // Indicador de ligação de arquivos objetos

    while (getline(input_file, line)) {                                             // Leitura linha a linha arquivo de entrada
        if (verify_label(line)){                                                    // Verifica se a linha contém um rótulo
            label = get_label(line);                                                // Obter rótulo / Repassa a linha sem rótulo
            if (verify_label_TS(label, symbol_table, label_number)){                // Verifica se o rótulo está na Tabela de Símbolos
                redefine_label_TS(symbol_table, label_number, location_counter);    // Redefinir rótulo, caso já esteja definido na TS
                define_label_TS(symbol_table, label_number, location_counter);      // Definir rótulo, usado antes em argumento          
            }   
            else{                                                                   // Se rótulo não está na Tabela de Símbolos - Criar e Definir            
                create_define_label_TS(symbol_table, label, location_counter);
            }
        }
        
        // OBTENÇÃO DE TOKENS DADA A LINHA ATUAL
        token1 = get_token(1,line,"void");                                          // CÓDIGO DA OPERAÇÃO DO COMANDO                                          
        token2 = get_token(2,line,token1);                                          // ARGUMENTO 1 DO COMANDO / VALOR DA DIRETIVA
        token3 = get_token(3,line,token1);                                          // ARGUMENTO 2 DO COMANDO

        // PROCURAR OPERAÇÃO NA TABELA DE INSTRUÇÕES
        code = command_code(token1, token2, location_counter);                      

        // PROCURAR ENDEREÇO DO ARGUMENTO PARA REFERÊNCIA POSTERIOR
        if (token1 == "COPY"){                                                      // COPY possui 2 argumentos
            arg_adress = command_adress(symbol_table, label, location_counter, label_number, token2);
            arg_adress2 = command_adress(symbol_table, label, location_counter, label_number, token3);
        }
        else{                                                                       // Outras possuem 1 argumento
            arg_adress = command_adress(symbol_table, label, location_counter, label_number, token2);
        }

        // CÓDIDO DE SAÍDA PARA REFERÊNCIA POSTERIOR
        create_object_code(token1, code, arg_adress, arg_adress2, output_file, link);

        line_counter = line_counter + 1;                                            // Avança uma linha na contagem

    } 

    // FINAL DA LEITURA DO ARQUIVO
    input_file.close();
    output_file.close();
    cout << "Montagem realizada com sucesso.\n\n" << endl;
    return 0; 
}



/* ALGORITMO DE PRE PROCESSAMENTO
Separa os comentários da linhas
Ignora os comentários
*/

/* ALGOTIMO DE PASSAGEM ÚNICA

contador_posição = 0
contador_linha = 1

Enquanto arquivo fonte não chegou ao fim, faça:

Obtém uma linha do fonte

Verifica se tem rótulo
Se houver: 
            obter rótulo 
            procurar na tabela de símbolos
            Se está na tabela de símbolos:
                Se o rótulo não está definido
                    Definir e Realizar Referência posterior
                Senão:
                    Erro -> redefinir símbolo
            Senão:
                    Inserir rótulo e contador de posições
            repassar a linha sem rótulo

Separa os elementos da linha: operação, operandos

Procurar operação na tabela de instruções
Se achou:
            contador_posição = contador_posição + tamanho da instrução
Senão:
        procurar na tabela de diretivas
        Se achou:
                    subrotina executa diretiva
                    contador_posição = valor retornado pela subrotina
        Senão:
                    Erro operação não identificada
Procurar endereço para referência posterior:
    Se Existe rótulo ainda indefinido:
        atualizar valor lista
    Senão:
        criar um rótulo indefinido
contador_linha = contador_linha + 1
*/


    //Exemplo
    //              TABELAS DE SIMBOLOS (TS)
    //  SIMBOLO     VALOR       DEF         LISTA
    //  ---------------------------------------------
    //  Array[0]     Array[1]     Array[2]     Array[3]
    //  Array2[0]    Array2[1]    Array2[2]    Array2[3]
    //  ...          ...          ...          ...

    // Array[0] = "Nome simbolo"
    // Array[1] = "Valor endereço"
    // Array[2] = "Def T ou F"
    // Array[3] = "Numero da lista"
