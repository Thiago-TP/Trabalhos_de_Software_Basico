## Trabalho 2: Calculadora em IA-32
Etapas para gerar o executável do zero:

1.  Com o nasm instalado numa máquina Linux,
    abra o terminal nesta pasta.
    
2. Crie os arquivos objeto de cada módulo através dos comandos (copie e cole o conjunto na linha de comando)
	```
    nasm -f elf CALCULADORA.asm             -o CALCULADORA.o 
    nasm -f elf operações/SOMA.asm			-o operações/SOMA.o
    nasm -f elf operações/SUBTRACAO.asm		-o operações/SUBTRACAO.o
    nasm -f elf operações/MULTIPLICACAO.asm	-o operações/MULTIPLICACAO.o
    nasm -f elf operações/DIVISAO.asm		-o operações/DIVISAO.o
    nasm -f elf operações/EXPONENCIACAO.asm	-o operações/EXPONENCIACAO.o
    nasm -f elf operações/MOD.asm 			-o operações/MOD.o
	```  
	Os argumentos de todas as operações vêm pela pilha, cortesia da `CALCULADORA.asm`.
	
3.  Ligue o arquivo objeto principal através do comando (copie e cole na linha de comando)
	```
    ld -m elf_i386 -o CALCULADORA CALCULADORA.o \
    operações/SOMA.o                            \
    operações/SUBTRACAO.o                       \
    operações/MULTIPLICACAO.o                   \
    operações/DIVISAO.o                         \
    operações/EXPONENCIACAO.o                   \
    operações/MOD.o
	```
	Se tudo der certo, será criado arquivo executável chamado `CALCULADORA`.
	
4.  Execute o executável gerado através do comando (copie e cole na linha de comando)
	```
	./CALCULADORA
	```
	Isso deve iniciar a interação com usuário da forma esperada.  
