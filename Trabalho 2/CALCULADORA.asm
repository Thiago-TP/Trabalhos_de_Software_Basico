;...........................................;
;   Cleyton Erick Caldas Sá - 190026201     ;
;   Thiago Tomás de Paula   - 190038641     ;
;   2023/1 - julho de 2023                  ;
;...........................................;
;   CIC0104 - Software Básico               ;
;   Turma 1, prof. Bruno L. M. Espinoza     ;
;   Módulo principal do Trabalho 2          ;
;...........................................;
;   CALCULADORA EM ASSEMBLY NASM IA-32      ; 
;...........................................;

; Detalhes em https://github.com/Thiago-TP/Trabalhos_de_Software_Basico.git

SECTION .data
    type_name           db  "Bem vindo. Digite seu nome: ", 0
    type_name_size      equ $-type_name

    welcome_msg1        db  "Hola, ", 0
    welcome_msg1_size   equ $-welcome_msg1
    welcome_msg2        db  ", bem-vindo ao programa de CALC IA-32", 10 
    welcome_msg2_size   equ $-welcome_msg2

    type_precision      db  "Vai trabalhar com 16 ou 32 bits? (digite 0 para 16, e 1 para 32): ", 0
    type_precision_size equ $-type_precision

    menu        db  10
                db  "ESCOLHA UMA OPÇÃO:",   10 
                db  "- 1: SOMA",            10
                db  "- 2: SUBTRACAO",       10
                db  "- 3: MULTIPLICACAO",   10
                db  "- 4: DIVISAO",         10
                db  "- 5: EXPONENCIACAO",   10
                db  "- 6: MOD",             10
                db  "- 7: SAIR",            10
    menu_size   equ $-menu

    OF_warning      db  "OCORREU OVERFLOW", 10
    OF_warning_size db  $-OF_warning

    name_size   dd 0

SECTION .bss
    name            resb    1
    precision       resd    1
    operation       resd    1

%include "io.mac" ; SOMENTE PARA DEBUG, DELETAR NA VERSÃO FINAL

SECTION .text
global _start 
_start:
    call getName
    call getPrecision
    while_true:
        call getOperation
        call runOperation
    jmp while_true

; "A função principal e funções de entrada e saída de dados 
; devem estar no mesmo arquivo CALCULADORA.ASM.
; Porém as operações deve estar cada uma num arquivo separado
; (ex.: SOMA.ASM, DIVISAO.ASM, etc.) 
; o programa deve ser compilado e ligado para gerar um único executável. 
; Para isso deve estar no arquivo README as instruções de compilar e ligar.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
global  precision
global  OF_warning,OF_warning_size
global  end_program
extern  putString, getInt16, getInt32, putInt
extern  soma, subtracao, multiplicacao, divisao, exponenciacao, mod
 
getName:
    enter 0, 0

    push type_name_size
    push type_name
    call putString 
    
    push DWORD [name_size]
    push name
    call getString  
    mov [name_size], eax

    push welcome_msg1_size
    push welcome_msg1
    call putString   
    
    push DWORD [name_size]
    push name
    call putString

    push welcome_msg2_size
    push welcome_msg2
    call putString 
    
    leave
    ret 40

getPrecision:
    enter 0, 0

    push type_precision_size
    push type_precision
    call putString 

    push precision
    call getInt32
    mov  [precision], eax

    leave
    ret 12

; Para o menu a ideia é chamar a função de saída de string uma vez por linha.
getOperation:
    enter 0, 0

    push menu_size
    push menu
    call putString 

    ; push operation
    call getInt32
    mov  [operation], eax

    leave
    ret 12

; "Ao receber a opção do menu, que pode ser qualquer número entre 1 a 6, 
; deve ir para uma função que vai executar a operação requerida. 
; Tal função deve pedir 2 números inteiros (que pode ter o sinal negativo). 
; Deve-se assumir que são de 16 ou 32 bits de acordo com a opção digitada pelo usuário. 
; Devem ser armazenados em variáveis locais. Deve mostrar o resultado final. 
; Esperar o usuário digitar ENTER e novamente mostrar menu de opções. 
; Até o usuário digitar a opção de sair."
runOperation:
    enter 0, 0

    cmp DWORD [operation], 1
    je run_add
    cmp DWORD [operation], 2
    je run_sub
    cmp DWORD [operation], 3
    je run_mul
    cmp DWORD [operation], 4
    je run_div
    cmp DWORD [operation], 5
    je run_exp
    cmp DWORD [operation], 6
    je run_mod
    cmp DWORD [operation], 7
    je end_program

    run_add:    call soma
                jmp  end_run
    run_sub:    call subtracao
                jmp  end_run
    run_mul:    call multiplicacao
                jmp  end_run
    run_div:    call divisao
                jmp  end_run
    run_exp:    call exponenciacao
                jmp  end_run
    run_mod:    call mod
                jmp  end_run

    end_run:
    leave
    ret

    end_program:
    mov eax, 1 
    mov ebx, 0
    int 0x80

