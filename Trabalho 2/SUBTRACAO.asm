extern precision
extern type_N1, type_N1_size, type_N2, type_N2_size, result_msg, result_msg_size
extern subtracao, getInt16, getInt32, putInt, putString

SECTION .text
subtracao:
    enter 0, 0

    ;DECISÃO DE PRECISÃO
    cmp  DWORD [precision], 1
    jne  subtracao16

    ;SUBTRAÇÃO32
    push type_N1_size
    push type_N1
    call putString
    pop  eax 
    pop  eax

    call getInt32     
    push eax                ; N1 empilhado 

    push type_N2_size
    push type_N2
    call putString
    pop  eax 
    pop  eax

    call getInt32     
    push eax                ; N2 empilhado

    mov  eax, DWORD [ebp-4] ; eax = N1 
    sub  eax, DWORD [ebp-8] ; eax = eax - N2 == N1 - N2

    push eax

    push result_msg_size
    push result_msg
    call putString
    pop  eax 
    pop  eax

    call putInt 

    leave
    ret 12

subtracao16:
    push type_N1_size
    push type_N1
    call putString
    pop  eax 
    pop  eax

    call getInt16     
    push eax                ; N1 empilhado  

    push type_N2_size
    push type_N2
    call putString
    pop  eax 
    pop  eax

    call getInt16     
    push eax                ; N2 empilhado

    mov  ax, WORD [ebp-4]   ; eax = N1 
    sub  ax, WORD [ebp-8]   ; eax = eax - N2 = N1 - N2  

    cwde
    push eax

    push result_msg_size
    push result_msg
    call putString
    pop  eax 
    pop  eax

    call putInt      

    leave
    ret 12
