; seção de variáveis
section .bss

; seção de dados
section .data 

; seção de códigos
extern  multiplicacao
section .text
    multiplicacao:
        enter   0, 0
        
        leave
        ret