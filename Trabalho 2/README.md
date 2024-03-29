## Trabalho 2: Calculadora em IA-32
Etapas para gerar o executável do zero:

1.  Com o nasm instalado numa máquina Linux,
    abra o terminal nesta pasta.
    
2. Crie os arquivos objeto de cada módulo através dos comandos (copie e cole o conjunto na linha de comando)
	```
    nasm -f elf CALCULADORA.asm   -o CALCULADORA.o 
    nasm -f elf SOMA.asm          -o SOMA.o
    nasm -f elf SUBTRACAO.asm     -o SUBTRACAO.o
    nasm -f elf MULTIPLICACAO.asm -o MULTIPLICACAO.o
    nasm -f elf DIVISAO.asm       -o DIVISAO.o
    nasm -f elf EXPONENCIACAO.asm -o EXPONENCIACAO.o
    nasm -f elf MOD.asm           -o MOD.o
	```  
	Os argumentos de todas as operações vêm pela pilha, cortesia da `CALCULADORA.asm`.
	
3.  Ligue o arquivo objeto principal através do comando (copie e cole na linha de comando)
	```
    ld -m elf_i386 -o CALCULADORA CALCULADORA.o \
    SOMA.o                                      \
    SUBTRACAO.o                                 \
    MULTIPLICACAO.o                             \
    DIVISAO.o                                   \
    EXPONENCIACAO.o                             \
    MOD.o	
	```
	Se tudo der certo, será criado arquivo executável chamado `CALCULADORA`.
	
4.  Execute o executável gerado através do comando (copie e cole na linha de comando)
	```
	./CALCULADORA
	```
	Isso deve iniciar a interação com usuário da forma esperada.  

Alternativamente, o trabalho pode ser montado, ligado e executado copiando e colando o bloco único abaixo,
que é a junção dos comandos listados acima.
```
nasm -f elf CALCULADORA.asm   -o CALCULADORA.o 
nasm -f elf SOMA.asm          -o SOMA.o
nasm -f elf SUBTRACAO.asm     -o SUBTRACAO.o
nasm -f elf MULTIPLICACAO.asm -o MULTIPLICACAO.o
nasm -f elf DIVISAO.asm       -o DIVISAO.o
nasm -f elf EXPONENCIACAO.asm -o EXPONENCIACAO.o
nasm -f elf MOD.asm           -o MOD.o
ld -m elf_i386 -o CALCULADORA CALCULADORA.o \
SOMA.o                                      \
SUBTRACAO.o                                 \
MULTIPLICACAO.o                             \
DIVISAO.o                                   \
EXPONENCIACAO.o                             \
MOD.o							
./CALCULADORA
```
