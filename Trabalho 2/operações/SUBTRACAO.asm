extern  precision
extern  subtracao, getInt16, getInt32, putInt

%include "io.mac"

SECTION .text
subtracao:
    enter   0, 0

    ;DECISÃO DE PRECISÃO
    cmp DWORD [precision],1
    jne subtracao16

    ;SUBTRAÇÃO32
    call getInt32     
    push eax                ; N1 empilhado        
    call getInt32     
    push eax                ; N2 empilhado

    mov eax, DWORD [ebp-4]  ; eax = N1 
    sub eax, DWORD [ebp-8]  ; eax = eax - N2 == N1 - N2

    PutLInt eax

    leave
    ret 4


    subtracao16:
        call getInt16     
        push eax                ; N1 empilhado        
        call getInt16     
        push eax                ; N2 empilhado

        mov ax, WORD [ebp-4]  ; eax = N1 
        sub ax, WORD [ebp-8]  ; eax = eax - N2 == N1 - N2        

        leave
        ret 4
