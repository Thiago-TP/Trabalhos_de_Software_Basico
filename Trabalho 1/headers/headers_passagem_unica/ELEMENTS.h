// ARQUIVO PARA OBTENÇÃO DE ELEMENTOS DE LINHA

// Retorna rótulo da linha / Repassa a linha sem rótulo
string get_label(string line)
{
    // line: linha de arquivo atual passada por referência -> função altera valor no escopo geral (single_pass)

    size_t pos_label = line.find(':');                  // Valor da posição ":" Primeiro sinal ":" encontrado na linha
    string label;                                       // Rótulo a ser obtido e retornado

    label =  line.substr(0, pos_label);                 // Obter rótulo, substring da linha (1º character até antes de":") 
    return label;                                       // Retornar rótulo
}

// Retorno de linha sem sua label
string no_label_line(string line)
{
    // line: linha de arquivo atual passada por referência -> função altera valor no escopo geral (single_pass)

    size_t pos_label = line.find(':');                                      // Valor da posição ":" Primeiro sinal ":" encontrado na linha
    string label;                                                           // Rótulo a ser obtido e retornado
    
    label =  line.substr(0, pos_label);                                     // Obter rótulo, substring da linha (1º character até antes de":")
    
    if (label + ':' != line){
        line = line.substr(pos_label + 2, line.size() - pos_label - 2);     // Linha sem rótulo, substring da linha (próxima palavra até o final)
        return line;                                                        // Retornar rótulo 
    }
    else{
        return "ERRO ROTULO INDEFINIDO";
    }
     
}

//BUSCA TOKENS DENTRO DA LINHA DE COMANDO DE ACORDO COM PARAMETRO INDICADO
string get_token(int num_token, string line, string cmd_token, string label)
{   
    // num_token: indica qual o numero do token a ser retornado
    // line: cópia da linha de arquivo atual 
    // cmd_token: indica qual o token de comando (TOKEN 1) para obter o TOKEN 2 ou 3. Obs: "void" para obter o TOKEN 1

    string token;           // Token de retorno
    size_t pos;             // Valor da posição " " primeiro espaço vazio da linha

    pos = line.find(' ');   // Encontrar valor da posição de " " da linha atual

    // TOKEN 1, COMANDO -> INSTRUÇÃO/DIRETIVA
    if (num_token == 1){

        if (pos != string::npos){               // Se existe um espaço na linha de comando, busca a primeira palavara(SUBSTRING) como Token
            token = line.substr(0, pos);
        }
        else if (line == "STOP"){               // Caso não existe um espaço na linha de comando, se a linha for somente STOP, token é STOP
            token = line;
        }
        else if (line == "SPACE"){              // Caso não existe um espaço na linha de comando, se a linha for somente SPACE, token é SPACE
            token = line;
        }
        else if (label=="EXTERN"){
            token = line;
        }
        
        else if (line == "EXTERN" || line == "PUBLIC" || line == "BEGIN" || line == "END"){
            token = line;
        }
        else{
            token = "ERRO NO TOKEN 1";          // ERRO, Não houve espaço, nem obedeceu as exceções
        }
        return token;                           // Retornar TOKEN 1  
    }

    line = line.substr(pos+1, line.size()-1);   // Linha recortada SEM TOKEN 1
    pos = line.find(' ');

    // TOKEN 2, ARGUMENTO DE INSTRUÇÃO / DIRETIVA SPACE -> VAZIO / DIRETIVA CONSTANT -> NUMERO
    if (num_token == 2){
        if (cmd_token == "STOP"){
            token = "NO ARGUMENT (STOP)";       // Se o comando foi a instrução STOP, ela não tem argumento
        }
        else if(cmd_token == "SPACE"){
            try{
                if(stoi(line)){                 // Se não é número em argumento
                token = line;
                }
                else{
                    throw 1; 
                }    
            }
            catch(...){
                if (line == "SPAC"){
                   token = "VOID TOKEN 2";             // Space sem argumento 
                }
                else{
                    token = line;                      // argumento em palavra
                }    
            }
        }
        // Existe argumento!
        else if (pos != string::npos){          // Se existe espaço entre argumentos, COPY: token é o argumento 1 
            token = line.substr(0, pos);
        }
        else if (cmd_token != "COPY"){          // Existe apenas argumento! Se instrução diferente de COPY, token é o argumento  
            token = line;
        }
        else{
            token = "COPY TOKEN2";              // ERRO, Não houve espaço entre argumentos, nem obedeceu as exceções
        }
        return token;                           // Retornar TOKEN 2 
    }

    // TOKEN 3, ARGUMENTO DE INTRUÇÃO, COPY
    if (num_token == 3){
        if (cmd_token == "COPY"){  

            line = line.substr(pos+3, line.size()-1);   // Linha recortada SEM TOKEN 2 e sem virgula de separação de COPY
            pos = line.find(' ');

            if (pos != string::npos){                   // Se existe espaço após argumento, ERRO ultimo token
                token = "ERRO NO TOKEN 3 TAB VOID";
            }
            else{
                token = line;
            }
        }
        else{
            pos = line.find('+');
            if (pos != string::npos){                   // Se existe espaço após argumento, ERRO ultimo token
                token = "+";
            }
            else{
                token = "NO COPY";                      // ERRO, Token 3 em instrução diferente de COPY
            }
        }
        
        return token;                                   // Retornar TOKEN 3
    }
    return "PEDIDO INVALIDO DE TOKEN";
}