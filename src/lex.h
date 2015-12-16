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
#define SEMICOLON	7
#define AND 		8
#define RIGHTPAR 	9
#define COMMA 		10
#define BEGIN 		11
#define OR		12
#define MINOR 		13
#define PLUS 		14
#define END  		15
#define NOT 		16
#define GREATER 	17
#define MINUS 		18
#define READLN 		19
#define WHILE  		20
#define EQUAL 		21
#define DIFF 		22
#define MULT 		23
#define WRITE 		24
#define IF		25
#define ATTR 		26
#define GREATEQ		27
#define DIVIDE 		28
#define WRITELN 	29
#define BYTETOK		30
#define STRINGTOK	31
#define BOOLEANTOK	32

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
#define SYMBOLS 11
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
