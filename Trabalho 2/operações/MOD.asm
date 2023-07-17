extern  precision
extern type_N1, type_N1_size, type_N2, type_N2_size, result_msg, result_msg_size
extern  mod, getInt16, getInt32, putInt, putString

SECTION .text
mod:
    enter   0, 0
    
    cmp DWORD [precision], 0
    je  mod16 
    jmp mod32

mod32:
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
    jl  negN1_32       ; N1<0 ? aciona flag, faz N1 = -N1

    get_N2_32_mod:
    push type_N2_size
    push type_N2
    call putString
    pop eax 
    pop eax

    call getInt32 
    push eax            ; N2 empilhado
    cmp eax, 0
    jl  negN2_32       ; N2<0 ? aciona flag, faz N2 = -N2

    ; |N1| < |N2| ? print N1
    make_mod32:
    mov DWORD eax, [ebp-12]
    cmp DWORD eax, [ebp-16]
    jl  shortcut_mod32

    ; divisão é feita com os valores absolutos de N1 e N2
    sub edx, edx
    mov DWORD eax, [ebp-12]
    idiv DWORD [ebp-16]

    ; eax <- resultado de interesse 
    mov eax, edx

    ; para impressão, o quociente é imbuído do sinal apropriado
    mov DWORD esi, [ebp-4]
    xor DWORD esi, [ebp-8]
    cmp esi, 0
    je put_mod32

    neg eax

    put_mod32:
        push eax 

        push result_msg_size
        push result_msg
        call putString
        pop eax 
        pop eax

        call putInt
    
    leave
    ret 20

negN1_32:
    mov DWORD [ebp-4], 1
    neg DWORD [ebp-12]
    jmp get_N2_32_mod
negN2_32:
    mov DWORD [ebp-8], 1
    neg DWORD [ebp-16]
    jmp make_mod32
shortcut_mod32:
    cmp DWORD [ebp-4], 0
    je put_mod32
    neg eax 
    jmp put_mod32
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

mod16:
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
    jl  negN1_16       ; N1<0 ? aciona flag, faz N1 = -N1

    get_N2_16_mod:
    push type_N2_size
    push type_N2
    call putString
    pop eax 
    pop eax

    call getInt16 
    push ax            ; N2 empilhado
    cmp ax, 0
    jl  negN2_16       ; N2<0 ? aciona flag, faz N2 = -N2

    ; |N1| < |N2| ? print N1
    make_mod16:
    mov WORD ax, [ebp-6]
    cmp WORD ax, [ebp-8]
    jl  shortcut_mod16

    ; divisão é feita com os valores absolutos de N1 e N2
    sub dx, dx
    mov WORD ax, [ebp-6]
    idiv WORD [ebp-8]

    ; ax <- resultado de interesse 
    mov ax, dx

    ; para impressão, o quociente é imbuído do sinal apropriado
    mov WORD si, [ebp-2]
    xor WORD si, [ebp-4]
    cmp si, 0
    je put_mod16

    neg ax

    put_mod16:
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

negN1_16:
    mov WORD [ebp-2], 1
    neg WORD [ebp-6]
    jmp get_N2_16_mod
negN2_16:
    mov WORD [ebp-4], 1
    neg WORD [ebp-8]
    jmp make_mod16
shortcut_mod16:
    cmp WORD [ebp-2], 0
    je put_mod16
    neg eax 
    jmp put_mod16