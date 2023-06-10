void begin_exc_and_get_offsets(const string&);
void fill_global_tables_and_exc(const string&, int);
void make_struct_vector();
void finish_executable(const string&);
void delete_tmp_tables();

void link(vector<string> obj_files) {

    if (obj_files.size() == 1) {
        cout << "LIGAÇÃO NÃO NECESSÁRIA\n" << endl;

        string label = obj_files[0].substr(0, obj_files[0].find(".obj"));
        const char* exc_name = (label+".exc").c_str();

        if (!rename(obj_files[0].c_str(), exc_name)) {        
            cout << "Código executável " << exc_name << " montado com sucesso.\n\n" << endl;
        }
        return;
    }

    cout << "LIGAÇÃO INICIADA\n" << endl;
    long unsigned int i;
    for(i=0; i<obj_files.size(); i++) {
        cout << "[" << obj_files[i] << "]" << endl;
        begin_exc_and_get_offsets(obj_files[i]);
        fill_global_tables_and_exc(obj_files[i], i); 
    }
    make_struct_vector(); 
    finish_executable(obj_files[0]);
    delete_tmp_tables();

    cout << "LIGAÇÃO BEM-SUCEDIDA\n" << endl;
}

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
// tabela de endereços relativos global é montada
// juntando as tabelas locais somadas aos offsets
// primeiro percorre-se o código não ligado de forma que,
// se o número no índice apontado pela tabela global é positivo (não é EXTERN),
// soma-se a ele o offset do módulo
// depois disso, usam-se a TGD e a TGU para atualizar os relativos nulos

// struct valor-posições
struct VALUE_POS{
    string label;   // label apenas para debug
    int value;
    vector<int> positions;
};

vector<int> offsets(1, 0);              // vetor de fatores de correção (iniciado como [0])
vector<VALUE_POS> value_positions;      // vetor label-valor-posições
vector<vector<int>> relatives;          // vetor de todos os endereços relativos, módulo a módulo
vector<int> exc_vector;                 // vetor do executável
ofstream TGD_file("TGD.txt");           // tabela global de definições
ofstream TGU_file("TGU.txt");           // tabela global de usos


void begin_exc_and_get_offsets(const string& objfile_name) {
    cout << "Inicializando executável com " << objfile_name << "..." << endl;
    ifstream input_file (objfile_name);

    if (!input_file.is_open()) {
        cout << "Falha ao abrir o arquivo de entrada." << endl;
        return;
    }

    string code_line;  
    int code_size = 0, code_num;   
    while (getline(input_file, code_line)) {
        if (code_line == "CODE") {
            getline(input_file, code_line); 
            istringstream iss(code_line);
            while (iss >> code_num) {
                ++code_size;
                exc_vector.push_back(code_num);
            }
        }
    }
    offsets.push_back(*(--offsets.end()) + code_size);
    cout << "Executável com " << objfile_name << " inicializado.\n\n" << endl;
}

string offset(const string& table_line, int file_index) {
    istringstream iss(table_line);
    ostringstream oss;
    string token;
    while (iss >> token) {
        try {
            int num = stoi(token) + offsets[file_index];
            token = to_string(num);
            oss << token << " ";
        } catch (const invalid_argument& _) {
            oss << token << " ";
        }
    }
    return oss.str();
}

void fill_global_tables_and_exc(const string& objfile_name, int file_index) {
    ifstream input_file (objfile_name);

    cout << "Construindo TGD com " << objfile_name << "..." << endl;
    if (sanity_check(input_file, TGD_file)) {return;} 
    bool in_DEF = true; string def_line;     
    while (in_DEF) { 
        getline(input_file, def_line);        
        if      (def_line == "USO") {in_DEF = false;}
        else if (def_line != "DEF") {TGD_file << offset(def_line, file_index) << endl;}
    }
    cout << "TGD com " << objfile_name << " construída.\n\n" << endl;


    cout << "Construindo TGU com " << objfile_name << "..." << endl;
    if (sanity_check(input_file, TGU_file)) {return;} 
    bool in_USE = true; string use_line;     
    while (in_USE) {
        getline(input_file, use_line);        
        if      (use_line == "RELATIVOS")   {in_USE = false;}
        else if (use_line != "USO")         {TGU_file << offset(use_line, file_index) << endl;}
    }
    cout << "TGU com " << objfile_name << " construída.\n\n" << endl;


    cout << "Construindo TGR com " << objfile_name << "..." << endl;
    int rel_num;
    vector<int> current_vector;
    bool in_REL = true; string rel_line;     
    while (in_REL) {
        getline(input_file, rel_line);        
        if      (rel_line == "CODE")        {in_REL = false;}
        else if (rel_line != "RELATIVOS")   {
            istringstream iss(rel_line);
            while (iss >> rel_num)
                current_vector.push_back(rel_num + offsets[file_index]);
        }
    }
    relatives.push_back(current_vector);
    cout << "TGR com " << objfile_name << " construída.\n\n" << endl;
}

vector<string> ordered_strings (const string& filename) {
    string line;
    vector<string> v;
    ifstream ifs (filename);

    while (getline(ifs, line)) v.push_back(line);
    ifs.close();

    sort(v.begin(), v.end());
    return v;
}
void make_struct_vector() {
    cout << "Combinando TGD e TGU em um vetor..." << endl;

    vector<string> ordered_TGD = ordered_strings("TGD.txt");
    vector<string> ordered_TGU = ordered_strings("TGU.txt");

    string TGD_token, TGU_token;
    for (long unsigned int i=0; i<ordered_TGD.size(); i++) {
        VALUE_POS current_struct;

        istringstream TGD_ifs (ordered_TGD[i]);
        istringstream TGU_ifs (ordered_TGU[i]);

        while (TGD_ifs >> TGD_token) { 
            try{
                current_struct.value = stoi(TGD_token);
            } catch(const std::exception& _){
                current_struct.label = TGD_token;
            }
        }    

        while (TGU_ifs >> TGU_token) { 
            try{
                current_struct.positions.push_back(stoi(TGU_token));
            } catch(const std::exception& _){}
        }     

        value_positions.push_back(current_struct);
    }
    cout << "Combinação concluída.\n\n" << endl;
}

void finish_executable(const string& first_mod_name) {
    cout << "Finalizando executável..." << endl;

    vector<int> passed_relatives;
    for (auto info : value_positions) {
        for (int index : info.positions) {
            exc_vector[index] += info.value;
            passed_relatives.push_back(index);
        }
    }

    long unsigned int i;
    for (i=0; i<relatives.size(); i++) {
        for (auto relative : relatives[i]) {
            bool done_value = false;
            for (int value : passed_relatives)
                if (value == relative) done_value = true;
            if (!done_value) 
                exc_vector[relative] += offsets[i];
        }
    }

    size_t pos = first_mod_name.find(".obj");
    string exc_name = first_mod_name.substr(0, pos) + ".exc";
    ofstream EXC_file (exc_name);
    for (auto num : exc_vector) EXC_file << num << " ";
    EXC_file.close();
    
    cout << "Código executável " << exc_name << " terminado com sucesso.\n\n" << endl;
}

void delete_tmp_tables() {
    const char* filenames[] = {"TGD.txt", "TGU.txt"};
    for (auto filename : filenames) 
        if (remove(filename)) cout << "Falha ao deletar o arquivo " << filename << endl;        
}
