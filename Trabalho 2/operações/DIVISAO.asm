extern precision
extern type_N1, type_N1_size, type_N2, type_N2_size, result_msg, result_msg_size
extern divisao, getInt16, getInt32, putInt, putString

SECTION .text
divisao:
    enter 0, 0
    
    ;DECISÃO DE PRECISÃO
    cmp  DWORD [precision], 1
    jne  divisao16 

    ;DIVISÃO32
    push type_N1_size
    push type_N1
    call putString
    pop  eax 
    pop  eax

    call getInt32     
    push eax            ; N1 empilhado

    push type_N2_size
    push type_N2
    call putString
    pop  eax 
    pop  eax

    call getInt32     
    push eax            ; N2 empilhado

    mov  eax, DWORD [ebp-4] 
    cdq
    idiv DWORD [ebp-8]

    push eax 

    push result_msg_size
    push result_msg
    call putString
    pop  eax 
    pop  eax

    call putInt
    
    leave
    ret 12
    

divisao16:
    push type_N1_size
    push type_N1
    call putString
    pop  eax 
    pop  eax

    call getInt16     
    push ax            ; N1 empilhado
    
    push type_N2_size
    push type_N2
    call putString
    pop  eax 
    pop  eax

    call getInt16 
    push ax            ; N2 empilhado

    mov  WORD ax, [ebp-2]
    cwde 
    cdq
    idiv WORD [ebp-4]

    cwde
    push eax 

    push result_msg_size
    push result_msg
    call putString
    pop  eax 
    pop  eax

    call putInt
    
    leave
    ret 8