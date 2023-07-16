extern  precision
extern  soma, getInt16, getInt32, putInt

%include "io.mac"

SECTION .text
soma:
    enter   0, 0

    ;DECISÃO DE PRECISÃO
    cmp DWORD [precision],1
    jne soma16

    ;SOMA32
    call getInt32     
    push eax                ; N1 empilhado        
    call getInt32     
    push eax                ; N2 empilhado

    mov eax, DWORD [ebp-4]  ; eax = N1 
    add eax, DWORD [ebp-8]  ; eax = eax + N2 == N1 + N2

    PutLInt eax

    leave
    ret 4


    soma16:
        call getInt16     
        push eax                ; N1 empilhado        
        call getInt16     
        push eax                ; N2 empilhado

        mov ax, WORD [ebp-4]  ; eax = N1 
        add ax, WORD [ebp-8]  ; eax = eax + N2 == N1 + N2        

        leave
        ret 4
