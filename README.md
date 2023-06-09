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
(arquivos .asm) e retornar o arquivo executável .exc relacionado aos programas.
Requisitos completos do trabalho se encontram no .pdf neste repositório.
Aqui segue uma documentação rápida dos programas centrais que formam o trabalho como um todo:

- [x] O Pré-processador, no PRE_PROCESSADOR.h. <br />
Gera arquivos .asm que correspondem aos .asm fornecidos na linha de comando
mas sem espaços vazios e comentários. 
Também realiza outros ajustes estéticos úteis no código,
em preparação para o montador. 

- [x] O Montador, no MONTADOR.h. <br />
Gera os arquivos objeto (.obj) de cada .asm gerado pelo pré-processamento,
assim como detecta erros léxicos, sintáticos e semânticos nesses .asm.
Na detecção de erro, os .obj ficam vazios, i. e., sem números.

- [x] O Ligador, no LIGADOR.h. <br />
Recebe os códigos objetos criados no montador, e deles gera o arquivo executável
.exc, que consiste em um arquivo texto de uma linha com números decimais separados por espaço.

Os headers acima são usados na main.cpp, e todos são arquivos na pasta src.
Para rodar o trabalho, coloque o executável main no diretório com os .asm de interesse e entre com 
```
./main programa1.asm programa2.asm
``` 
na linha de comando. 
O resultado será o arquivo .exc que será criado na mesma pasta.
Em src, existe como exemplo o executável dos módulos MOD1.asm e MOD2.asm,
que juntos recebem 3 inteiros positivos e imprimem na tela o menor deles e em seguida o maior
(escrever, montar e ligar esses programas foi a questão 3 da prova 1 deste semestre).  

### Documentação do pré-processador

### Documentação do montador

### Documentação do ligador

---
## Trabalho 2
