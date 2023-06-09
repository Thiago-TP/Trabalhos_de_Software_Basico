# Software-Basico
Este repositório guarda os trabalhos da disciplina CIC0104 - SOFTWARE BASICO 
com o professor Bruno Luiggi Macchiavello Espinoza (UnB, abril-julho de 2023) 
feitos pela dupla 

- Cleyton Erick Caldas Sá - 190026201 
- Thiago Tomás de Paula - 190038641

A seguir apresentam-se as documentações de cada trabalho feito pela dupla.

> Nota: foi pedido que o trabalho 1 fosse feito em C ou C++.
> Nenhum de nós sabia usar C++, e quanto ao C, tivemos apenas uma introdução básica 2019.
> Dessa forma, frequentemente recorremos ao ChatGPT para tirar dúvidas de C++ ou pedir sugestões de código.  

---
## Trabalho 1: Montador do assembly inventado
Neste trabalho, o montador deve receber 1 ou mais módulos do assembly inventado apresentado em sala,
(arquivos `.asm`) e retornar o arquivo executável `.exc` relacionado aos programas.
Requisitos completos do trabalho se encontram no `.pdf` neste repositório.
Aqui segue uma documentação rápida dos programas centrais que formam o trabalho como um todo:

- O Pré-processador, no `PRE_PROCESSADOR.h`. <br />
Gera arquivos `.asm` que correspondem aos `.asm` fornecidos na linha de comando
mas sem espaços vazios e comentários. 
Também realiza outros ajustes estéticos úteis no código,
em preparação para o montador. 

- O Montador, no `MONTADOR.h`. <br />
Gera os arquivos objeto (`.obj`) de cada .asm gerado pelo pré-processamento,
assim como detecta erros léxicos, sintáticos e semânticos nesses .asm.
Na detecção de erro, os `.obj` ficam vazios, i. e., sem números.

- O Ligador, no `LIGADOR.h`. <br />
Recebe os códigos objetos criados no montador, e deles gera o arquivo executável
`.exc`, que consiste em um arquivo texto de uma linha com números decimais separados por espaço.

Os headers acima são usados na `main.cpp`, e todos são arquivos na pasta `src`.
Para rodar o trabalho, coloque o executável main no diretório com os `.asm` de interesse e entre com 
```
./main programa1.asm programa2.asm
``` 
na linha de comando. 
O resultado será o arquivo .exc que será criado na mesma pasta.
Em src, existe como exemplo o executável dos módulos `MOD1.asm` e `MOD2.asm`,
que juntos recebem 3 inteiros positivos e imprimem na tela o menor deles e em seguida o maior
(escrever, montar e ligar esses programas foi a questão 3 da prova 1 deste semestre).  

#### Documentação do pré-processador
O pré-processamento se dá pela chamada em sequência de 11 funções (que por sua vez podem chamar subfunções), 
através da função principal `pre_process`, sendo a maioria delas mudanças estéticas sobre os códigos fontes.
Módulo a módulo, em ordem, faz-se:
1. os comentários são removidos, função `remove_comments`; 
2. linhas em branco são apagadas, função `remove_blanks`;
3. todo o texto é posto em maiúscula, função `capitalize_text`;
4. o espaçamento entre os tokens ficam iguais a um " ", função `equal_spacing_between_tokens`;
5. espaço vazio após label é preenchido com a linha debaixo, função `remove_enter_after_label`;
6. a presença de SECTION TEXT é procurada, função `catch_absent_text_section`;
7. o bloco DATA é movido para baixo do bloco TEXT (caso existam), função `move_data_section_down`;
8. valores hexadecimais na diretiva `CONST` são convertidos para decimais, função `hex_const_to_decimal`;
9. a presença de duas labels na mesma linha de código é verificada, função `catch_double_label`;
10. cria-se o `asm` do arquivo pré-processado final, colocando seu nome no vetor de strings `pre_processed_files`;
11. deletam-se os arquivos temporários criados nos passos 1-5, 7 e 8.

Nenhuma função modifica os arquivos fonte, ou apenas os leem ou criam arquivos temporários.
O `pre_processed_files` é dado pela `main` em `main.cpp`, e será a entrada da função principal no `MONTADOR.h`.

> Nota: todos os headers têm uma estrutura similar, isto é, é apresentada no início do arquivo a 
> função principal do `.h`, logo depois das *function signatures* relevantes.

> Nota: Todos os headers assumirão que labels são indicadas por ":"

#### Documentação do montador

#### Documentação do ligador
A ligação se dá pela chamada em sequência de 5 funções (que por sua vez podem chamar subfunções), 
através da função principal `link`, sendo a maioria delas mudanças estéticas sobre os códigos fontes.
Módulo a módulo, em ordem, faz-se:
1. , função `begin_exc_and_get_offsets`; 
2. , função `fill_global_tables_and_exc`;
3. , função `make_struct_vector`;
4. , função `finish_executable`;
5. , função `delete_tmp_tables`;

---
## Trabalho 2
