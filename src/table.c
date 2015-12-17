#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lex.h"
#include "table.h"
#include "consts.h"

Entry table[HASHSIZE];

char* keywords[] = {"final","else","(","<=","TRUE","int","and",")",",","begin","FALSE","byte","or","<","+","end","bool","string","not",">","-","readln","while","==","!=","*","write","if","=",">=","/","writeln"};
unsigned char keywordsTokens[] = {FINAL, ELSE, LEFTPAR, MINEQ, LITERAL, INTTOK, AND, RIGHTPAR, COMMA, BEGIN, LITERAL, BYTETOK, OR , MINOR, PLUS, END , BOOLEANTOK, STRINGTOK, NOT, GREATER, MINUS, READLN, WHILE , EQUAL, DIFF, MULT, WRITE, IF , ATTR, GREATEQ, DIVIDE, WRITELN};

int initTable()
{
	for(int i = 0; i < KEYWORDS; i++)
	{
		addSymbol(keywords[i], keywordsTokens[i]);
	}

	return 0;
}

int hash(char* lexem)
{
	int hash = 0;
	char c = ' ';
	int i = 0;
	while (c != '\0')
	{
		c = lexem[i++];
		hash = (c + hash) % HASHSIZE;
	}

	return hash;
}

Entry* addSymbol(char* lexem, unsigned char token)
{
	int pos = hash(lexem);

	Entry* node = &table[pos];

	Entry* first = malloc(sizeof(Entry));
	memcpy(first->lexem, lexem, MAX);
	first->token = token;

	if (token == LITERAL) {
		first->type = BOOL;
	}

	first->next = node->next;
	node->next = first;

	return first;
}

Entry* searchLexem(char* lexem)
{
	Entry* node = &table[hash(lexem)];

	Entry* ret = (Entry*)0;

	int found = 0;
	while(!found && node->next != 0)
	{
		if (strcmp(node->next->lexem, lexem) == 0)
		{
			ret =  node->next;
			found = 1;
		}

		node = node->next;
	}

	return ret;
}

int printSymbolTable()
{
	for (int i = 0; i < HASHSIZE; i++)
	{
		Entry* node = &table[i];
		while(node->next != 0)
		{
			printf("i:%d %s Lexem: %s\n", i, "teste", node->next->lexem);
			node = node->next;
		}
	}

	return 0;
}
