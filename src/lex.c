#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "consts.h"
#include "lex.h"
#include "table.h"

int state = INITIAL;

char lowerLetters[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'x', 'w', 'y', 'z'};
char upperLetters[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'X', 'W', 'Y', 'Z'};
char letters[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'x', 'w', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'X', 'W', 'Y', 'Z'};
char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
char symbols[] = {'+', '-', '=', '(', ')', '*', '/', '<', '>', ',', ';'};
char halfsymbols[] = {'=', '!', '<', '>'};
char others[] = {' ', ':', '_', '.', '[', ']', '{', '}', '"', '\'', '\\', '/', '|', '?', '\n', '\r', '\t', ENOF};
char hexa[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

char in(char c, char* arr, int length)
{
	for (int i = 0; i < length; i++)
		if (arr[i] == c)
			return 1;

	return 0;
}

FILE *fp;

int lex()
{
	unsigned char lastType = reg.type;
	if (fp == NULL)
		fp = fopen(file, "r");

	char c;
	char final = 0;
	int i = 0;
	memset(reg.lexem, 0, MAX);
	reg.entry = malloc(sizeof(Entry));
	reg.type = 255;

	int unget = 1;

	while (!final)
	{
		c = fgetc(fp);

		if (!(in(c, letters, LETTERS) || in(c, digits, DIGITS) || in(c, symbols, SYMBOLS) || in(c, others, OTHERS) || in(c, halfsymbols, HALFSYMBOLS)))
		{
			printf("%lu:%s.\n", line, invalidChar);
			exit(INVALIDCHAR);
		}

		switch(state)
		{
			case INITIAL:
				if (c == '_' || in(c, letters, LETTERS))
				{
					state = IDENTIFIER;
					reg.lexem[i++] = c;
				}
				else if (c == '0')
				{
					state = HEXA;
					reg.lexem[i++] = c;
				}
				else if (in(c, digits, DIGITS))
				{
					state = NUMBER;
					reg.lexem[i++] = c;
				}
				else if (c == '"')
				{
					state = STRING;
					reg.lexem[i++] = c;
				}
				else if (c == '/')
				{
					state = BAR;
					reg.lexem[i++] = c;
				}
				else if (in(c, halfsymbols, HALFSYMBOLS))
				{
					state = HALFSYMBOL;
					reg.lexem[i++] = c;
				}
				else if (in(c, symbols, SYMBOLS))
				{
					state = INITIAL;
					final = 1;

					reg.lexem[i++] = c;

					Entry* entry = searchLexem(reg.lexem);

					memcpy(reg.lexem, entry->lexem, MAX);
					reg.token = entry->token;
					reg.entry = entry;

					unget = 0;
				}
				else if (c == DELIMITER || c == '\n' || c == '\r' || c == '\t')
				{
					if (c == '\n')
						line++;

					state = INITIAL;
				}
				else if (c == EOF)
				{
					if (lastType == 250)
					{
						printf("%lu: %s.\n", line, unexpectedEof);
						exit(UNEXPECTEDEOF);
					}

					final = 1;
					reg.type = 250;
				}
				else
				{
					final = 1;
					state = INITIAL;
					reg.lexem[i++] = c;
					printf("%lu:%s [%s].\n", line, unidentifiedLex, reg.lexem);
					exit(UNIDENTIFIEDLEX);
				}
				break;
			case IDENTIFIER:
				if (c == '_' || in(c, letters, LETTERS) || in(c, digits, DIGITS))
				{
					reg.lexem[i++] = c;
				}
				else
				{
					state = INITIAL;
					final = 1;

					Entry* entry = searchLexem(reg.lexem);

					if (entry == NULL)
					{
						reg.token = ID;
						reg.entry = addSymbol(reg.lexem, reg.token);
					}
					else
					{
						memcpy(reg.lexem, entry->lexem, MAX);
						reg.token = entry->token;
						reg.entry = entry;
						reg.type = entry->type;
					}
				}
				break;
			case NUMBER:
				if (in(c, digits, DIGITS))
				{
					reg.lexem[i++] = c;
				}
				else
				{
					state = INITIAL;
					final = 1;
					reg.token = LITERAL;

					if (atoi(reg.lexem) >= 0 && atoi(reg.lexem) <= 255)
						reg.type = BYTE;
					else
						reg.type = INT;
				}
				break;
			case HEXA:
				if (in(c, digits, DIGITS))
				{
					state = NUMBER;
					reg.lexem[i++] = c;
				}
				else if (c == 'x')
				{
					state = HEXFIRST;
					reg.lexem[i++] = c;
				}
				else
				{
					state = INITIAL;
					final = 1;
					reg.token = LITERAL;
					reg.type = BYTE;
				}
				break;
			case HEXFIRST:
				if (in(c, hexa, HEXAC))
				{
					state = HEXSEC;
					reg.lexem[i++] = c;
				}
				else
				{
					reg.lexem[i++] = c;
					printf("%lu:%s [%s].\n", line, unidentifiedLex, reg.lexem);
					exit(UNIDENTIFIEDLEX);
				}
				break;
			case HEXSEC:
				if (in(c, hexa, HEXAC))
				{
					state = INITIAL;
					final = 1;
					reg.lexem[i++] = c;
					reg.token = LITERAL;
					unget = 0;
					reg.type = BYTE;
				}
				else
				{
					reg.lexem[i++] = c;
					printf("%lu:%s [%s].\n", line, unidentifiedLex, reg.lexem);
					exit(UNIDENTIFIEDLEX);
				}
				break;
			case STRING:
				if (c == '"')
				{
					state = INITIAL;
					reg.lexem[i++] = c;
					final = 1;
					unget = 0;
					reg.token = LITERAL;
					reg.type = STR;
				}
				else if (c == ENOF || c == '\n')
				{
					printf("%lu: %s.\n", line, unexpectedEof);
					exit(UNEXPECTEDEOF);
				}
				else
				{
					reg.lexem[i++] = c;
				}
				break;
			case HALFSYMBOL:
				if (c == '=')
				{
					state = INITIAL;
					final = 1;
					unget = 0;

					reg.lexem[i++] = c;
					Entry* entry = searchLexem(reg.lexem);

					memcpy(reg.lexem, entry->lexem, MAX);
					reg.token = entry->token;
					reg.entry = entry;
				}
				else if (in(reg.lexem[i-1], symbols, SYMBOLS))
				{
					state = INITIAL;
					final = 1;
					Entry* entry = searchLexem(reg.lexem);

					memcpy(reg.lexem, entry->lexem, MAX);
					reg.token = entry->token;
					reg.entry = entry;
				}
				else
				{
					printf("%lu:%s [%s].\n", line, unidentifiedLex, reg.lexem);
					exit(UNIDENTIFIEDLEX);
				}
				break;
			case BAR:
				if (c == '*')
				{
					state = COMMENT;
				}
				else
				{
					state = INITIAL;
					final = 1;
					Entry* entry = searchLexem(reg.lexem);

					memcpy(reg.lexem, entry->lexem, MAX);
					reg.token = entry->token;
					reg.entry = entry;
				}
				break;
			case COMMENT:
				if (c == '*')
				{
					state = COMMENTEND;
				}
				else if (c == ENOF)
				{
					printf("%lu: %s.\n", line, unexpectedEof);
					exit(UNEXPECTEDEOF);
				}
				else if (c == '\n')
				{
					line++;
				}
				break;
			case COMMENTEND:
				if (c == '/')
				{
					state = INITIAL;
					i = 0;
				}
				else if (c == ENOF)
				{
					printf("%lu: %s.\n", line, unexpectedEof);
					exit(UNEXPECTEDEOF);
				}
				else if (c == '*')
				{
					state = COMMENTEND;
				}
				else
				{
					if (c == '\n') line++;
					state = COMMENT;
				}
				break;
		}
	}

	if (unget)
		ungetc(c, fp);

	return SUCCESS;
}