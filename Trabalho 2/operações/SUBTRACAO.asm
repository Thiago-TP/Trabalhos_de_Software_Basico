; seção de variáveis
section .bss

; seção de dados
section .data 

; seção de códigos
extern  subtracao
section .text
    subtracao:
        enter   0, 0
        
        push eax
        enter   0, 0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
    ; Condições de subtracao

        ; Se N2 menor que zero pula N2_neg_sub
        cmp [ebp+8],0
        jl N2_neg_sub

        ; Se N1 menor que zero dado que N2 é positivo pula N2_pos_N1_neg_sub
        cmp [ebp+12],0
        jmp N2_pos_N1_neg_sub

        ; Se ambos forem positivos
        jmp Dois_pos_sub

        N2_neg_sub:
            ; Se N1 menor que zero dado que N2 é negativo pula Dois_neg_sub
            cmp [ebp+12],0
            jl Dois_neg_sub

            ; Se N1 positivo dado que N2 é negativo pula N1_pos_N2_neg_sub
            jmp N1_pos_N2_neg_sub
        
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
    ; Subtracoes possíveis

        ; Subtracao sem sinal 
        Dois_pos_sub:                ; N1 - N2
            mov eax, [ebp+12]        ; eax = N1
            sub eax, [ebp+8]         ; eax = eax - N2 <- N1 - N2
            jmp fim_subtracao

        ; Subtracao com sinal 
        N1_pos_N2_neg_sub:          ; N1 - (-N2) = N1 + N2
            neg [ebp+8]             ; N2 = -1*-N2, torna N2 positivo pra somar posteriormente
            mov eax, [ebp+12]       ; eax = N1
            add eax, [ebp+8]        ; eax = eax + N2 <- N1 + N2
            jmp fim_subtracao

        N2_pos_N1_neg_sub:          ; -N1 - N2 = -(N1 + N2)
            neg [ebp+12]            ; N1 = -1*-N1, torna N1 positivo pra somar posteriormente
            mov eax, [ebp+8]        ; eax = N2
            add eax, [ebp+12]       ; eax = eax + N1 <- N2 + N1
            neg eax                 ; -eax = -(N1 + N2)
            jmp fim_subtracao

        Dois_neg_sub:               ; -N1 - (-N2) = N2 - N1 
            neg [ebp+8]             ; N2 = -N2, torna N2 positivo pra subtrair posteriormente
            neg [ebp+12]            ; N1 = -N1, torna N1 positivo pra subtrair posteriormente
            mov eax, [ebp+8]        ; eax = N2
            sub eax, [ebp+12]       ; eax = eax - N1 <- N2 - N1
            jmp fim_subtracao

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        fim_subtracao:
            leave
            pop eax
            ret
