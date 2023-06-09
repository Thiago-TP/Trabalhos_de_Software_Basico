#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

#include "PRE_PROCESSADOR.h"
// #include "MONTADOR.h"
#include "LIGADOR.h"

int main (int argc, char *argv[]) {

    vector<string> pre_processed_files, assembled_files;
    assembled_files = {"MOD1.obj", "MOD2.obj"};
    
    pre_process(argc, argv, pre_processed_files);
    // assemble(pre_processed_files, assembled_files);
    link(assembled_files);

    return 0;
}