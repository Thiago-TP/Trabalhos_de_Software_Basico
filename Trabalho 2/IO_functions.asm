; lê ou imprime a string de tamanho edx apontada por ecx.
; eax, ebx, ecx, edx devem ter sido colocados na pilha em ordem reversa.
handle_text:
    enter   0, 0

    mov     eax, [ebp + 8]
    mov     ebx, [ebp + 12]
    mov     ecx, [ebp + 16]
    mov     edx, [ebp + 20]

    int     80h         ; chamada ao sistema

    leave        
    ret     

ask_name:
    enter   16, 0                           ; 16 = 4*4 bytes

    mov     dword [ebp - 4], pede_nome_size ; tamanho da string em bytes
    mov     dword [ebp - 8], pede_nome      ; endereço da string 
    mov     dword [ebp - 12], 1             ; valor 1 -> stdout 
    mov     dword [ebp - 16], 4             ; valor 4 -> print syscall
    
    call    handle_text
    
    leave
    ret 

read_name: 
    enter   16, 0                           ; 16 = 4*4 bytes

    mov     dword [ebp - 4], user_name_size ; tamanho da string em bytes
    mov     dword [ebp - 8], user_name      ; endereço da string 
    mov     dword [ebp - 12], 0             ; valor 0 em ebx -> stdin 
    mov     dword [ebp - 16], 3             ; valor 3 em eax -> scanf syscall
    
    call    handle_text
    
    leave
    ret 

welcome:
    enter   16, 0                           ; 16 = 4*4 bytes

    mov     dword [ebp - 4], hola_size      ; tamanho da string em bytes
    mov     dword [ebp - 8], hola           ; endereço da string 
    mov     dword [ebp - 12], 1             ; valor 1 -> stdout 
    mov     dword [ebp - 16], 4             ; valor 4 -> print syscall
    call    handle_text

    mov     dword [ebp - 4], user_name_size ; tamanho da string em bytes
    mov     dword [ebp - 8], user_name      ; endereço da string
    call    handle_text

    mov     dword [ebp - 4], bem_vindo_size ; tamanho da string em bytes
    mov     dword [ebp - 8], bem_vindo      ; endereço da string
    call    handle_text
    
    leave
    ret

ask_precision:
    enter   16, 0                               ; 16 = 4*4 bytes

    mov     dword [ebp - 4], qual_precision_size ; tamanho da string em bytes
    mov     dword [ebp - 8], qual_precision      ; endereço da string 
    mov     dword [ebp - 12], 1                 ; valor 1 -> stdout 
    mov     dword [ebp - 16], 4                 ; valor 4 -> print syscall 
    
    call    handle_text
    
    leave
    ret

read_precision:
    enter   16, 0                           ; 16 = 4*4 bytes

    mov     dword [ebp - 4], is_32bit_size  ; tamanho da string em bytes
    mov     dword [ebp - 8], is_32bit       ; endereço da string 
    mov     dword [ebp - 12], 0             ; valor 0 em ebx -> stdin 
    mov     dword [ebp - 16], 3             ; valor 3 em eax -> scanf syscall
    
    call    handle_text
    
    leave
    ret 

show_menu:
    enter   16, 0                       ; 16 = 4*4 bytes

    mov     dword [ebp - 4], menu_size  ; tamanho da string em bytes
    mov     dword [ebp - 8], menu       ; endereço da string 
    mov     dword [ebp - 12], 1         ; valor 1 -> stdout 
    mov     dword [ebp - 16], 4         ; valor 4 -> print syscall
    
    call    handle_text
    
    leave
    ret

read_op:
    enter   16, 0                               ; 16 = 4*4 bytes

    mov     dword [ebp - 4], user_choice_size   ; tamanho da string em bytes
    mov     dword [ebp - 8], user_choice        ; endereço da string 
    mov     dword [ebp - 12], 0                 ; valor 0 em ebx -> stdin 
    mov     dword [ebp - 16], 3                 ; valor 3 em eax -> scanf syscall
    
    call    handle_text
    
    leave
    ret  
