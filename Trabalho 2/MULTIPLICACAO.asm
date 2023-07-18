extern precision
extern type_N1, type_N1_size, type_N2, type_N2_size, result_msg, result_msg_size
extern multiplicacao, getInt16, getInt32, putInt, putString
extern OF_warning, OF_warning_size

SECTION .text
multiplicacao:
    enter 0, 0

    ;DECISÃO DE PRECISÃO
    cmp  DWORD [precision], 1
    jne  multiplicacao16

    ;MULTIPLICAÇÃO32
    push type_N1_size
    push type_N1
    call putString
    pop  eax 
    pop  eax

    call getInt32     
    push eax                    ; N1 empilhado

    push type_N2_size
    push type_N2
    call putString
    pop  eax 
    pop  eax

    call getInt32     
    push eax                    ; N2 empilhado

    mov  eax, DWORD [ebp-4]     ; edx = N1 
    mov  edx, DWORD [ebp-8]     ; eax = N2
    imul edx                    ; edx.eax = eax*edx = N1*N2

    ;Analisa se edx foi somente extensão de sinal ou overflow
    jo Mul_Overflow

    push eax

    push result_msg_size
    push result_msg
    call putString
    pop  eax 
    pop  eax

    call putInt
    leave
    ret 12


multiplicacao16:
    push type_N1_size
    push type_N1
    call putString
    pop  eax 
    pop  eax

    call getInt16     
    push eax                    ; N1 empilhado 

    push type_N2_size
    push type_N2
    call putString
    pop  eax 
    pop  eax

    call getInt16     
    push eax                    ; N2 empilhado

    sub  eax,eax                ; zera eax  
    sub  edx,edx                ; zera edx

    mov  ax, WORD [ebp-4]       ; edx = N1 
    mov  dx, WORD [ebp-8]       ; eax = N2
    imul dx                     ; dx.ax = ax*dx == N1*N2  

    ;Analisa se dx foi somente extensão de sinal ou overflow
    jo  Mul_Overflow

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
    

Mul_Overflow:
    push OF_warning_size
    push OF_warning

    call putString

    mov  eax, 1
    mov  ebx, 0
    int  0x80
