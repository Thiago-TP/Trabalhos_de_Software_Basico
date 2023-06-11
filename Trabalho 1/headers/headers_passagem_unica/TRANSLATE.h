//ARQUIVOS DE ETAPAS DE TRADUÇÃO
#include "TABELAS.h"
#include "VERIFY.h"
#include "ELEMENTS.h"
#include "ASSIST.h"
#include "OUTPUTSTRING.h"

// Analisador Lexico
int lexicon_error(int line_counter, string element, string name_element, int mode, string token1)
{
    // mode: modo de avaliação de casos
    // 0 ou 1: modo de avaliação do nome da label e do token1
    // 2: modo de avaliação do argumento de costante  
    int error=0;

    // Se o elemento não se inicia de acordo com as regras léxicas gramaticais: error
    if ( !(element[0] == '_' || (element[0] >= 'A' && element[0] <= 'Z') || (element[0] >= 'a' && element[0] <= 'z'))){
        if (mode == 0 || mode == 1){
            cout <<"Erro lexico na linha "<< line_counter << ": "<< name_element <<" inicializado com caractere invalido" << endl;
            error = 1; 
        }
        if (mode == 2){
            if(!(token1 == "CONST" || token1 == "SPACE")){
                cout <<"Erro lexico na linha "<< line_counter << ": "<< name_element <<" inicializado com caractere invalido" << endl;
                error = 1; 
            }
            else{
                // Se não é número, deve passar pelas demais avaliações
                try{
                    if(stoi(element)){                                           
                        error = 0;
                    }
                    else{
                        throw 1; 
                    }
                }
                catch(...){
                    if ( !(element[0] == '_' || (element[0] >= 'A' && element[0] <= 'Z') || (element[0] >= 'a' && element[0] <= 'z')) ){
                    // Se o argumento não é número e não se inicia de acordo com as regras léxicas
                    cout <<"Erro lexico na linha "<< line_counter << ": "<< name_element <<" inicializado com caractere invalido" << endl;
                    error = 1;
                    }
                    else{
                        error = 0;
                    }
                    if(token1 == "CONST"){
                        cout<<"Linha "<< line_counter <<": A constante informada nao foi um numero inteiro"<<endl;
                    }
                    if (token1 == "SPACE" && element != "VOID TOKEN 2"){
                        cout<<"Linha "<< line_counter <<": O argumento informado nao foi um numero inteiro"<<endl;
                    }
                }
            }
        }
    }
    return error;
}

// Gerenciador da tabela de uso
int def_table_manager(string label, vector <vector<string>>& def_table, string token1, string token2)
{
    string public_label;

    // Adquire o rotulo da diretiva PUBLIC caso exista
    if (label == "no label" && token1 != "PUBLIC"){                                               
        return 0;
    }
    if (token1 == "PUBLIC"){
        public_label = token2;
    }
    else if (label == "PUBLIC"){
        public_label = token1;
    }

    // Acrescenta simbolo na tabela de uso
    vector<string> line_def_table;                           
    line_def_table.push_back(public_label);                         
    def_table.push_back(line_def_table);                  

    return 0;
}

// Atualiza a tabela de definição de acordo com a tabela de simbolos
int def_table_TS_table(vector <vector<string>>& def_table, vector <vector<string>>& symbol_table)
{
    vector <vector<string>> update_def_table;
    vector<string> line_update_def_table;
    int k = 0;

    // Verifica simbolos em comum
    for (size_t i = 0; i < def_table.size(); i++){
        for (size_t j = 0; j < symbol_table.size(); j++){
            if (def_table[i][0] == symbol_table[j][0]){
                line_update_def_table.push_back(symbol_table[j][0]);
                line_update_def_table.push_back(symbol_table[j][1]);
                update_def_table.push_back(line_update_def_table);
            } 
        }
    }
    
    // Esvazia tabela de uso desatualizada
    while (def_table.size()>0){
        def_table.pop_back();     
    }

    // Preenche com as informações atualizadas
    while (def_table.size()!=update_def_table.size()){
        def_table.push_back(update_def_table[k]);
        k++;
    }        
    return 0;
}

