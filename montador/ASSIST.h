#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

// ARQUIVOS ASSISTENTES

// Criar e Definir rótulo na Tabela de Símbolos
void create_define_label_TS(vector <vector<string>> & symbol_table, string label, int location_counter)
{
    // symbol_table: Tabela de símbolos
    // label: rótulo a ser criado
    // location_counter: posição do endereço de memória

    vector<string> line_symbol_table;                           // Linha da Tabela de Símbolos que vai ser anexada -> Um vetor
    line_symbol_table.push_back(label);                         // Posição 0: "Nome simbolo"                Rótulo 
    line_symbol_table.push_back(to_string(location_counter));   // Posição 1: "Endereço símbolo             Posição do endereço de memória
    line_symbol_table.push_back("T");                           // Posição 2: "Definição"                   True
    line_symbol_table.push_back("-1");                          // Posição 3: "Valor lista de endereços"    -1 (elemento inicial)

    symbol_table.push_back(line_symbol_table);                  // Anexar linha
}

// Cria rótulo indefinido na Tabela de Símbolos
void create_undefine_label_TS(vector <vector<string>> & symbol_table, string label, int location_counter)
{
    vector<string> line_symbol_table;                           // Linha da Tabela de Símbolos que vai ser anexada -> Um vetor
    line_symbol_table.push_back(label);                         // Posição 0: "Nome simbolo"                Rótulo 
    line_symbol_table.push_back("X");                           // Posição 1: "Endereço símbolo             Sem endereço de memória ainda
    line_symbol_table.push_back("F");                           // Posição 2: "Definição"                   False
    line_symbol_table.push_back(to_string(location_counter-1)); // Posição 3: "Valor lista de endereços"    Posição do endereço de memória 

    symbol_table.push_back(line_symbol_table);                  // Anexar linha
}

// Define o endereço do rótulo na Tabela de Símbolos
void define_label_TS(vector <vector<string>> & symbol_table, int label_number, int location_counter)
{
    if ((symbol_table[label_number][2] ) == "F"){                              // Se o rótulo não está definido
        (symbol_table[label_number][2] ) = "T";                                // Defnir estado do rótulo 
        (symbol_table[label_number][1] ) = to_string(location_counter + 1);    // Definir rótulo para esta Posição do endereço de memória
    }    
}

// Redefine o endereço do rótulo na Tabela de Símbolos
void redefine_label_TS(vector <vector<string>> & symbol_table, int label_number, int location_counter)
{
    if ((symbol_table[label_number][2]) == "T"){                               // Se o rótulo já está definido
        cout << "Erro, símbolo repetido" << endl;
        (symbol_table[label_number][1]) = to_string(location_counter+1);       // Redefinir rótulo para um novo endereço
        cout << "Símbolo foi redefinido" << endl; 
    }    
}

// Atualiza valor lista da label já definida
void update_list_label_TS(vector <vector<string>> & symbol_table, int label_number, int location_counter)
{
    (symbol_table[label_number][3]) = to_string(location_counter-1);           // Redefinir Valor lista de endereços para um novo endereço
}



// Retorna código do comando correspondente e atualiza posição de endereço
string command_code(string command, string const_value, int& location_counter)
{
    // command: comando da linha do arquivo de entrada, Token 1 
    // const_value: valor da constante reservada pela diretiva CONST, Token 2 pode assume o valor para esse caso
    // location_counter: posição do endereço de memória

    // Procurar comando na tabela de instruções 
    for (int i = 0; i < 15; i++){                       
        if (command == tables_default(1,i,0)){
            location_counter = location_counter + 1;    //contador_posição = contador_posição + tamanho da instrução
            return tables_default(1,i,1);
        }  
    }

    // Procurar comando na tabela de diretivas 
    if (command == "SPACE"){                            // Se é a diretiva SPACE retornar 00
        location_counter = location_counter + 1;
        return "00";
    }
    else if (command == "CONST"){                       // Se é a diretiva CONST retornar o valor da constante 
        location_counter = location_counter + 1;
        return const_value;
    }
    else{
        return "ERRO NA CONVERSAO DE COMANDO PARA CODIGO";
    }
}

int command_adress(vector <vector<string>> & symbol_table, string label, int& location_counter, int label_number, string token)
{   
    int arg_adress;                                                                 // Retorno para endereço de argumento para referência posterior
    label = token;                                                                  // Rótulo tido como argumento
    location_counter = location_counter +1;                                         // Atualização do contador de posição do endereço de memória

    if (verify_label_TS(label, symbol_table, label_number)){                        // Verifica se o argumento está na Tabela de Símbolos
        if ((symbol_table[label_number][2]) == "T"){                                // Se rótulo já definido
            arg_adress = stoi((symbol_table[label_number][1]));                     // Retorna valor do rótulo
        }
        else{
            arg_adress = stoi((symbol_table[label_number][3]));                     // Busca localização anterior do uso de rótulo
            update_list_label_TS(symbol_table, label_number, location_counter);     // Atualizar o valor lista da TS do rótulo
        }
        return arg_adress;
    }
    else{                                                                           // Se argumento não está na Tabela de Símbolos, arg_adress == -1
        create_undefine_label_TS(symbol_table, label, location_counter);            // Criar rótulo indefinido na TS
        return -1; 
    }   
}
