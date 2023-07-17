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

    menu        db  10, 10
                db  "ESCOLHA UMA OPÇÃO:",   10 
                db  "- 1: SOMA",            10
                db  "- 2: SUBTRACAO",       10
                db  "- 3: MULTIPLICACAO",   10
                db  "- 4: DIVISAO",         10
                db  "- 5: EXPONENCIACAO",   10
                db  "- 6: MOD",             10
                db  "- 7: SAIR",            10
    menu_size   equ $-menu

    show_32         db "(Precisão: 32)", 10 
    show_32_size    equ $-show_32
    show_16         db "(Precisão: 16)", 10 
    show_16_size    equ $-show_16

    type_N1         db "Digite N1: ", 0
    type_N1_size    equ $-type_N1
    type_N2         db "Digite N2: ", 0
    type_N2_size    equ $-type_N2
    result_msg      db "N1 op N2 = ", 0
    result_msg_size equ $-result_msg

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
global  OF_warning, OF_warning_size, type_N1, type_N1_size, type_N2, type_N2_size, result_msg, result_msg_size
global  end_program ; end program não é função
extern  putString, getInt16, getInt32, putInt, putString
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

    cmp DWORD [precision], 0 
    je  is16bit

    push show_32_size
    push show_32
    jmp end_getOperation

    is16bit:
    push show_16_size
    push show_16

    end_getOperation:
    call putString 

    call getInt32
    mov  [operation], eax

    leave
    ret 16

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

;   Recebe um inteiro da pilha, converte-o para string,
;   e o imprime na tela. A conversão é feita dígito a dígito,
;   salvando na pilha os bytes dos caracteres resultantes.
;   A impressão é feita iterando sobre a pilha.
putInt:
    enter 12, 0 ; são impressos no máximo 12 bytes (10 dígitos + sinal + \0)

    ; preparação do primeiro loop
    mov DWORD esi, [ebp+8]  ; esi = inteiro a ser convertido = num
    sub edi, edi            ; edi = número de dígitos
    
    mov ecx, ebp 
    sub ecx, 4              ; ecx = ebp-4 = endereço do primeiro byte 
    mov BYTE [ecx], 0

    ; se o número é negativo, toma-se o módulo
    cmp esi, 0 
    jge while_digit 

    neg esi
    
    ; loop para colocar os bytes na pilha
    while_digit:
        ; prepara endereço pro próximo char
        dec ecx 

        ; atualiza tamanho da string
        inc edi

        ; dígito = num % 10
        sub edx, edx    ; edx=0 para divisão
        mov eax, esi 
        mov ebx, 10 
        idiv ebx        ; num % 10 em edx (dl)

        ; (char) dígito = dígito + '0'
        add edx, '0'

        ; guarda char na pilha
        mov [ecx], dl

        ; num = num//10
        mov esi, eax    ; eax = quociente do idiv

        ; num//10 = 0 ? não tem mais número
        cmp esi, 0 
        jne while_digit

    ; imprime sinal se necessário
    mov ebx, 1  ; std file descriptor
    mov edx, 1  ; será impresso apenas um byte

    cmp DWORD [ebp+8], 0
    jge for_putDigit 

    inc edi     ; string ganhou um caracter, o sinal
    dec ecx 
    mov BYTE [ecx], '-'

    ; imprime string de final em ecx e início em ebp-4
    for_putDigit:
        cmp edi, 0
        je end_for_putDigit
        mov eax, 4
        int 0x80

        inc ecx 
        dec edi
        jmp for_putDigit

    end_for_putDigit:

    leave 
    ret

; "A de ler números deve ter duas versões: 16 e 32 bits."
getInt32:
    enter 1, 0

    sub esi, esi        ; esi = inteiro convertido
    sub edi, edi        ; edi = flag de negativo
    mov ecx, ebp 
    sub ecx, 1          ; ecx = endereço do espaço reservado para o número
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
    enter 1, 0

    sub si, si        ; esi = inteiro convertido
    sub di, di        ; edi = flag de negativo
    mov ecx, ebp 
    sub ecx, 1          ; ecx = endereço do espaço reservado para o número
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
        mov bx, si 
        shl bx, 1      ; bx = 2*acc
        shl si, 3      ; si = 8*acc
        add si, bx    ; si = 10*acc

        add si, ax    ; si = 10*acc + (int) dígito
        jmp while_getChar16

        isNegative16:
        mov di, 1
        jmp while_getChar16

    end_while_getChar16:
    cmp di, 0
    je isPositive16
    neg si    
    isPositive16:
    mov ax, si
    movsx eax, ax

    leave 
    ret
