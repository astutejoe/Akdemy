#ifndef _LEXH_
#define _LEXH_

#include "table.h"

/* TOKENS */
#define ID      	0
#define LITERAL 	1
#define INTTOK		2
#define FINAL     	3
#define ELSE      	4
#define LEFTPAR		5
#define MINEQ		6
#define AND 		7
#define RIGHTPAR 	8
#define COMMA 		9
#define BEGIN 		10
#define OR			11
#define MINOR 		12
#define PLUS 		13
#define END  		14
#define NOT 		15
#define GREATER 	16
#define MINUS 		17
#define READLN 		18
#define WHILE  		19
#define EQUAL 		20
#define DIFF 		21
#define MULT 		22
#define WRITE 		23
#define IF			24
#define ATTR 		25
#define GREATEQ		26
#define DIVIDE 		27
#define WRITELN 	28
#define BYTETOK		29
#define STRINGTOK	30
#define BOOLEANTOK	31

/* STATES */
#define INITIAL 0
#define IDENTIFIER 1
#define NUMBER 2
#define HEXA 3
#define HEXFIRST 4
#define HEXSEC 5
#define STRING 6
#define HALFSYMBOL 7
#define BAR 8
#define COMMENT 9
#define COMMENTEND 10

#define DELIMITER ' '

/* CONSTS */
#define LETTERS 52
#define HLETTERS 26
#define DIGITS 10
#define SYMBOLS 10
#define HALFSYMBOLS 4
#define OTHERS 18
#define HEXAC 16

/* TYPES */
#define BYTE 0
#define STR 1
#define INT 2
#define BOOL 3

typedef struct lexicalRegister {
	char lexem[256];
	unsigned char token;
	unsigned char type;
	Entry* entry;
} LexicalRegister;

int lex();

unsigned long line;

LexicalRegister reg;

char* file;

#endif
