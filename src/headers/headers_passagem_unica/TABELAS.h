//TABELA DE INSTRUÇÕES 
string tables_default(int num_line, int num_column)
{
    
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
    return intruction_tab[num_line][num_column];
}
