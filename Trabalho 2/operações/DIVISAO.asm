; seção de variáveis
section .bss

; seção de dados
section .data 

; seção de códigos
extern  divisao
section .text
    divisao:
        enter   0, 0
        
        leave
        ret