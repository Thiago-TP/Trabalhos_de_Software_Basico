#include <iostream>
#include <fstream>
#include <string>

int remove_blanks_and_comments     (const std::string&);
int capitalize_text                (const std::string&);
int equal_spacing_between_tokens   (const std::string&);
int remove_enter_after_label       (const std::string&);
int catch_double_label             (const std::string&);
int catch_absent_text_section      (const std::string&);
int catch_absent_begin_or_end      (const std::string&);
int catch_lexic_error              (const std::string&);

int main() 
{
    const std::string& source_file_name = "codigo_x85.txt";
    
    if (remove_blanks_and_comments(source_file_name))   {return 1;}
    if (capitalize_text("codigo_x85_limpo.txt"))        {return 1;}
    // equal_spacing_between_tokens();
    // remove_enter_after_label();
    // catch_double_label();
    // catch_absent_text_section();
    // catch_absent_begin_or_end();
    // catch_lexic_error();

    return 0;
}


int remove_blanks_and_comments(const std::string& filename)
{
    std::cout << "Limpando o código..." << std::endl;
    std::ifstream inputFile(filename);
    std::ofstream outputFile("codigo_x85_limpo.txt");

    if (!inputFile.is_open()) {
        std::cout << "Falha ao abrir o arquivo de entrada." << std::endl;
        return 1;
    }

    if (!outputFile.is_open()) {
        std::cout << "Falha ao criar o arquivo de saída." << std::endl;
        inputFile.close();
        return 1;
    }

    std::string line;
    while (std::getline(inputFile, line)) {

        // Remove comentário (pega a substring até o ';')
        size_t pos = line.find(';');
        if (pos != std::string::npos) {
            line = line.substr(0, pos);
        }

        // Não imprime linha em branco
        if (!line.empty()) {
            outputFile << line << std::endl;
        }

    }

    inputFile.close();
    outputFile.close();

    std::cout << "Código limpado com sucesso.\n\n" << std::endl;
    return 0;
}


int capitalize_text(const std::string& filename) 
{
    std::cout << "Deixando as linhas em maiúscula..." << std::endl;
    std::ifstream inputFile(filename);
    std::ofstream outputFile("codigo_x85_caixa_alta.txt");

    if (!inputFile.is_open()) {
        std::cout << "Falha ao abrir o arquivo fonte." << std::endl;
        return 1;
    }

    if (!outputFile.is_open()) {
        std::cout << "Falha ao criar o arquivo de destino." << std::endl;
        inputFile.close();
        return 1;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        for (char& c : line) {
            if (std::isalpha(c)) {
                c = std::toupper(c);
            }
        }
        outputFile << line << std::endl;
    }

    inputFile.close();
    outputFile.close();

    std::cout << "Linhas foram colocadas em maiúscula com sucesso.\n\n" << std::endl;
    return 0;
}
