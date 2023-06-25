; seção de variáveis
section .bss

; seção de dados
section .data 

; seção de códigos
extern  soma
section .text
    soma:
        enter   0, 0
        
        leave
        ret