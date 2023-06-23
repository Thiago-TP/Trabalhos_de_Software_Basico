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
./main programa1 programa2
``` 
na linha de comando. 
O resultado será o arquivo `programa1.exc` que será criado na mesma pasta.
Em `src`, existe como exemplo o executável e objetos dos módulos `MOD1.asm` e `MOD2.asm`,
que juntos recebem 3 inteiros positivos e imprimem na tela o menor deles e em seguida o maior
(escrever, montar e ligar esses programas foi a questão 3 da prova 1 deste semestre).  

#### Documentação do pré-processador
O pré-processamento se dá pela chamada em sequência de 11 funções (que por sua vez podem chamar subfunções), 
através da função principal `pre_process`, sendo a maioria delas mudanças estéticas sobre os códigos fontes.
Módulo a módulo, em ordem, faz-se:
1. os comentários são removidos, função `remove_comments`; 
2. todo o texto é posto em maiúscula, função `capitalize_text`;
3. linhas em branco são apagadas, função `remove_blanks`;
4. o espaçamento entre os tokens ficam iguais a um " ", função `equal_spacing_between_tokens`;
5. espaço vazio após label é preenchido com a linha debaixo, função `remove_enter_after_label`;
6. a presença de SECTION TEXT é procurada, função `catch_absent_text_section`;
7. o bloco DATA é movido para baixo do bloco TEXT (caso existam), função `move_data_section_down`;
8. valores hexadecimais na diretiva `CONST` são convertidos para decimais, função `hex_const_to_decimal`;
9. a presença de duas labels na mesma linha de código é verificada, função `catch_double_label`;
10. cria-se o `asm` do arquivo pré-processado final, colocando seu nome no vetor de strings `pre_processed_files`;
11. deletam-se os arquivos temporários criados nos passos 1, 2, 4, 5, 6, 8 e 9, função `delete_tmp_files`.

O próximo passo só executa se o anterior for bem sucedido, como avisarão as mensagens na tela.
Nenhuma função modifica os arquivos fonte, ou apenas os leem ou criam arquivos temporários.
O `pre_processed_files` é dado pela `main` em `main.cpp`, e será a entrada da função principal no `MONTADOR.h`.
Observe que os arquivos resultantes do pré-processamento bem sucedido terminam em `_pré-processado.asm`, e não possuem `END`.

> Nota: todos os headers têm uma estrutura similar, isto é, é apresentada no início do arquivo a 
> função principal do `.h`, logo depois das *function signatures* relevantes.

> Nota: Todos os headers assumirão que labels são indicadas por ":"

#### Documentação do montador
O montador receber um vetor de strings cujos elementos são os nomes dos arquivos pré-processados a serem montados,
e sempre retorna um arquivo `.obj` para cada arquivo de entrada.
Reconhecemos que isso vai contra o requerimento que o montador gere `.exc`

O arquivo `.obj` tem o formato indicado no `.pdf`, sendo o campo `CODE` as tuplas inteiras 
(OpCode, OP1, OP2) de cada instrução, colocadas em única linha. 
Caso a instrução use 1 argumento, a tupla se torna (OpCode, OP1), e caso não se utilize nenhum argumento, apenas (OpCode).
Para a diretiva CONST, o OpCode é o número indicado, e para a SPACE, 0.
As instruções processadas pelo montador da dupla são detalhadas abaixo.
  
| Instrução | OpCode | Tamanho (palavras) | Ação |
| --------- | :----: | :----------------: | ---- |
| ADD    |  1 | 2 | ACC <- ACC + mem(OP) | 
| SUB    |  2 | 2 | ACC <- ACC - mem(OP) | 
| MUL    |  3 | 2 | ACC <- ACC * mem(OP) | 
| DIV    |  4 | 2 | ACC <- ACC / mem(OP) | 
| JMP    |  5 | 2 | PC <- OP             | 
| JMPN   |  6 | 2 | PC <- OP se ACC<0    | 
| JMPP   |  7 | 2 | PC <- OP se ACC>0    | 
| JMPZ   |  8 | 2 | PC <- OP se ACC=0    | 
| COPY   |  9 | 3 | mem(OP2) <- mem(OP1) | 
| LOAD   | 10 | 2 | ACC <- mem(OP)       | 
| STORE  | 11 | 2 | mem(OP) <- ACC       | 
| INPUT  | 12 | 2 | mem(OP) <- entrada   | 
| OUTPUT | 13 | 2 | saída <- mem(OP)     | 
| STOP   | 14 | 1 | Suspende a execução  |

#### Documentação do ligador
A ligação se dá pela chamada em sequência de 5 funções (que por sua vez podem chamar subfunções), 
através da função principal `link`. 
Caso apenas um arquivo tenha sido montado 
(i. e., o vetor de string global `assemble_files` tem apenas uma entrada), 
`link` apenas renomeia o arquivo `.obj` de saída do montador para `.exc`, e a compilação está completa.
Caso contrário, módulo a módulo, em ordem, chama-se:

1. função `begin_exc_and_get_offsets`: <br />
   o código de máquina não ligado é colocado no vetor de inteiros `exc_vector`
   e o fator de correção é calculado e inserido no vetor de inteiros `offsets`
; 
2. função `fill_global_tables_and_exc`: <br />
   as tabela de definições, uso, e relativos do módulo 
   são somadas ao fator de correção apropriado guardado em `offsets`,
   formando uma porção da 
   Tabela Global de Definições (arquivo temporário `TGD.txt`), 
   Tabela Global de USO (arquivo temporário `TGU.txt`), 
   Tabela Global de Relativos.
   A TGR está guardada pelo vetor de vetor de inteiros `relatives`, 
   e cada módulo corresponde a um vetor
;
3. função `make_struct_vector`: <br />
   `TGD.txt` e `TGU.txt` são passadas para vetores de strings que são em seguida ordenados.
   Esses vetores são então lidos e seus conteúdos passados para uma struct que guarda 
   a label (`string`),
   o endereço na memória dessa label (`int`),
   e os endereços relativos onde essa label é chamada/usada (vector de inteiro).
   Finalizada a struct, esta é colocada no vetor `value_positions`
;
4. função `finish_executable`: <br />
   com `value_positions` em mãos, o `exc_vector` não ligado é revisitado,
   de forma que as labels na tabela de uso ganham seus devidos valores e, feito isso,
   o restante dos endereços relativos é somando ao devido fator de correção em `offsets`.
   A essa altura `exc_vector` contém o código de máquina ligado e 
   seu conteúdo é passado para o arquivo `.exc`.
;
5. função `delete_tmp_tables`: <br />
   deletam-se a `TGD.txt` e a `TGU.txt`.