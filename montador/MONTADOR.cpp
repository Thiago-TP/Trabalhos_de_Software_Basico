// g++ MONTADOR.cpp -o MONTADOR -Wall
// ./MONTADOR teste1.asm

//FEITO
// TABELA CÓDIGO MÁQUINA
//Coluna de comando OPCODE/diretiva
//Coluna de endereço com ainda com Label

//PRÓXIMO PASSO
//colocar valor do endereço no lugar da label, criação e aplicação da tabela de simbolos
//escolha se vai ser mostrado em linha única

//Verificar depois/talvez já feito
// VERIFICACAO DE ERRO, ROTULOS MESMA LINHA, ROTULO NAO DEF EM TEXT, DADO NAO DEF EM DATA, FALTA TEXT, CUIDADO BEGIN END, LEXICO 
// TABELA DE DEFINICOES
// AJUSTE DE DIRETIVA SPACE
// AJUSTE DE SAIDA EM DECIMAL, DIRETIVA CONST
// ESPACO MINIMO DE TOKEN
// ARGUMENTO EM SPACE
// AJUSTAR NOME DE SAIDA, EXTENSAO

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;  // Evita usar "std::"

int single_pass                     (const string&);

int main(int argc, char *argv[]) 
{
    const string& source_file_name = argv[1]; // ./MONTADOR <arquivo> talvez o idx mude no Linux
    if (single_pass             (source_file_name))                     {return 1;}

    return 0;
}

int sanity_check(ifstream& inputFile, ofstream& outputFile) 
{
    if (!inputFile.is_open()) {
        cout << "Falha ao abrir o arquivo de entrada." << endl;
        return 1;
    }
    if (!outputFile.is_open()) {
        cout << "Falha ao criar o arquivo de saida." << endl;
        inputFile.close();
        return 1;
    }
    return 0;
}

string tables_default(int num_tab, int num_line, int num_column)
{
    //TABELA DE INSTRUÇÕES - num_tab 1
    //NOME DA INSTRUÇÃO, OPCODE, TAMANHO(palavras)
    //NUMERO DE ARGUMENTOS = TAMANHO - 1 

    string intruction_tab[14][3]={
        {"ADD",       "01", "2"}, 
        {"SUB",       "02", "2"},
        {"MUL",       "03", "2"},
        {"DIV",       "04", "2"},
        {"JMP",       "05", "2"}, 
        {"JMPN",      "06", "2"}, 
        {"JMPP",      "07", "2"}, 
        {"JMPZ",      "08", "2"}, 
        {"COPY",      "09", "3"}, 
        {"LOAD",      "10", "2"}, 
        {"STORE",     "11", "2"}, 
        {"INPUT",     "12", "2"}, 
        {"OUTPUT",    "13", "2"}, 
        {"STOP",      "14", "1"} 
    };

    //TABELA DE DIRETIVAS - num_tab 2
    string directive_tab[2][3]={
        {"CONST","CT","1"},
        {"SPACE", "00","1"}
    };

    if (num_tab == 1){        
        return intruction_tab[num_line][num_column];
    }

    if (num_tab == 2){
        return directive_tab[num_line][num_column];
    }
    return "TAB ERROR";
}

int instruction_directive_label(ofstream& outputFile, string line, size_t pos_label)
{
    string next_label;
    string constant_value;

    next_label = line.substr(pos_label + 2, line.size() - pos_label - 2);   // substring -> linha de comando da label

    size_t pos_next_label = next_label.find(' ');

    // segundo token do comando da label, caso seja diretiva CONST, O TOKEN será o valor de CONST
    constant_value = next_label.substr(pos_next_label + 1, next_label.size() - pos_next_label - 1); 

    // primeiro token do comando da label
    next_label = next_label.substr(0, pos_next_label);          
    
    for (int i = 1; i < 15; i++){
        if(tables_default(1,i,0) == next_label){                // se o comando é INSTRUÇÃO retorna OPCODE dela
            if (next_label == "STOP"){
                outputFile << tables_default(1,i,1) << "\n";                // saida == STOP pula linha
            }
            else{
                outputFile << tables_default(1,i,1) << "\t";
            }
            return 0;
        }
    }

    if (tables_default(2,1,0) == next_label){                    // se o comando é DIRETIVA SPACE retorna 00 
        outputFile << "00" << "\n"; 
        return 0;
    }

    if (tables_default(2,0,0) == next_label){                    // se o comando é DIRETIVA CONST retorna VALOR DA CONST 
        outputFile << constant_value << "\n";
        return 0;
    }
    return 1;
}

