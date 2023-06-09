#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

#include "headers/PRE_PROCESSADOR.h"
// #include "headers/MONTADOR.h"
#include "headers/LIGADOR.h"

int main (int argc, char *argv[]) {

    vector<string> pre_processed_files, assembled_files;

    assembled_files = {"MOD1.obj", "MOD2.obj"}; // para testes enquanto o montador não é integrado
    
    pre_process(argc, argv, pre_processed_files);
    // assemble(pre_processed_files, assembled_files);
    link(assembled_files);

    return 0;
}