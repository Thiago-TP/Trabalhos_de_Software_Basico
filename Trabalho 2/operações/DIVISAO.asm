extern  precision
extern  divisao, getInt16, getInt32, putInt

%include "io.mac"

SECTION .text
divisao:
    enter   0, 0

    push esp
    call getInt32 
    PutLInt eax            ; N1 empilhado
    ; call getOperand 
    ; push eax            ; N2 empilhado

    ; idiv vai vir aqui

    ; call putInt ; imprime resultado
    
    leave
    ret 4