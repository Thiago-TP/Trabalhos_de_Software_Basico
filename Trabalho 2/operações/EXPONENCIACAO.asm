; seção de variáveis
section .bss

; seção de dados
section .data 

; seção de códigos
extern  exponenciacao
section .text
    exponenciacao:
        enter   0, 0
        
        leave
        ret