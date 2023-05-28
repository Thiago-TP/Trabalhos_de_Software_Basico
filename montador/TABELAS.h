#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

//TABELAS INICIAIS -> SUJEITO A ALTERAÇÃO

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