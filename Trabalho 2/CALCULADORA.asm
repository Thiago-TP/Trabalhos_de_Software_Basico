; seção de variáveis
section .bss

; seção de dados
section .data   ; apenas strings podem ser globais
    digite_seu_nome db   "Bem vindo. Digite seu nome: ", 10  ; 10 é o end line
    pede_nome_size      equ $-digite_seu_nome

; seção de códigos
section .text
    global _start
    _start:
        push    ebp
        mov     ebp, esp
        sub     esp, 0           
        
        ; mensagem que pede o nome
        call     pede_nome

        ; mensagem de boas vindas
        ; mensagem de precisão
        ; menu de opções de operação
        ; executa a operação escolhida
        ; repete o processo se não ocorreu overflow/usuário escolheu sair

        add     esp, 0 
        pop     ebp 

        mov eax, 1  ; return 0    
        mov ebx, 0
        int 80h     

; imprime a string de tamanho edx apontada por ecx.
; edx e ecx devem ter sido colocados na pilha
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

; imprime mensagem pedindo o nome
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