// Obtenção dos elememtos da linha de comando
int scanner(vector <vector<string>> & def_table ,int line_counter, string line, string& label, string& token1, string& token2, string& token3, int& section_text_end, int section_data_begin)
{
    if (verify_label(line)){                                                    // Verifica se a linha contém um rótulo
        label = get_label(line);                                                // Obter rótulo 
        line = no_label_line(line);                                             // Repassa a linha sem rótulo
    }
    else{
        label = "no label";
    }

    if (label == "PUBLIC"){                                                     // Se o rótulo foi PUBLIC vai ao gerenciador de tabela de definições
        def_table_manager(label, def_table, line, token2);
        return 0;
    }
    
    // Obter tokens
    token1 = get_token(1,line,"void",label);                                    // CÓDIGO DA OPERAÇÃO DO COMANDO / INSTRUÇÃO                                      
    token2 = get_token(2,line,token1,label);                                    // ARGUMENTO 1 DA OPERAÇÃO / TAMANHO DO SPACE 
    token3 = get_token(3,line,token1,label);                                    // ARGUMENTO 2 DA OPEAÇÃO COPY

    if (label=="EXTERN"){                                                       // Se EXTERN foi uma label, trocar elementos entre rotulo e instrução
        string aux = token1;                                                    // Procedimento para formatação das demais instruções
        token1 = label;
        label = aux;
    }

    if (token1 == "PUBLIC"){                                                    // Se o token1 foi PUBLIC vai ao gerenciador de tabela de definições
        def_table_manager(label, def_table, token1, token2);
        return 0;
    } 

    // Caso for uma das diretivas seguintes não serão analisados os erros léxicos
    if (token1 == "EXTERN" || token1 == "PUBLIC" || token1 == "BEGIN" || token1 == "END"){
            return 0;
        }

    // Analise de erros lexicos
    if (label != "no label"){
        if(lexicon_error(line_counter, label, "rotulo",0, token1)){             // Houve erro léxico no rotulo
            label = "ERROR_LABEL" + to_string(line_counter) + ":";
        }        
    }
    
    if (lexicon_error(line_counter, token1,"comando",1, token1)){               // Houve erro léxico no token 1
        token1 = "ERROR_TOKEN1_" + to_string(line_counter);
        }            

    if (lexicon_error(line_counter, token2,"argumento 1",2, token1)){           // Houve erro léxico no token 2
        token2 = "ERROR_TOKEN2_" + to_string(line_counter);
        }        
    
    if (token1 == "COPY"){
        if (lexicon_error(line_counter, token3,"argumento 2",0, token1)){       // Houve erro léxico no token 3
            token2 = "ERROR_TOKEN3_" + to_string(line_counter);
            }    
    }

    // Verificação de fim da SECTION TEXT
    if(token1 == "STOP" || section_data_begin == 1){                            // Indica fim da seção TEXT
            section_text_end = 1;
        }
    return 0;                                                                   // Não houve erro léxico
}

// Analisador sintático
int parser(int line_counter, string line, string token1, string token2, string label)
{
    int error=0;
    // Erro de Rótulo
    size_t pos_label = line.find(':');                  // Valor da posição ":" Primeiro sinal ":" encontrado na linha
    
    if (line[pos_label + 1] == ':'){
        cout <<"Erro sintatico na linha "<< line_counter << " : houve dois caracteres ':' seguidos" << endl;
        error=1;
    }

    
    // Instrução inexistente
    if (!verify_instruction(token1) && label != "EXTERN" && label != "PUBLIC"){
        // se instrução foi invalida
        cout <<"Erro sintatico na linha "<< line_counter << ": instrucao ou diretiva invalida" << endl;
        error = 1;
    }

    // Erros de COPY
    
    if (token1 =="COPY")
    {
        if (label != "no label"){
            line = no_label_line(line);                     // Repassa a linha sem rótulo
        }
        size_t pos = line.find(token2);                     // Encontrar valor da posição de token2 da linha atual

        if (line[pos + token2.size()]== ' ' && line[pos + token2.size()+2] == ' '){
            // se estiver corretamente
            return error;
        } 
        else if (token2 =="COPY TOKEN2"){
            // Escrita errada
            cout <<"Erro sintatico na linha "<< line_counter << " : nao houve espaco entre virgulas ou foi fornecido somente um argumento em COPY" << endl;
            error=1;
        }     
        else if (line[pos + token2.size()-1]== ','){
            // Se não houver espaço antes da vírgula
            cout <<"Erro sintatico na linha "<< line_counter << " : nao houve espaco antes da virgula na instrucao COPY" << endl;
            error=1;
        }
        else if (line[pos + token2.size()+1] != ' '){
            // Se não houver espaço depois da vírgula
            cout <<"Erro sintatico na linha "<< line_counter << " : nao houve espaco depois da virgula na instrucao COPY" << endl;
            error=1;
        }
    }
    return error;
}

