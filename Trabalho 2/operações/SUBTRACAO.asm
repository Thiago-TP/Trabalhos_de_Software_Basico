; seção de variáveis
section .bss

; seção de dados
section .data 

; seção de códigos
extern  subtracao
section .text
    subtracao:
        enter   0, 0
        
        leave
        ret