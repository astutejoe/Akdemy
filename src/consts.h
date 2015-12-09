#ifndef _CONSTH_
#define _CONSTH_

#define SUCCESS 0
#define INVALIDCHAR 1
#define UNIDENTIFIEDLEX 2
#define UNSPECTEDEOF 3
#define UNSPECTEDTOKEN 4
#define INCOMPATIBLETYPES 5
#define ALREADYDECLARED 6
#define UNDECLAREDIDENTIFIER 7

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