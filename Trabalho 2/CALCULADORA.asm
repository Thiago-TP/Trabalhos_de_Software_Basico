; seção de variáveis
section .bss
    user_name           resb    20  ; nome do usuário pode ter até 20 letras ascii
    is_32bit            resb    1   ; byte flag de 32 bits
    user_choice         resb    1   ; opção de operação do usuário (byte entre 1 e 7)

; seção de dados
section .data   ; apenas strings podem ser globais

    digite_seu_nome     db   "Bem vindo. Digite seu nome: ", 10 ; 10 é o \n
    pede_nome_size      equ $-digite_seu_nome

    hola                db  "Hola, "
    hola_size           equ $-hola
    bem_vindo           db  ", bem-vindo ao programa de CALC IA-32", 10 
    bem_vindo_size      equ $-bem_vindo

    qual_precisao       db  "Vai trabalhar com 16 ou 32 bits? (digite 0 para 16, e 1 para 32):"
    qual_precisao_size  equ $-qual_precisao

    menu                db      10, 10, 
    db  "ESCOLHA UMA OPÇÃO:",   10, 
    db  "-1: SOMA",             10
    db  "-2: SUBTRACAO",        10
    db  "-3: MULTIPLICACAO",    10
    db  "-4: DIVISAO",          10
    db  "-5: EXPONENCIACAO",    10
    db  "-6: MOD",              10
    db  "-7: SAIR",             10, 10
    menu_size           equ $-menu  

    qual_o_N1           db  "Digite o primeiro numero: ", 10
    qual_o_N2           db  "Digite o segundo numero: ", 10
    qual_o_N1_size      equ $-qual_o_N1
    qual_o_N2_size      equ $-qual_o_N2

    ocorreu_OF          db  "OCORREU OVERFLOW", 10
    ocorreu_OF_size     equ $-ocorreu_OF

; seção de códigos
section .text
    global _start
    _start:         
        
        ; mensagem que pede o nome
        call    pede_nome

        ; lê o nome dado pelo usuário 
        ;call   le_nome

        ; mensagem de boas vindas
        call    boas_vindas

        ; mensagem de precisão
        call    pede_precisao

        ; lê a decisão de precisão dada pelo usuário 
        ;call    le_precisao

        repete_interacao:
        ; menu de opções de operação
        call    mostra_menu

        ; lê a opção do usuário 
        ;call    le_operacao

        ; executa a operação escolhida
        call    executa_escolha         ; no momento apenas fecha o programa

        ; repete o processo se não ocorreu overflow/usuário escolheu sair
        jmp      repete_interacao   


; imprime a string de tamanho edx apontada por ecx.
; eax a edx devem ter sido colocados na pilha em ordem reversa
imprime_texto:
    push    ebp
    mov     ebp, esp
    sub     esp, 0

    mov     eax, [ebp + 8]
    mov     ebx, [ebp + 12]
    mov     ecx, [ebp + 16]
    mov     edx, [ebp + 20]

    int     80h         ; chamada ao sistema

    add     esp, 0      ; fim da função
    pop     ebp         
    ret     

pede_nome:
    push    ebp
    mov     ebp, esp
    sub     esp, 16             ; 16 = 4*4 bytes

    mov     eax, 4               ; valor 4 em eax -> print syscall
    mov     ebx, 1               ; valor 1 em ebx -> stdout
    mov     ecx, digite_seu_nome ; endereço da string
    mov     edx, pede_nome_size  ; tamanho da string em bytes 

    mov     [ebp - 4], edx 
    mov     [ebp - 8], ecx 
    mov     [ebp - 12], ebx 
    mov     [ebp - 16], eax 
    
    call    imprime_texto
    
    add     esp, 16
    pop     ebp
    ret 

le_nome:
    ret    

boas_vindas:
    push    ebp
    mov     ebp, esp
    sub     esp, 16             ; 16 = 4*4 bytes

    mov     eax, 4              ; valor 4 em eax -> print syscall
    mov     ebx, 1              ; valor 1 em ebx -> stdout
    mov     ecx, hola           ; endereço da string
    mov     edx, hola_size      ; tamanho da string em bytes 

    mov     [ebp - 4], edx 
    mov     [ebp - 8], ecx 
    mov     [ebp - 12], ebx 
    mov     [ebp - 16], eax 
    
    call    imprime_texto
    
    add     esp, 16
    pop     ebp
    ret

pede_precisao:
    push    ebp
    mov     ebp, esp
    sub     esp, 16                 ; 16 = 4*4 bytes

    mov     eax, 4                  ; valor 4 em eax -> print syscall
    mov     ebx, 1                  ; valor 1 em ebx -> stdout
    mov     ecx, qual_precisao      ; endereço da string
    mov     edx, qual_precisao_size ; tamanho da string em bytes 

    mov     [ebp - 4], edx 
    mov     [ebp - 8], ecx 
    mov     [ebp - 12], ebx 
    mov     [ebp - 16], eax 
    
    call    imprime_texto
    
    add     esp, 16
    pop     ebp
    ret

mostra_menu:
    push    ebp
    mov     ebp, esp
    sub     esp, 16         ; 16 = 4*4 bytes

    mov     eax, 4          ; valor 4 em eax -> print syscall
    mov     ebx, 1          ; valor 1 em ebx -> stdout
    mov     ecx, menu       ; endereço da string
    mov     edx, menu_size  ; tamanho da string em bytes 

    mov     [ebp - 4], edx 
    mov     [ebp - 8], ecx 
    mov     [ebp - 12], ebx 
    mov     [ebp - 16], eax 
    
    call    imprime_texto
    
    add     esp, 16
    pop     ebp
    ret

executa_escolha:
    ; fim do programa
    mov eax, 1    
    mov ebx, 0
    int 80h 

coleta_operandos:
    ret
