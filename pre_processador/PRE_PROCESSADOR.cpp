#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;  // Evita usar "std::" toda hora

int remove_blanks_and_comments     (const string&);
int capitalize_text                (const string&);
int equal_spacing_between_tokens   (const string&);
int remove_enter_after_label       (const string&);
int catch_double_label             (const string&);
int catch_absent_text_section      (const string&);
int catch_absent_begin_or_end      (const string&);
int catch_lexic_error              (const string&);

int main() 
{
    const string& source_file_name = "codigo_x85.txt";  // Vai virar "argv"
    
    if (remove_blanks_and_comments(source_file_name))               {return 1;}
    if (capitalize_text("codigo_x85_limpo.txt"))                    {return 1;}
    if (equal_spacing_between_tokens("codigo_x85_caixa_alta.txt"))  {return 1;}
    if (remove_enter_after_label("codigo_x85_espaçado.txt"))        {return 1;}
    // catch_double_label();
    // catch_absent_text_section();
    // catch_absent_begin_or_end();
    // catch_lexic_error();

    return 0;
}


int sanity_check(ifstream& inputFile, ofstream& outputFile) 
{
    if (!inputFile.is_open()) {
        cout << "Falha ao abrir o arquivo de entrada." << endl;
        return 1;
    }
    if (!outputFile.is_open()) {
        cout << "Falha ao criar o arquivo de saída." << endl;
        inputFile.close();
        return 1;
    }
    return 0;
}


int remove_blanks_and_comments(const string& filename)
{
    cout << "Limpando o código..." << endl;
    ifstream inputFile(filename);
    ofstream outputFile("codigo_x85_limpo.txt");

    if (sanity_check(inputFile, outputFile)) {return 1;} 

    string line;
    while (getline(inputFile, line)) {

        // Remove comentário (pega a substring até o ';')
        size_t pos = line.find(';');
        if (pos != string::npos) {
            line = line.substr(0, pos);
        }
        // Não imprime linha em branco
        if (!line.empty()) {
            outputFile << line << endl;
        }
    }

    inputFile.close();
    outputFile.close();

    cout << "Código limpado com sucesso.\n\n" << endl;
    return 0;
}


int capitalize_text(const string& filename) 
{
    cout << "Deixando as linhas em maiúscula..." << endl;
    ifstream inputFile(filename);
    ofstream outputFile("codigo_x85_caixa_alta.txt");

    if (sanity_check(inputFile, outputFile)) {return 1;} 

    string line;
    while (getline(inputFile, line)) {
        for (char& c : line) {
            if (isalpha(c)) {
                c = toupper(c);
            }
        }
        outputFile << line << endl;
    }

    inputFile.close();
    outputFile.close();

    cout << "Linhas foram colocadas em maiúscula com sucesso.\n\n" << endl;
    return 0;
}


int equal_spacing_between_tokens(const string& filename)
{
    cout << "Espaçando o código..." << endl;
    ifstream inputFile(filename);
    ofstream outputFile("codigo_x85_espaçado.txt");

    if (sanity_check(inputFile, outputFile)) {return 1;}    

    string line;
    while (getline(inputFile, line)) {

        istringstream iss(line); 
        string token;
        while (iss >> token) {

            // Separa os argumentos do copy com vírgula
            size_t pos = token.find(',');
            if (pos != string::npos) {
                token = token.substr(0, pos) + " ,";
            }

            outputFile << token << " ";
        }
        outputFile << endl;
    }

    inputFile.close();
    outputFile.close();

    cout << "Código espaçado com sucesso.\n\n" << endl;
    return 0;
}


int remove_enter_after_label(const string& filename)
{
    cout << "Tirando enters depois de label..." << endl;
    ifstream inputFile(filename);
    ofstream outputFile("codigo_x85_labels_sem_enter.txt");

    if (sanity_check(inputFile, outputFile)) {return 1;} 

    string line;
    while (getline(inputFile, line)) {

        line.pop_back();  // O último caracter é sempre ' '
        string label = line; 
        
        if (label.back() == ':') {
            getline(inputFile, line);
            outputFile << label << " " << line << endl;
        }

        else {
            outputFile << line << endl;
        }
    }

    inputFile.close();
    outputFile.close();

    cout << "Enters depois de label retirados com sucesso.\n\n" << endl;
    return 0;
}
