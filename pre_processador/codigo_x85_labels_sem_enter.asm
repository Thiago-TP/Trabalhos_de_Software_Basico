BIN1: BEGIN
EXTERN: B2
EXTERN: OLD_DATA
PUBLIC L1
SECTION DATA
DOIS: CONST 0X20
NEW_DATA: SPACE
TMP_DATA: SPACE
SECTION TEXT
JMP BIN2
L1: DIV DOIS L1: 
STORE NEW_DATA
MU*L DOIS
STORE TMP_DATA
LOAD OLD_DATA
SUB TMP_DATA
STORE TMP_DATA
OUTPUT TMP_DATA
COPY NEW_DATA , OLD_DATA
LOAD OLD_DATA
JMPP L1
STOP