// Gerenciador da tabela de simbolos
int symbol_table_manager(string label, int location_counter, int& label_number, vector <vector<string>> & symbol_table, string token1, vector <vector<string>>& use_table)
{
    // Se não há rotulo a ser avaliado retorna
    if (label == "no label"){                                               
        return 0;
    }
       
    else if (verify_label_TS(label, symbol_table, label_number)){           // Verifica se o rótulo está na Tabela de Símbolos, atualiza label number
        redefine_label_TS(symbol_table, label_number, location_counter);    // Redefinir rótulo, caso já esteja definido na TS
        define_label_TS(symbol_table, label_number, location_counter);      // Definir rótulo, usado antes em argumento              
    }   
     else{                                                                  // Se rótulo não está na Tabela de Símbolos - Criar e Definir            
        create_define_label_TS(symbol_table, label, location_counter, token1);
    }
    return 0;
}

// Gerador do tradução sem referência posterior
int no_reference_code_generation(string no_reference_code[], string line, string command_code, string arg_adress, string arg_adress2, string label, 
string token1, string token2, string token3, int label_number, int& location_counter, vector <vector<string>> & symbol_table, int& begin, vector <vector<string>> & use_table)

{       
        // location_counter: posição do endereço de memória
        
        // Evitar casos especiais na geração de código
        size_t pos;                                     // Valor da posição "1" da linha
        pos = token1.find('1');                         // Encontrar valor da posição de "1" da linha atual

        if (token1.substr(0, pos+1) == "ERROR_TOKEN1"){ // verifica se token1 teve erro
            return 1;
        }

        if (token1 == "BEGIN"){
            begin = 1;
            return 1;
        }
        if (token1 == "PUBLIC" || label == "PUBLIC" || token1 == "EXTERN" || token1 == "END"){
            return 1;
        }
        
        // PREMEIRA COLUNA DO CÓDIGO SEM REFERÊNCIA POSTERIOR
        if (token1 =="SPACE"){
            if (token2 == "VOID TOKEN 2"){
                no_reference_code[location_counter] = "00";                    // reserva número de espaços dado pelo valor do argumento de SPACE
                location_counter = location_counter + 1;                       // contador_posição avança
            }
            else{
                try{
                    if(stoi(token2)){
                        for (int i = 0; i < stoi(token2); i++){
                            no_reference_code[location_counter] = "00";        // reserva número de espaços dado pelo valor do argumento de SPACE
                            location_counter = location_counter + 1;           // contador_posição avança
                        }
                    }
                    else{
                        throw 1;
                    }
                }
                catch(...){
                    cout <<"Erro semantico no endereco "<< location_counter << ": "<< "Argumento de SPACE nao foi um numero"<< endl;
                }
            }    
        }
        else if (token1 =="CONST"){
            no_reference_code[location_counter] = token2;                      // recebe o valor da constante
            location_counter = location_counter + 1;                           // contador_posição avança
        }
        else{
            command_code = command_code_generation(token1, location_counter);  // Busca comando na tabela de instruções      
            no_reference_code[location_counter] = command_code;                // recebe código de comando
            location_counter = location_counter + 1;                           // contador_posição avança
        }
        

        //SEGUNDA/TERCEIRA COLUNA DO CÓDIGO SEM REFERÊNCIA POSTERIOR

        // PROCURAR ENDEREÇO DO ARGUMENTO PARA REFERÊNCIA POSTERIOR
        if (token1 == "STOP" || token1 == "SPACE" || token1 =="CONST"){}            // Opereções sem segunda coluna, coluna não vai para no_reference_code[]

        else if (token1 == "COPY"){                                                 // COPY possui 2 argumentos
            arg_adress = command_adress(symbol_table, label, location_counter, label_number, token2, begin, use_table);
            no_reference_code[location_counter] = arg_adress;                       // Segunda coluna Código sem referencia recebe argumento de comando
            location_counter = location_counter + 1;                                // contador_posição avança  

            arg_adress2 = command_adress(symbol_table, label, location_counter, label_number, token3, begin, use_table);
            no_reference_code[location_counter] = arg_adress2;                      // Terceira coluna Código sem referencia recebe argumento de comando
            location_counter = location_counter + 1;                                // contador_posição avança
        }
        else{                                                                       // Outras Instruções possuem 1 argumento
            arg_adress = command_adress(symbol_table, label, location_counter, label_number, token2, begin, use_table);
            
            // TRATAMENTO CASO ESPECIAL DE ACESSO A PARTIR DE UMA LABEL -> LABEL + NUM 
            if (token3 == "+"){
                pos = line.find('+');
                try{
                    if (stoi(line.substr(pos+2))){
                        arg_adress = arg_adress + " +" + line.substr(pos+2);
                    }
                    else{
                        throw 1;
                    }
                }
                catch(...){
                    cout<<"Formato indevido de rotulo no endereco: "<< location_counter <<endl;
                    arg_adress = arg_adress + " +0";    
                }     
            }

            no_reference_code[location_counter] = arg_adress;                       // Segunda coluna Código sem referencia recebe argumento de comando
            location_counter = location_counter + 1;                                // contador_posição avança
        }
        return 0;
}

