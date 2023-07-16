extern  precision
extern  divisao, getInt16, getInt32, putInt

%include "io.mac"

SECTION .text
divisao:
    enter   0, 0

    sub eax, eax
    push eax        ; flag de N1 negativo
    push eax        ; flag de N2 negativo

    call getInt32     
    push eax            ; N1 empilhado
    cmp eax, 0
    jl  flagN1_32       ; N1<0 ? aciona flag, faz N1 = -N1

    get_N2:
    call getInt32 
    push eax            ; N2 empilhado
    cmp eax, 0
    jl  flagN2_32       ; N2<0 ? aciona flag, faz N2 = -N2

    ; divisão é feita com os valores absolutos de N1 e N2
    make_div32:
    sub edx, edx
    mov DWORD eax, [ebp-12]
    idiv DWORD [ebp-16]

    ; eax <- resultado de interesse
    mov eax, eax

    ; para impressão, o quociente é imbuído do sinal apropriado
    mov DWORD esi, [ebp-4]
    mov DWORD edi, [ebp-8]
    xor esi, edi
    cmp esi, 0
    je put_div32

    neg eax

    put_div32:
        push eax 
        call putInt
    
    leave
    ret 24

flagN1_32:
    mov DWORD [ebp-4], 1
    neg DWORD [ebp-12]
    jmp get_N2
flagN2_32:
    mov DWORD [ebp-8], 1
    neg DWORD [ebp-16]
    jmp make_div32