; "Todas as mensagens de TEXTO devem ser mostradas 
; usando uma ÚNICA função de saída de dados de string. 
; Esta função deve receber pela pilha o ponteiro da variável global que
; contém o string e a quantidade de bytes a serem escritos. 
; Não deve ter retorno."
putString:
    enter 0, 0

    sub esi, esi        
    mov ecx, [ebp + 8]
    print_for:
        cmp DWORD esi, [ebp + 12]
        jae end_print_for

        mov eax, 4
        mov ebx, 1
        mov edx, 1          
        int 0x80  

        inc esi
        inc ecx
        jmp print_for

    end_print_for:
    leave 
    ret 

; "A leitura do teclado deve ser feita por 2 funções: 
; uma para ler strings, e outra para ler números." 
getString:
    enter 0, 0
       
    mov ecx, [ebp + 8] 
    read_while:
        mov eax, 3
        mov ebx, 0
        mov edx, 1          
        int 0x80  

        sub eax, eax 
        mov al, [ecx]
        inc ecx

        cmp al, 10 
        jne read_while
        
    mov eax, ecx
    sub eax, [ebp + 8]
    dec eax
    
    leave 
    ret 

putInt:
    enter 20, 0

    mov ecx, ebp 
    sub ecx, 4          ; ecx = endereço do espaço reservado para o caracter

    sub esi, esi        ; índice i do dígito

    cmp DWORD [ebp + 8], 0
    jge while_putChar

    ; imprime sinal negativo se o inteiro for negativo
    mov eax, 4
    mov ebx, 1
    mov BYTE [ecx], '-'
    mov edx, 1
    int 0x80 

    ; toma o valor absoluto do número
    neg DWORD [ebp + 8]

    ; loop coloca caracter do i-ésimo dígito no endereço ecx + i
    while_putChar:
        ; eax = num//10, edx = num%10 
        sub edx, edx
        mov eax, DWORD [ebp + 8]
        mov ebx, 10
        idiv ebx             

        ; PutLInt edx
        ; nwln
        add dl, '0'        ; dl = (char) dígito i
        mov dh, 0
        mov [ebp-4 + esi], dl ; str[i] = (char) dígito i

        mov DWORD [ebp + 8], eax ; num = num//10
        cmp DWORD [ebp + 8], 0 ; sem mais dígito, para o programa
        je end_while_putChar

        mov [ebp-4 + esi - 1], dl ; str[i+1] = str[i] (para não imprimir ao contrário)    
        dec esi             ; ecx = endereço do próximo caracter   

        jmp while_putChar

    end_while_putChar:
    dec esi
    mov BYTE [ebp-4 + esi], 0   ; termina a string com 0

    ; imprime valor absoluto do número
    ; TODO: percorrer a pilha imprimindo os chars
    ; mov eax, 4
    ; mov ebx, 1
    ; mov ecx, ebp 
    ; sub ecx, 4
    ; neg esi 
    ; mov edx, esi
    ; int 0x80

    leave 
    ret 20

; "A de ler números deve ter duas versões: 16 e 32 bits."
getInt32:
    enter 0, 0

    sub esi, esi        ; esi = inteiro convertido
    sub edi, edi        ; edi = flag de negativo
    mov ecx, ebp 
    sub ecx, 4          ; ecx = endereço do espaço reservado para o número
    while_getChar32:
        mov eax, 3
        mov ebx, 0
        mov edx, 1          
        int 0x80  

        sub eax, eax 
        mov al, [ecx]
        cmp al, 10 
        je  end_while_getChar32

        cmp al, '-' 
        je  isNegative32

        sub al, '0'     ; al = dígito - '0' = (int) dígito
        mov ebx, esi 
        shl ebx, 1      ; ebx = 2*acc
        shl esi, 3      ; esi = 8*acc
        add esi, ebx    ; esi = 10*acc

        add esi, eax    ; esi = 10*acc + (int) dígito
        jmp while_getChar32

        isNegative32:
        mov edi, 1
        jmp while_getChar32

    end_while_getChar32:
    cmp edi, 0
    je isPositive32
    neg esi    
    isPositive32:
    mov eax, esi

    leave 
    ret 

; "A de ler números deve ter duas versões: 16 e 32 bits."
getInt16:
    enter 0, 0

    sub esi, esi        ; esi = inteiro convertido
    sub edi, edi        ; edi = flag de negativo
    mov ecx, ebp 
    sub ecx, 4          ; ecx = endereço do espaço reservado para o número
    while_getChar16:
        mov eax, 3
        mov ebx, 0
        mov edx, 1          
        int 0x80  

        sub eax, eax 
        mov al, [ecx]
        cmp al, 10 
        je  end_while_getChar16

        cmp al, '-' 
        je  isNegative16

        sub al, '0'     ; al = dígito - '0' = (int) dígito
        mov ebx, esi 
        shl ebx, 1      ; ebx = 2*acc
        shl esi, 3      ; esi = 8*acc
        add esi, ebx    ; esi = 10*acc

        add esi, eax    ; esi = 10*acc + (int) dígito
        jmp while_getChar16

        isNegative16:
        mov edi, 1
        jmp while_getChar16

    end_while_getChar16:
    cmp edi, 0
    je isPositive16
    neg esi    
    isPositive16:
    mov eax, esi

    leave 
    ret
