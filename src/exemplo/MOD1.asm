BIN1: BEGIN
EXTERN: B2
       EXTERN:     OLD_DATA
PuBLIC L1
SECTION DATA
DOIS: CONST      0x20
NEW_DATA: SPACE
TMP_DATA: SPACE ; fasdkfjçl^~
SECTION TEXT
JMP BIN2
L1: 
DIV DOIS  
STORE  NEW_DATA
MU*L            DOIS; 129837027&*&
STORE TMP_DATA
LOAD OLD_DATA


SUB TMP_DATA
STORE TMP_DATA
OUTPUT TMP_DATA
COPY NEW_DATA, OLD_DATA
LOAD OLD_DATA
JMPP L1
STOP
END