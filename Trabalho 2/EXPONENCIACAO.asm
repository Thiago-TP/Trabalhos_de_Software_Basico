extern precision
extern type_N1, type_N1_size, type_N2, type_N2_size, result_msg, result_msg_size
extern exponenciacao, getInt16, getInt32, putInt, putString
extern OF_warning, OF_warning_size, putString, end_program

SECTION .text
exponenciacao:
    enter 0, 0

    push type_N1_size
    push type_N1
    call putString
    pop eax 
    pop eax

    call getInt32     
    push eax                ; N1 empilhado

    push type_N2_size
    push type_N2
    call putString
    pop eax 
    pop eax

    call getInt32     
    push eax                ; N2 empilhado

    sub eax, eax
    sub ecx, ecx

    mov eax, DWORD [ebp-4]  ; eax = N1 
    mov ecx, DWORD [ebp-8]  ; ecx = N2

    mov ebx, eax            ; eax = N1

    ;Casos expeciais
    cmp ecx, 0
    je exp_zero
    cmp ecx, 1
    je fim_exp32

    ;DECISÃO DE PRECISÃO
    cmp DWORD [precision],1
    jne exp16

    ;EXPONENCIAÇÃO32
    exp32:
        imul ebx               ; edx.eax = eax*ebx == N1*N1
        jo Exp_Overflow        ;Analisa se edx foi somente extensão de sinal ou overflow
        dec ecx
        cmp ecx, 1
        jg exp32

    fim_exp32:
        push eax

        push result_msg_size
        push result_msg
        call putString
        pop eax 
        pop eax

        call putInt
        leave
        ret 12
    
    exp16:
        imul bx                ;dx.ax = ax*bx == N1*N1
        jo Exp_Overflow        ;Analisa se edx foi somente extensão de sinal ou overflow
        dec cx
        cmp cx, 1
        jg exp16

    fim_exp16:
        cwde
        push eax

        push result_msg_size
        push result_msg
        call putString
        pop eax 
        pop eax

        call putInt
        leave
        ret 12

Exp_Overflow:
    push OF_warning_size
    push OF_warning

    call putString

    mov eax, 1
    mov ebx, 0
    int 0x80

exp_zero:
        mov eax, 1
        jmp fim_exp32
