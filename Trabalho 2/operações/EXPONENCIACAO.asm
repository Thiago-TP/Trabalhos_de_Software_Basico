extern  precision
extern type_N1, type_N1_size, type_N2, type_N2_size, result_msg, result_msg_size
extern  exponenciacao, getInt16, getInt32, putInt, putString
extern  OF_warning, OF_warning_size, putString, end_program

%include "io.mac"

SECTION .text
    exponenciacao:
        enter   0, 0
        
        leave
        ret