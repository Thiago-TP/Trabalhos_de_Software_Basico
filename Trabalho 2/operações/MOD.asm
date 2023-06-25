; seção de variáveis
section .bss

; seção de dados
section .data 

; seção de códigos
extern  mod
section .text
    mod:
        enter   0, 0
        
        leave
        ret