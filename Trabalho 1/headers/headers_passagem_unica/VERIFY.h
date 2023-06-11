//ARQUIVOS DE VERIFICAÇÃO

// Verifica se existe rótulo na linha atual
int verify_label(string line)
{
    // line: cópia da linha de arquivo atual

    size_t pos_label = line.find(':');                  // Valor da posição ":" Primeiro sinal ":" encontrado na linha
    
    if (pos_label != string::npos){                     // Se foi encontrada ":" Existe rótulo 
        return 1;
    }
    else{                                               // Não foi encontrada ":" Não existe rótulo
        return 0;
    }
}

// Verifica se exite rótulo na Tabela de Símbolos
int verify_label_TS(string label, vector <vector<string>> symbol_table, int& label_number)
{
    // label: rótulo a ser verificado na tabela
    // symbol_table: Tabela de símbolos
    // label_number: número do índice da linha da tabela que possui o rótulo verificado

    if (symbol_table.size() > 0){                           // Se a tabela existe, faz a busca
        for (size_t n = 0; n < symbol_table.size(); n++){   // Busca linha a linha da Tabela de Símbolos
            if (label == (symbol_table[n][0])){             // Se o rótulo é igual a alguma das line_symbol_table[0], Existe em Tabela de Símbolos
                label_number = n;                           // Atribui o número da linha do rótulo encontrado
                return 1;
            }
        }
    }
    return 0;                                               // Rótulo não encontrado ou tabela inexistente
}

// VERIFICA SE INSTRUÇÃO É INVÁLIDA
int verify_instruction(string command)
{
    
    // Procurar comando na tabela de diretivas 
    if (command == "SPACE"){                           
        return 1;
    }
    if (command == "CONST"){                      
        return 1;
    }
    if (command == "BEGIN"){                      
        return 1;
    }
    if (command == "END"){                      
        return 1;
    }    
    if (command == "EXTERN"){                      
        return 1;
    }
    if (command == "PUBLIC"){                      
        return 1;
    }
    
    // // Procurar comando na tabela de instruções
    for (size_t i = 0; i < 14; i++){
        if (command == tables_default(i,0)){
            return 1;
        }
    }
    return 0;
}
