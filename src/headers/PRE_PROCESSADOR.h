int remove_comments                 (const string&);
int remove_blanks                   (const string&);
int capitalize_text                 (const string&);
int equal_spacing_between_tokens    (const string&);
int remove_enter_after_label        (const string&);
int move_data_section_down          (const string&);
int hex_const_to_decimal            (const string&);

int catch_absent_END                (const string&); // deve ir para o montador
int catch_double_label              (const string&); // deve ir para o montador
int catch_absent_text_section       (const string&); // deve ir para o montador

void delete_tmp_files ();
void create_asm (char *); 

void pre_process(int num, char *src_files[]){
    cout << "PRÉ-PROCESSAMENTO INICIADO\n" << endl;
    for(int i=1; i<num; i++) {
        cout << "[" << src_files[i] << "]" << endl;
        if (remove_comments             (src_files[i]))                         return;
        if (capitalize_text             ("código_x85_descomentado.asm"))        return;
        if (catch_absent_END            ("código_x85_caixa_alta.asm"))          return;
        if (remove_blanks               ("código_x85_caixa_alta.asm"))          return;
        if (equal_spacing_between_tokens("código_x85_sem_linha_vazia.asm"))     return;
        if (remove_enter_after_label    ("código_x85_espaçado.asm"))            return;
        if (catch_absent_text_section   ("código_x85_labels_sem_enter.asm"))    return;
        if (move_data_section_down      ("código_x85_labels_sem_enter.asm"))    return;
        if (hex_const_to_decimal        ("código_x85_com_DATA_embaixo.asm"))    return;
        if (catch_double_label          ("código_x85_com_DATA_embaixo.asm"))    return;
        
        create_asm(src_files[i]);
        delete_tmp_files();
    }
    cout << "PRÉ-PROCESSAMENTO BEM-SUCEDIDO\n" << endl;
}


int sanity_check(ifstream& input_file, ofstream& output_file) 
{
    if (!input_file.is_open()) {
        cout << "Falha ao abrir o arquivo de entrada." << endl;
        return 1;
    }
    if (!output_file.is_open()) {
        cout << "Falha ao criar o arquivo de saída." << endl;
        input_file.close();
        return 1;
    }
    return 0;
}


int remove_comments(const string& filename)
{
    cout << "Tirando comentários..." << endl;
    ifstream input_file(filename + ".asm");
    ofstream output_file("código_x85_descomentado.asm");

    if (sanity_check(input_file, output_file)) {return 1;} 

    string line;
    while (getline(input_file, line)) {

        // Remove comentário (pega a substring até o ';')
        size_t pos = line.find(';');
        if (pos != string::npos) {
            line = line.substr(0, pos);
        }
        output_file << line << endl;
    }

    input_file.close();
    output_file.close();

    cout << "Comentários retirados com sucesso." << endl;
    return 0;
}


int remove_blanks(const string& filename)
{
    cout << "Removendo linha em branco..." << endl;
    ifstream input_file(filename);
    ofstream output_file("código_x85_sem_linha_vazia.asm");

    if (sanity_check(input_file, output_file)) {return 1;} 

    string line;
    while (getline(input_file, line)) {

        // Não imprime linha em branco ou END
        if (!line.empty() && line.find("END") == string::npos) {
            output_file << line << endl;
        }
    }

    input_file.close();
    output_file.close();

    cout << "Linhas removidas com sucesso." << endl;
    return 0;
}


int capitalize_text(const string& filename) 
{
    cout << "Deixando as linhas em maiúscula..." << endl;
    ifstream input_file(filename);
    ofstream output_file("código_x85_caixa_alta.asm");

    if (sanity_check(input_file, output_file)) {return 1;} 

    string line;
    while (getline(input_file, line)) {
        for (char& c : line) {
            if (isalpha(c)) {
                c = toupper(c);
            }
        }
        output_file << line << endl;
    }

    input_file.close();
    output_file.close();

    cout << "Linhas foram colocadas em maiúscula com sucesso." << endl;
    return 0;
}


int catch_absent_END (const string& filename) {
    cout << "Procurando END..." << endl;
    ifstream input_file(filename);

    string line; int line_num=1;
    while (getline(input_file, line)) {
        if (line.find("END") != string::npos) {
            cout << "END encontrada." << endl;
            return 0;
        }
        ++line_num;
    }

    input_file.close();

    cout << "Erro semântico na linha " << line_num <<": END ausente no arquivo " << filename << "." << endl;
    return 1;
}


int equal_spacing_between_tokens(const string& filename)
{
    cout << "Espaçando o código..." << endl;
    ifstream input_file(filename);
    ofstream output_file("código_x85_espaçado.asm");

    if (sanity_check(input_file, output_file)) {return 1;}    

    string line;
    while (getline(input_file, line)) {

        istringstream iss(line); 
        string token;
        while (iss >> token) {

            // Separa os argumentos do copy com vírgula
            size_t pos = token.find(',');
            if (token != "," && pos != string::npos) {
                token = token.substr(0, pos) + " ,";
            }

            output_file << token << " ";
        }
        output_file << endl;
    }

    input_file.close();
    output_file.close();

    cout << "Código espaçado com sucesso." << endl;
    return 0;
}


int remove_enter_after_label(const string& filename)
{
    cout << "Tirando enters depois de label..." << endl;
    ifstream input_file(filename);
    ofstream output_file("código_x85_labels_sem_enter.asm");

    if (sanity_check(input_file, output_file)) {return 1;} 

    string line;
    while (getline(input_file, line)) {

        line.pop_back();  // O último caracter é sempre ' '
        string label = line; 
        
        if (label.back() == ':') {
            getline(input_file, line);
            output_file << label << " " << line << endl;
        }

        else {
            output_file << line << endl;
        }
    }

    input_file.close();
    output_file.close();

    cout << "Enters depois de label retirados com sucesso." << endl;
    return 0;
}


