; seção de variáveis
section .bss

; seção de dados
section .data 

; seção de códigos
extern  soma
section .text
    soma:
        push eax
        enter   0, 0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
    ; Condições de soma

        ; Se N2 menor que zero pula N2_neg
        cmp DWORD [ebp+8],0
        jl N2_neg

        ; Se N1 menor que zero dado que N2 é positivo pula N2_pos_N1_neg
        cmp DWORD [ebp+12],0
        jmp N2_pos_N1_neg

        ; Se ambos forem positivos
        jmp Dois_pos

        N2_neg:
            ; Se N1 menor que zero dado que N2 é negativo pula Dois_neg
            cmp DWORD [ebp+12],0
            jl Dois_neg

            ; Se N1 positivo dado que N2 é negativo pula N1_pos_N2_neg:
            jmp N1_pos_N2_neg
        
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
    ; Somas possíveis

        ; Soma sem sinal
        Dois_pos:
            mov eax, [ebp+8]        ; eax = N2
            add eax, [ebp+12]       ; eax = eax + N1 <- N2 +N1
            jmp fim_soma

        ; Somas com sinal
        N1_pos_N2_neg:
            neg DWORD [ebp+8]             ; N2 = -N2, torna N2 positivo pra subtrair posteriormente
            mov eax, [ebp+12]       ; eax = N1
            sub eax, [ebp+8]        ; eax = eax - N2 <- N1 - N2
            jmp fim_soma

        N2_pos_N1_neg:
            neg DWORD [ebp+12]            ; N1 = -N1, torna N1 positivo pra subtrair posteriormente
            mov eax, [ebp+8]        ; eax = N2
            sub eax, [ebp+12]       ; eax = eax - N1 <- N2 - N1
            jmp fim_soma

        Dois_neg:
            neg DWORD [ebp+8]             ; N2 = -N2, torna N2 positivo pra somar posteriormente
            neg DWORD [ebp+12]            ; N1 = -N1, torna N1 positivo pra somar posteriormente
            mov eax, [ebp+8]        ; eax = N2
            add eax, [ebp+12]       ; eax = eax + N1 <- N2 +N1
            neg eax                 ; eax = -eax, sinal da resposta
            jmp fim_soma

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        fim_soma:
            leave
            pop eax
            ret
