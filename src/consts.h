#ifndef _CONSTH_
#define _CONSTH_

#define SUCCESS 0
#define INVALIDCHAR 1
#define invalidChar "Invalid character"
#define UNIDENTIFIEDLEX 2
#define unidentifiedLex "Unidetified lexem"
#define UNEXPECTEDEOF 3
#define unexpectedEof "End of file unexpected"
#define UNEXPECTEDTOKEN 4
#define unexpectedToken "Token unexpected"
#define INCOMPATIBLETYPES 5
#define incompatibleTypes "Incompatible types"
#define ALREADYDECLARED 6
#define alreadyDeclared "Identifier already declared"
#define UNDECLAREDIDENTIFIER 7
#define undeclaredIdentifier "Identifier undeclared"
#define INCOMPATIBLECLASS 8
#define incompatibleClass "Class incompatible, you are probably trying to change a constant value "

#define NOTFOUND -1
#define TOKEN 1

#define MAX 256
#define LASTCHAR 122
#define ENOF -1
#define HASHSIZE (MAX * LASTCHAR)
#define STACKBASE 0x0

#define VAR "classe-var"
#define CONST "classe-const"

#endif