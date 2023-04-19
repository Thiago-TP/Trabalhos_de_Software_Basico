import sys

#############################################
#   Cleyton Erick Caldas Sá -- 190026201    #
#   Thiago Tomás de Paula   -- 190038641    #
#   19/04/2023 a XX/YY/2023                 #
#############################################

#####################################################################################################
#                                       MONTADOR DE ASSEMBLY X85                                    #
#   Trabalho 1 da disciplina                                                                        #
#   CIC0104 - Software Básico - Turma 01 - 2023.1 - Prof. Dr. BRUNO LUIGGI MACCHIAVELLO ESPINOZA    #
#####################################################################################################

#   Entrada ≥ programa x85 num .txt
#   Saída ≥ código executável num .txt
#   Formatação: PEP8 (https://peps.python.org/pep-0008/)

#   Fim da identificação    


symbol_table = {}  # Esta tabela será construída na primeira passagem do compilador

opp_table = {
    'ADD':      {'CODE':  1, 'SIZE': 2},
    'SUB':      {'CODE':  2, 'SIZE': 2},
    'MUL':      {'CODE':  3, 'SIZE': 2},
    'DIV':      {'CODE':  4, 'SIZE': 2},
    'JMP':      {'CODE':  5, 'SIZE': 2},
    'JMPN':     {'CODE':  6, 'SIZE': 2},
    'JMPP':     {'CODE':  7, 'SIZE': 2},
    'JMPZ':     {'CODE':  8, 'SIZE': 2},
    'COPY':     {'CODE':  9, 'SIZE': 3},
    'LOAD':     {'CODE': 10, 'SIZE': 2},
    'STORE':    {'CODE': 11, 'SIZE': 2},
    'INPUT':    {'CODE': 12, 'SIZE': 2},
    'OUTPUT':   {'CODE': 13, 'SIZE': 2},
    'STOP':     {'CODE': 14, 'SIZE': 1}
}

directive_table = {
    'CONST': 1,
    'SPACE': 1
}


# Realiza a primeira passagem do compilador conforme o algoritmo na pg. 27 da Aula 4
def first_pass(file_name):

    position_counter = 0
    line_counter = 1

    # Coloca as linhas do código assembly numa lista
    with open(file_name, 'r') as f:
        assembly_lines = f.readlines()
    
    # Obtém uma linha da fonte
    for line in assembly_lines:  

        # Separa os elementos da linha
        parsed_line = parse(line)
        
        # Atualiza a tabela de símbolos, se necessário
        if parsed_line['LABEL'] in symbol_table:
            print(f"Erro na linha {line_counter}: símbolo {parsed_line['LABEL']} redefinido")
            return 1  # Saída da função
        
        elif parsed_line['LABEL'] is not None:
            symbol_table[parsed_line['LABEL']] = position_counter 
        
        # Trata operação ou diretiva
        if parsed_line['OPP'] in opp_table:
            position_counter += opp_table[parsed_line['OPP']]['SIZE']
        
        elif parsed_line['OPP'] in directive_table:
            position_counter += directive_table[parsed_line['OPP']]
        
        else:
            print(f"Erro na linha {line_counter}: operação {parsed_line['OPP']} não identificada")
            return 1  # Saída da função
        
        # Segue para a próxima linha
        line_counter += 1
    
    # Fim da função
    return symbol_table


def parse(line):
    parsed_line = {'LABEL': None, 'OPP': None, 'ARG1': None, 'ARG2': None}  # OPP pode também ser uma diretiva

    # Ignora os comentários
    commentless_line = ''
    for char in line:
        if char == ';':
            break
        commentless_line += char

    # Separa os elementos da linha
    commentless_line = commentless_line.split()

    # Preenche a linha organizada  
    for token in commentless_line:
        if ':' in token:
            parsed_line['LABEL'] = token[:-1]  # apenas o último caracter do tolken é retirado ('abc:' -> 'abc')

        elif parsed_line['OPP'] is None:
            parsed_line['OPP'] = token

        elif parsed_line['ARG1'] is None:
            parsed_line['ARG1'] = token

        elif parsed_line['ARG2'] is None:
            parsed_line['ARG2'] = token
    
    # Fim da função
    return parsed_line


if __name__ == "__main__":
    # s = 'abc:'
    # print(s[:-1])

    # print(parse('ADD N1\n'))
    # print(parse('ADD N1    ;soma N1 a ACC'))
    # print(parse('SOMA: ADD N1'))
    # print(parse('SOMA:    ADD N1    ;soma N1 a ACC'))

    # parsed_line = {'OPP': 'ADD'}
    # position_counter = 0
    # if parsed_line['OPP'] in opp_table:
    #     position_counter += opp_table[parsed_line['OPP']]['SIZE']
    # print(position_counter)

    symbol_dict = first_pass(sys.argv[1])
    with open('symbol_table.txt', 'w') as f:
        sys.stdout = f
        print(symbol_dict)
