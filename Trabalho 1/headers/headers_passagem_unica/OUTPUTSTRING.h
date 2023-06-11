// AQUIVO CRIADOR DE ARQUIVOS DE TEXTO PARA SAÍDA

// Cria código traduzido em arquivo de texto
void create_object_code(string no_reference_code[], int location_counter, ofstream& outputFile, bool link)
{
    bool section_data_start = false;

    if (link==false){
        for (int i = 0; i < location_counter-1; i++)
        {
            outputFile << no_reference_code[i]<< "\t";
        }
        outputFile << no_reference_code[location_counter-1];               
    }
    else{
        for (int i = 0; i < location_counter; i=i+2)
        {
            if(no_reference_code[i] == "00" || no_reference_code[i] == "14"){
                section_data_start = true;
            }
            if (section_data_start){
                if (i == location_counter-1){
                    outputFile << no_reference_code[i];
                }
                else{
                    outputFile << no_reference_code[i]<< "\n";
                    i--;
                }
            }
            else{
                if(no_reference_code[i]=="09"){
                    outputFile << no_reference_code[i]<< "\t";
                    outputFile << no_reference_code[i+1]<< "\t";
                    outputFile << no_reference_code[i+2]<< "\n";
                    i++;
                }
                else{
                    outputFile << no_reference_code[i]<< "\t";
                    outputFile << no_reference_code[i+1]<< "\n";
                }
            }
        }
    }
}

// Cria arquivo de texto .obj com as tabelas e código traduzido
int link_object_code(string filename, vector <vector<string>>& def_table, vector <vector<string>>& use_table, bool linker, vector<string>& assembled_files)
{
    const string filename_input = filename + ".objt";            
    const string filename_output = filename + ".obj";

    ifstream input_file(filename_input);                          // Declaração da variável do tipo <arquivo de entrada> atrelada a seu (nome) 
    ofstream output_file(filename_output);                        // Declaração da variável do tipo <arquivo de saída> atrelada a seu (nome)

    // VETORES AUXILIARES PARA GERAÇÃO DA TABELA DE TEXTO CORRESPONDENTE
    string code[150];
    string relatives[300];
    string def[300];
    string use[300];

    string line;        // Varivel de linha atual da leitura
    size_t pos;         // Valor da posição " " primeiro espaço vazio da linha

    int i=0;
    int j=0;
    int p=0;
    int r=0;
    size_t m=0;
    size_t q=0;

    // CASO NÃO TENHA LIGAÇÃO COPIAR CÓDIGO JÁ GERADO
    
    if (!linker){
        while (getline(input_file, line)){
            output_file<<line<<endl;
        }
        assembled_files.push_back(filename + ".obj");

        output_file.close();
        input_file.close();
        
        return 0;
    }
    
    // LEITURA DO ARQUIVO DE CÓDIGO PARA SEPARAR COLUNA DE CODIGO E ENDEREÇO RELATIVO
    while (getline(input_file, line)){
        pos = line.find('\t');                                   

        if (pos != string::npos){                               
            code[i] = line.substr(0, pos);
            i++;

            line = line.substr(pos+1, line.size()-1);

            pos = line.find('\t');
            if (pos != string::npos){
                if (line.substr(0, pos) != "0"){
                    relatives[j] = line.substr(0, pos);
                    j++;
                }                 

                line = line.substr(pos+1, line.size()-1);

                if (line != "0"){
                    relatives[j] = line;
                    j++;
                }
            }
            else{
                if (line != "0"){
                    relatives[j] = line;
                    j++;
                }
            }
        }
        else{
            code[i] = line;
            i++;
        }           
    }

    // GERAÇÃO DO VETOR DA TABELA DE DEFINIÇÃO
    while (m < def_table.size()){
        def[p] = def_table[m][0];
        p++;
        def[p] = def_table[m][1];
        p++;
        m++;
    }

    // GERAÇÃO DO VETOR DA TABELA DE USO
    while (q < use_table.size()){
        use[r] = use_table[q][0];
        r++;
        use[r] = use_table[q][1];
        r++;
        q++;
    }

    // IMPRESSÃO DA TABELA DE USO
    output_file <<"USO"<< endl;
    
    for (int s = 0; s < r-2; s++){
        output_file <<use[s]<< "\t";
        s++;
        output_file <<use[s]<< "\n";
    }
    output_file <<use[r-2]<< "\t";
    output_file <<use[r-1];

    output_file<< endl;

    // IMPRESSÃO DA TABELA DE DEFINIÇÕES
    output_file <<"DEF"<< endl;
    
    for (int l = 0; l < p-2; l++){
        output_file <<def[l]<< "\t";
        l++;
        output_file <<def[l]<< "\n";
    }
    output_file <<def[p-2]<< "\t";
    output_file <<def[p-1];

    output_file<< endl;

    // IMPRESSÃO DOS ENDEREÇOS RELATIVOS
    output_file <<"RELATIVOS"<< endl;
    for (int k = 0; k < j-1; k++){
        output_file <<relatives[k]<< "\t";
    }
    output_file <<relatives[j-1];

    output_file<< endl;

    // IMPRESSÃO DOS CÓDIGOS DE INSTRUÇÕES
    output_file <<"CODE"<< endl;
    for (int n = 0; n < i-1; n++){
        output_file <<code[n]<< "\t";
    }
    output_file <<code[i-1];

    assembled_files.push_back(filename + ".obj");

    output_file.close();
    input_file.close();
    return 0;
}