extern  precision
extern  divisao, getInt16, getInt32, putInt

%include "io.mac"

SECTION .text
divisao:
    enter   0, 0

    cmp DWORD [precision], 0
    je  divisao16

    ; TODO: terminar divisão (tratar sinal)

    call getInt32     
    push eax            ; N1 empilhado
    call getInt32 
    push eax            ; N2 empilhado

    sub edx, edx
    mov DWORD eax, [ebp - 4]
    idiv DWORD [ebp - 8]

    PutLInt eax
    nwln
    PutLInt edx
    nwln

    ; call putInt ; imprime resultado
    
    leave
    ret 8

divisao16:
    
    leave
    ret 8