extern  precision
extern  multiplicacao, getInt16, getInt32, putInt
extern  OF_warning, OF_warning_size, putString, end_program

%include "io.mac"

SECTION .text
multiplicacao:
    enter   0, 0

    ;DECISÃO DE PRECISÃO
    cmp DWORD [precision],1
    jne multiplicacao16

    ;MULTIPLICAÇÃO32
    call getInt32     
    push eax                ; N1 empilhado        
    call getInt32     
    push eax                ; N2 empilhado

    mov eax, DWORD [ebp-4]  ; edx = N1 
    mov edx, DWORD [ebp-8]  ; eax = N2
    imul edx                ; edx.eax = eax*edx == N1*N2

    ;Analisa se edx foi somente extensão de sinal ou overflow
    cmp edx,-1
    je fim_mul32
    cmp edx,0
    je fim_mul32

    call Mul_Overflow

    fim_mul32:
        PutLInt eax
        leave
        ret 4


    multiplicacao16:
        call getInt16     
        push eax                ; N1 empilhado        
        call getInt16     
        push eax                ; N2 empilhado

        sub eax,eax             ; zera eax  
        sub edx,edx             ; zera edx

        mov ax, WORD [ebp-4]    ; edx = N1 
        mov dx, WORD [ebp-8]    ; eax = N2
        imul dx                 ; dx.ax = ax*dx == N1*N2  

        ;Analisa se dx foi somente extensão de sinal ou overflow
        cmp dx,-1
        je fim_mul16
        cmp dx,0
        je fim_mul16

        call Mul_Overflow

        fim_mul16:
        PutLInt eax
        leave
        ret 4
    

    Mul_Overflow:
        enter 0,0

        push OF_warning_size
        push OF_warning

        call putString

        ; essas linhas davam erro
        ; pop [OF_warning]
        ; pop [OF_warning_size]

        leave
        call end_program 