int catch_absent_text_section(const string& filename)
{
    cout << "Procurando SECTION TEXT..." << endl;
    ifstream input_file(filename);

    if (!input_file.is_open()) {
        cout << "Falha ao abrir o arquivo de entrada." << endl;
        return 1;
    }

    string line;
    while (getline(input_file, line)) {
        size_t pos = line.find("SECTION TEXT");
        if (pos != string::npos) {
            input_file.close();
            cout << "SECTION TEXT encontrada." << endl;
            return 0;
        }
    }
    cout << "Erro semântico: SECTION TEXT não encontrada." << endl;
    return 1;    
}


int move_data_section_down(const string& filename)
{
    cout << "Movendo SECTION DATA para o final do código..." << endl;
    ifstream input_file(filename);
    ofstream output_file("código_x85_com_DATA_embaixo.asm");

    if (sanity_check(input_file, output_file)) {return 1;}

    int t_section_index, d_section_index, line_index=0; 
    vector<string> lines;
    string line;

    while (getline(input_file, line)) {
        lines.push_back(line);
        size_t aux_t = line.find("SECTION TEXT"); if (aux_t != string::npos) {t_section_index = line_index;}
        size_t aux_d = line.find("SECTION DATA"); if (aux_d != string::npos) {d_section_index = line_index;}
        ++line_index;
    }

    if (d_section_index > t_section_index) {
        for (const auto& modifiedLine : lines) {
            output_file << modifiedLine << endl;
        }
        input_file.close();
        output_file.close();
        cout << "SECTION DATA movida com sucesso." << endl;
        return 0;
    }

    vector<string> other_slice(lines.begin(), lines.begin() + d_section_index);
    vector<string> data_slice(lines.begin() + d_section_index, lines.begin() + t_section_index);
    vector<string> text_slice(lines.begin() + t_section_index, lines.end());

    for (const auto& modifiedLine : other_slice){output_file << modifiedLine << endl;}
    for (const auto& modifiedLine : text_slice) {output_file << modifiedLine << endl;}
    for (const auto& modifiedLine : data_slice) {output_file << modifiedLine << endl;}
 
    input_file.close();
    output_file.close();
    cout << "SECTION DATA movida com sucesso." << endl;
    return 0;
}


string hex2dec(const string& token) {
    // Check if the string starts with "0x"
    if (token.size() >= 2 && token.substr(0, 2) == "0X") {
        // Remove the "0x" prefix
        string hexDigits = token.substr(2);

        // Convert hexadecimal to decimal
        unsigned long decimalValue = strtoul(hexDigits.c_str(), nullptr, 16);

        // Convert decimal to string
        ostringstream oss;
        oss << decimalValue;
        return oss.str();
    }

    return token; // Return as-is if not a valid hexadecimal string
}

int hex_const_to_decimal(const string& filename)
{
    cout << "Convertendo valores de CONST para decimal..." << endl;
    ifstream input_file(filename);
    ofstream output_file("código_x85_CONST_decimal.asm");

    if (sanity_check(input_file, output_file)) {return 1;}
    
    string line;
    while (getline(input_file, line)) {
        istringstream iss(line);
        string token;
        bool firstToken = true;

        while (iss >> token) {
            string convertedToken = hex2dec(token);
            if (firstToken) {firstToken = false;} 
            else {output_file << " ";}
            output_file << convertedToken;
        }
        output_file << endl; // Add newline after each line
    }
    
    input_file.close();
    output_file.close();
    cout << "Valores convertidos com sucesso." << endl;
    return 0;
}


int catch_double_label(const string& filename)
{
    cout << "Procurando label duplamente definida em mesma linha..." << endl;

    ifstream input_file(filename);
    if (!input_file.is_open()) {
        cout << "Falha ao abrir o arquivo de entrada." << endl;
        return 1;
    }

    string line;
    int line_number = 1;

    while (getline(input_file, line)) {
        size_t first_double_dot = line.find_first_of(':');
        size_t last_double_dot = line.find_last_of(':');

        if (first_double_dot != string::npos && 
            first_double_dot != last_double_dot) {
            cout << "Label dupla na linha " << line_number << " do arquivo " << filename << "\n\n" << endl;
            return 1;
        }

        ++line_number;
    }

    input_file.close();

    cout << "Nenhuma dupla detectada.\n\n" << endl;
    return 0;
}


void delete_tmp_files() {
    const char* filenames[] = { // arquivos temporários criados ao longo do pré-processamento
        "código_x85_descomentado.asm", 
        "código_x85_sem_linha_vazia.asm",
        "código_x85_caixa_alta.asm",
        "código_x85_espaçado.asm",
        "código_x85_labels_sem_enter.asm",
        "código_x85_com_DATA_embaixo.asm",
        "código_x85_CONST_decimal.asm"
    };

    for (auto filename : filenames) 
        if (remove(filename)) cout << "Falha ao deletar o arquivo " << filename << endl;        
}


// Cria o .asm pré-processado final, insere no vetor de arquivos pré-processados
void create_asm(char *original_name) {
    
    ifstream ifs("código_x85_CONST_decimal.asm");
    
    string aux_str = original_name;
    string new_name = aux_str + "_pré-processado.asm";
    
    ofstream MOD(new_name);
    
    while (getline(ifs, aux_str)) MOD << aux_str << endl;

    pre_processed_files.push_back(new_name);
    cout << pre_processed_files.size() << endl;
}