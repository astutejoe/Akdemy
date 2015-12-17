#ifndef _TABLEH_
#define _TABLEH_

typedef struct entry {
	char lexem[256];
	unsigned char token;
	unsigned char type;
	char* class;
	unsigned int address;
	struct entry* next;
} Entry;

Entry* addSymbol(char* lexem, unsigned char token);

Entry* searchLexem(char* lexem);

int printSymbolTable();
int initTable();

#define KEYWORDS 32

#endif