int comand_column(ofstream& outputFile, string line) 
{   
    //string::npos maior valor de size_t, indica valor não encontrado

    //BUSCAR COMANDOS PRIMEIRA COLUNA DO ARQUIVO DE SAIDA

    //Instrução indireta - Existe label -> : encontrado ->  Verifica qual instrução ou diretiva após a Label
    size_t pos_label = line.find(':');
    if (pos_label != string::npos){ 
        instruction_directive_label(outputFile,line, pos_label);  // Código do comando OPCODE/00/valor CONST vai pra saida
    }

    // Instrução direta - sem label
    else{   
        size_t pos = line.find(' ');
        if (pos != string::npos || line == "STOP"){ 
            line = line.substr(0, pos); // Busca primeiro token (pega a substring até o ' ')
            for (int i = 1; i < 15; i++){
                if (line == tables_default(1,i,0)){
                    if (line == "STOP"){
                        outputFile << tables_default(1,i,1) << "\n";                // saida == STOP pula linha
                        }
                    else{
                        outputFile << tables_default(1,i,1) << "\t";                // saida == código tab
                    }
                }          
            }
        }    
    }
    return 0;
}

int adress_column(ofstream& outputFile, string line) 
{   
    //string::npos maior valor de size_t, indica valor não encontrado
    
    //BUSCAR ENDEREÇOS SEGUNDA COLUNA DO ARQUIVO DE SAIDA

    //Instrução indireta - Existe label -> : encontrado ->  Verifica qual instrução ou diretiva após a Label
    size_t pos_label = line.find(':');
    if (pos_label != string::npos){ 
        string next_label;
        string argmt;
        
        next_label = line.substr(pos_label + 2, line.size() - pos_label - 2);   // substring -> linha de comando da label
        size_t pos_next_label = next_label.find(' ');

        if (next_label.substr(0, pos_next_label) == "CONST" || next_label.substr(0, pos_next_label) == "SPACE"){
            return 0;   // Caso o primeiro TOKEN seja uma diretiva não tem valor na coluna de ndereço
        }
        
        // segundo token do comando da label, caso seja uma INSTRUÇÃO, O TOKEN será o ARGUMENTO
        argmt = next_label.substr(pos_next_label + 1, next_label.size() - pos_next_label - 1);
        outputFile << argmt<< "\n";                // saida da INSTRUÇÃO
    }

    // Instrução direta - sem label
    else{
        size_t pos = line.find(' ');
        if (pos != string::npos){ 
            line = line.substr(pos + 1, line.size() - pos - 1); // Busca ARGUMENTO
            outputFile << line << "\n";    // saida NOME DO ARGUMENTO
        }    
    }
    return 0;
}

int single_pass(const string& filename)
{
    cout << "Inicializado o algoritmo de passagem unica" << endl;
    ifstream input_file(filename);
    ofstream output_file("montagem");

    if (sanity_check(input_file, output_file)) {return 1;}     
    
    string line;
    while (getline(input_file, line)) {                  //Leitura linha a linha arquivo de entrada
        comand_column(output_file, line);                // Gera "coluna de comandos do arquivo de saida"
        adress_column(output_file, line);                // Gera "coluna de endereços do arquivo de saida"
    } //Final da leitura de arquivo

    input_file.close();
    output_file.close();

    cout << "Montagem realizada com sucesso.\n\n" << endl;
    return 0; 
}



