#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <vector>
#include <algorithm>

// definir vetor global para 
// a Tabela Global de Definições e
// a Tabela Global de Usos

// ideia:
// ler cada obj, extrair a tabela de uso, 
// definição e código máquina não ligados de cada um 
// criar vetor de offsets (tamanho do código que precede o módulo)
// criar a TGD
// juntar todas tabelas de uso numa estrutura só, a TGU  
// (basta somar os offsets aos números na TU)
// juntar os códigos máquina num arquivo .exc
// alterar o .exc conforme a TGD e a TGU 

// ideia:
// para cada label na TGD, criar struct com valor e usos (posição em bytes)
struct value_and_uses
{
    int value;
    vector<int> uses;
};


using namespace std;

int link(const char* objfile_name) {
    cout << objfile_name << endl;
    return 0;
}

int main (int argc, char *argv[]) {
    // teste porque não sei usar argv/argc
    // argv[1:] deve ter os nomes dos arquivos obj
    for(int i=1; i<argc; i++) {
        link(argv[i]); 
        cout << i << endl;
    }

    return 0;
}