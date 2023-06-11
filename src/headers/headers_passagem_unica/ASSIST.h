// ARQUIVOS ASSISTENTES

// Criar e Definir rótulo na Tabela de Símbolos
void create_define_label_TS(vector <vector<string>> & symbol_table, string label, int location_counter, string token1)
{
    // symbol_table: Tabela de símbolos
    // label: rótulo a ser criado
    // location_counter: posição do endereço de memória

    vector<string> line_symbol_table;                               // Linha da Tabela de Símbolos que vai ser anexada -> Um vetor
    // CASO DO USO DE EXTERN
    if (token1 == "EXTERN"){
        line_symbol_table.push_back(label);                         // Posição 0: "Nome simbolo"                Rótulo
        line_symbol_table.push_back(to_string(0));                  // Posição 1: "Endereço símbolo             Posição do endereço de memória
    }
    else if (label == "EXTERN"){
        line_symbol_table.push_back(token1);                        // Posição 0: "Nome simbolo"                Rótulo
        line_symbol_table.push_back(to_string(0));                  // Posição 1: "Endereço símbolo             Posição do endereço de memória
    }
    //CASO NORMAL
    else{
        line_symbol_table.push_back(label);                         // Posição 0: "Nome simbolo"                Rótulo
        line_symbol_table.push_back(to_string(location_counter));   // Posição 1: "Endereço símbolo             Posição do endereço de memória
    }    

    line_symbol_table.push_back("T");                               // Posição 2: "Definição"                   True
    line_symbol_table.push_back("-1");                              // Posição 3: "Valor lista de endereços"    -1 (elemento inicial)

    symbol_table.push_back(line_symbol_table);                      // Anexar linha
}

// Cria rótulo indefinido na Tabela de Símbolos
void create_undefine_label_TS(vector <vector<string>> & symbol_table, string label, int location_counter)
{
    vector<string> line_symbol_table;                               // Linha da Tabela de Símbolos que vai ser anexada -> Um vetor
    line_symbol_table.push_back(label);                             // Posição 0: "Nome simbolo"                Rótulo 
    line_symbol_table.push_back("X");                               // Posição 1: "Endereço símbolo             Sem endereço de memória ainda
    line_symbol_table.push_back("F");                               // Posição 2: "Definição"                   False
    line_symbol_table.push_back(to_string(location_counter-1));     // Posição 3: "Valor lista de endereços"    Posição do endereço de memória 

    symbol_table.push_back(line_symbol_table);                      // Anexar linha
}

// Define o endereço do rótulo na Tabela de Símbolos
void define_label_TS(vector <vector<string>> & symbol_table, int label_number, int location_counter)
{
    if ((symbol_table[label_number][2] ) == "F"){                               // Se o rótulo não está definido
        (symbol_table[label_number][2] ) = "T";                                 // Defnir estado do rótulo 
        (symbol_table[label_number][1] ) = to_string(location_counter);         // Definir rótulo para esta Posição do endereço de memória
    }    
}

// Redefine o endereço do rótulo na Tabela de Símbolos
void redefine_label_TS(vector <vector<string>> & symbol_table, int label_number, int location_counter)
{
    if ((symbol_table[label_number][2]) == "T"){                                // Se o rótulo já está definido
        cout << "Erro, simbolo repetido" << endl;
        (symbol_table[label_number][1]) = to_string(location_counter);          // Redefinir rótulo para um novo endereço
        cout << "Simbolo foi redefinido" << endl; 
    }    
}

// Atualiza valor lista da label já definida
void update_list_label_TS(vector <vector<string>> & symbol_table, int label_number, int location_counter, int begin, vector <vector<string>> & use_table)
{
    (symbol_table[label_number][3]) = to_string(location_counter-1);                // Redefinir Valor lista de endereços para um novo endereço
}

// Retorna código do comando correspondente e atualiza posição de endereço
string command_code_generation(string command, int& location_counter)
{
    // command: comando da linha do arquivo de entrada, Token 1 
    // const_value: valor da constante reservada pela diretiva CONST, Token 2 pode assume o valor para esse caso
    // Procurar comando na tabela de instruções 

    for (int i = 0; i < 14; i++){                       
        if (command == tables_default(i,0)){                                        //verificando a existencia da instrução
            return tables_default(i,1);
        }  
    }
    return "99";                                                                    //ERRO NA CONVERSAO DE COMANDO PARA CODIGO
}

string command_adress(vector <vector<string>> & symbol_table, string label, int location_counter, int label_number, string token, int begin, vector <vector<string>> & use_table)
{   
    string arg_adress;                                                              // Retorno para endereço de argumento para referência posterior
    label = token;                                                                  // Rótulo tido como argumento
    location_counter = location_counter +1;                                         // Atualização do contador de posição do endereço de memória

    if (verify_label_TS(label, symbol_table, label_number)){                        // Verifica se o argumento está na Tabela de Símbolos
        if ((symbol_table[label_number][2]) == "T"){                                // Se rótulo já definido
            
            if (symbol_table[label_number][1] == "0"){
            vector <string> use_table_line;
            use_table_line.push_back(symbol_table[label_number][0]); 
            use_table_line.push_back(to_string(location_counter-1));
            use_table.push_back(use_table_line); 
            }
            
            arg_adress = (symbol_table[label_number][1]);                                               // Retorna valor do rótulo
        }
        else{
            arg_adress = (symbol_table[label_number][3]);                                               // Busca localização anterior do uso de rótulo
            update_list_label_TS(symbol_table, label_number, location_counter, begin, use_table);       // Atualizar o valor lista da TS do rótulo
        }
        return arg_adress;
    }
    else{                                                                           // Se argumento não está na Tabela de Símbolos, arg_adress == -1
        create_undefine_label_TS(symbol_table, label, location_counter);            // Criar rótulo indefinido na TS
        return "-1"; 
    }   
}
