#include "headers_passagem_unica/TRANSLATE.h"

int one_pass(string filename, vector<string>& assembled_files, size_t number_of_files);

int assemble() 
{   
    string source_file_name;                                    // Nome do arquivo a ser montado pela passagem única
    size_t count = 0;                                           // Contador da quantidade de arquivos montados
    size_t number_of_files = pre_processed_files.size();        // Numero total de arquivos a serem montados
    
    // Montagem de arquivos
    while (count<number_of_files){      
        source_file_name = pre_processed_files[count];
        if(one_pass(source_file_name, assembled_files, number_of_files))         {return 1;}
        count++;
    }
    return 0;
}

// Algoritmo de passagem única que realiza a tradução do código e gera tabelas de símbolo, uso e definição  
int one_pass(string filename, vector<string>& assembled_files, size_t number_of_files)
{
    // ENTRADA/SAIDA DE ARQUIVOS
    const string filename_input = filename;                         // Declaração do nome do arquivo de entrada vindo do pre processamento

    size_t pos = filename.find('_');                                // Encontra '_' e somente o nome antes dele é considerado
    filename = filename.substr(0, pos); 

    const string filename_output = filename + ".objt";              // Declaração do nome do arquivo traduzido               

    ifstream input_file(filename_input);                            // Declaração da variável do tipo <arquivo de entrada> atrelada a seu (nome) 
    ofstream output_file(filename_output);                          // Declaração da variável do tipo <arquivo de saída> atrelada a seu (nome)

    if (sanity_check(input_file, output_file)) {return 1;}          // Verificação de Erro de Leitura/Escrita em arquivos

    // VARIAVEIS
    
    vector <vector<string>> symbol_table;               // Tabela de Símbolos -> Vetor de linhas com colunas (outro vetor)
    vector <vector<string>> def_table;                  // Tabela de Definições 
    vector <vector<string>> use_table;                  // Tabela de Uso


    string no_reference_code[300];                      // Código sem referência posterior
    
    string token1;                                      // Token de Operação
    string token2;                                      // Token de argumento ou Constante
    string token3;                                      // Token de argumento 2 para COPY

    string command_code;                                // Código da Operação do comando correspondente
    string label;                                       // Rótulo de uma linha
    string line;                                        // Linha a ser lida do código fonte

    string arg_adress;                                  // Endereço do argumento do comando -> Rótulo de diretiva 
    string arg_adress2;                                 // Endereço do argumento 2 do comando -> Rótulo de diretiva -> COPY

    int label_number;                                   // Número do índice que o vetor guardou a label - posição (linha) da label na TS
    int line_counter = 1;                               // Contador de linha
    int location_counter = 0;                           // Contador da posição  de endereço

    int section_text_begin = 0;                         // Define inicio da SECTION TEXT
    int section_data_begin = 0;                         // Define inicio da SECTION DATA
    int section_text_end = 0;                           // Define fim da SECTION TEXT
    int begin = 0;                                      // Define uso de BEGIN
    int end = 0;                                        // Define uso de END
    int public_id = 0;                                  // Define uso de PUBLIC
    int extern_id = 0;                                  // Define uso de EXTERN
    
    bool no_section_text = false;                       // Define SECTION TEXT ainda encontrada para analise semantica
    bool link = false;                                  // Define se o código deve ser traduzido em tabela ou linha MODIFICADO MANUALMENTE PARA TESTES
    bool linker = false;                                // Define se o código objeto oferecido deve ser ou não para ligação  
    
    // INÍCIO
    cout << endl;
    cout << "Inicializado o algoritmo de passagem unica" << endl;

    // GERAÇÃO LINHA A LINHA DO CÓDIGO TRADUZIDO
    while (getline(input_file, line)){
        if(line=="END"){
            end = 1;                                                                                // END foi usado
        }
        if(line=="SECTION TEXT"){
            section_text_begin = 1;                                                                 // Inicio da SECTION TEXT
            line_counter = line_counter + 1;                                                        // Avança uma linha na contagem
        }
        else if(line=="SECTION DATA"){
            section_data_begin = 1;                                                                 // Inicio da SECTION DATA 
            line_counter = line_counter + 1;                                                    
        }                                              
        else{
            // Obtenção de elementos da linha - separação de tokens, Analisador de erros Léxicos
            scanner(def_table,line_counter, line, label, token1, token2, token3, section_text_end, section_data_begin);
            // Analisador de erros Sintáticos
            parser(line_counter, line, token1,token2,label);            
            // Analisador de erros Semanticos: Rotulo ou dado não definido nas SECTIONS TEXT e DATA
            semantic_error(begin, end, public_id, extern_id, token1, label, line_counter, section_text_begin, section_text_end,no_section_text,2,no_reference_code, location_counter);
            // Gerenciador da Tabela de Simbolos
            symbol_table_manager(label, location_counter, label_number, symbol_table, token1, use_table);  
            // Gerador de código sem referência posterior
            no_reference_code_generation(no_reference_code, line, command_code, arg_adress, arg_adress2, label, token1, token2, token3, label_number, location_counter, symbol_table, begin, use_table);
            // Gerador de código com referência posterior linha a linha após entrar na sessão dados
            later_reference_fixer(no_reference_code, label_number, symbol_table, label, token3);    
            // Avança uma linha na contagem
            line_counter = line_counter + 1;                                                
        }
    } 
    // Ao final do loop no_reference_code se torna um código com referência posterior
    
    // Atualizar Tabela de Definições de acordo com Tabela de Símbolos
    def_table_TS_table(def_table, symbol_table);
    // Analisador de erros Semanticos: existencia da SECTION TEXT
    semantic_error(begin, end, public_id, extern_id, token1, label, line_counter, section_text_begin, section_text_end, no_section_text,1,no_reference_code, location_counter);    // Erro Semantico
    // Analisador de erros Semanticos: relacionados a ligação BEGIN END EXTERN PUBLIC
    semantic_error(begin, end, public_id, extern_id, token1, label, line_counter, section_text_begin, section_text_end,no_section_text,3,no_reference_code, location_counter);// Erro Semantico
    
    if(begin==1 && number_of_files > 1){
        linker = true;
        link = true;
    }
    
    // Cria código objeto completo
    create_object_code(no_reference_code, location_counter, output_file, link);
    
    // FINAL DA LEITURA DO ARQUIVO E ESCRITA DO ARQUIVO DE CODIGO TRADUZIDO
    output_file.close();
    input_file.close();
    
    // Muda código objeto para o formato de linker ou manter mesmo arquivo mudando tipo pra .obj
    link_object_code(filename, def_table, use_table, linker, assembled_files);  

    // FIM
    cout << "Montagem realizada com sucesso.\n\n" << endl;
    return 0;
}