extern  precision
extern type_N1, type_N1_size, type_N2, type_N2_size, result_msg, result_msg_size
extern  divisao, getInt16, getInt32, putInt, putString

SECTION .text
divisao:
    enter   0, 0
    
    cmp DWORD [precision], 0
    je  divisao16 
    jmp divisao32

divisao32:
    sub eax, eax
    push eax        ; flag de N1 negativo
    push eax        ; flag de N2 negativo

    push type_N1_size
    push type_N1
    call putString
    pop eax 
    pop eax

    call getInt32     
    push eax            ; N1 empilhado
    cmp eax, 0
    jl  flagN1_32       ; N1<0 ? aciona flag, faz N1 = -N1

    get_N2_32_div:
    push type_N2_size
    push type_N2
    call putString
    pop eax 
    pop eax

    call getInt32 
    push eax            ; N2 empilhado
    cmp eax, 0
    jl  flagN2_32       ; N2<0 ? aciona flag, faz N2 = -N2

    ; divisão é feita com os valores absolutos de N1 e N2
    make_div32:
    sub edx, edx
    mov DWORD eax, [ebp-12]
    idiv DWORD [ebp-16]

    ; eax <- resultado de interesse (poderia ser edx, o resto)
    mov eax, eax

    ; para impressão, o quociente é imbuído do sinal apropriado
    mov DWORD esi, [ebp-4]
    xor DWORD esi, [ebp-8]
    cmp esi, 0
    je put_div32

    neg eax

    put_div32:
        push eax 

        push result_msg_size
        push result_msg
        call putString
        pop eax 
        pop eax
        
        call putInt
    
    leave
    ret 20

flagN1_32:
    mov DWORD [ebp-4], 1
    neg DWORD [ebp-12]
    jmp get_N2_32_div
flagN2_32:
    mov DWORD [ebp-8], 1
    neg DWORD [ebp-16]
    jmp make_div32
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

divisao16:
    sub ax, ax
    push ax        ; flag de N1 negativo
    push ax        ; flag de N2 negativo

    push type_N1_size
    push type_N1
    call putString
    pop eax 
    pop eax

    call getInt16     
    push ax            ; N1 empilhado
    cmp ax, 0
    jl  flagN1_16       ; N1<0 ? aciona flag, faz N1 = -N1

    get_N2_16_div:
    push type_N2_size
    push type_N2
    call putString
    pop eax 
    pop eax

    call getInt16 
    push ax            ; N2 empilhado
    cmp ax, 0
    jl  flagN2_16       ; N2<0 ? aciona flag, faz N2 = -N2

    ; divisão é feita com os valores absolutos de N1 e N2
    make_div16:
    sub dx, dx
    mov WORD ax, [ebp-6]
    idiv WORD [ebp-8]

    ; ax <- resultado de interesse (poderia ser dx, o resto)
    mov ax, ax

    ; para impressão, o quociente é imbuído do sinal apropriado
    mov WORD si, [ebp-2]
    xor WORD si, [ebp-4]
    cmp si, 0
    je put_div16

    neg ax

    put_div16:
        movsx eax, ax
        push eax 

        push result_msg_size
        push result_msg
        call putString
        pop eax 
        pop eax

        call putInt
    
    leave
    ret 12

flagN1_16:
    mov WORD [ebp-2], 1
    neg WORD [ebp-6]
    jmp get_N2_16_div
flagN2_16:
    mov WORD [ebp-4], 1
    neg WORD [ebp-8]
    jmp make_div16