// Conserta o problema da falta de referência posterior
int later_reference_fixer(string no_reference_code[], int& label_number, vector <vector<string>> & symbol_table, string label, string token3){
    string aux;
    
    if (verify_label_TS(label, symbol_table, label_number)){
        while (symbol_table[label_number][3] != "-1")
        {   
            size_t pos = no_reference_code[stoi(symbol_table[label_number][3])].find('+');
            // CASO ESPECIAL DE ACESSO DE ENDEREÇO POR LABEL -> LABEL = NUM
            if (pos != string::npos){
                int sum = stoi(no_reference_code[stoi(symbol_table[label_number][3])].substr(pos+1));
                no_reference_code[stoi(symbol_table[label_number][3])] = no_reference_code[stoi(symbol_table[label_number][3])].substr(0, pos-1);

                aux = no_reference_code[stoi(symbol_table[label_number][3])];
                no_reference_code[stoi(symbol_table[label_number][3])] = to_string (stoi(symbol_table[label_number][1]) + sum);  
                symbol_table[label_number][3] = aux;      
            }
            // CASO NORMAL  
            else{
                aux = no_reference_code[stoi(symbol_table[label_number][3])];                               //aux = no_reference_code[ultimo_end]
                no_reference_code[stoi(symbol_table[label_number][3])] = symbol_table[label_number][1];     //no_reference_code[ultimo_end]= endereço real 
                symbol_table[label_number][3] = aux;                                                        //prox_end = no_reference_code[ultimo_end]
            }   
        }
        return 0;
    }
    return 1;
}

// Analisador de erro semantico
// possui diferentes modos de uso
// Analisador de erros Semanticos: existencia da SECTION TEXT modo 1
// Analisador de erros Semanticos: Rotulo ou dado não definido nas SECTIONS TEXT e DATA modo 2
// Analisador de erros Semanticos: relacionados a ligação BEGIN END EXTERN PUBLIC modo 3
int semantic_error(int begin, int end, int public_id, int extern_id, string token1, string label, int line_counter, int section_text_begin, int section_text_end, bool& no_section_text, int mode, string no_reference_code[], int location_counter)
{
    int error = 0;

    if(mode==1 && !no_section_text){
        if (section_text_begin == 0){
            cout <<"Erro semantico: Nao existe SECTION TEXT"<< endl;
            error = 1;
            no_section_text = true;
        }
    }    

    if(mode==2){
        size_t pos;                                     // Valor da posição "1" da linha
        pos = token1.find('1');                         // Encontrar valor da posição de "1" da linha atual

        if (token1.substr(0, pos+1) == "ERROR_TOKEN1"){
            token1 = "ERRO NO TOKEN 1";        
        }    

        if (label!="no label" && token1 == "ERRO NO TOKEN 1" && label != "EXTERN" && label != "PUBLIC"){
            if(section_text_end == 0){
                cout <<"Erro semantico na linha "<< line_counter << ": "<< "Rotulo nao definido SECTION TEXT"<< endl;
            }
            if(section_text_end == 1){
                cout <<"Erro semantico na linha "<< line_counter << ": "<< "Dado nao definido SECTION DATA"<< endl;
            }
            error = 1;
        }
    }

    if (mode==3){
        int i = 0;
        while (i<=location_counter){
            if(no_reference_code[i]=="-1" && begin == 0){
                cout <<"Erro semantico no endereco "<< i << ": "<< "Dado nao definido"<< endl;
                error = 1;
            }
            i++;
        }
        if (extern_id || public_id){
            if (begin==0){
                cout <<"Erro semantico: EXTERN ou PUBLIC declarados sem BEGIN"<< endl;
            }
            
        }
        if (begin){
            if (!end){
                cout <<"Erro semantico: Ausencia de END no final do modulo"<< endl;
                error = 1;
            }
        }
        //if (end){
        //    if (!begin){
        //        cout <<"Erro semantico: Ausencia de BEGIN no inicio do modulo"<< endl;
        //        error = 1;
        //    }
        //}
    }
    return error;
}