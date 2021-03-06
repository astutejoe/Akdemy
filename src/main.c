#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "lex.h"
#include "consts.h"
#include "table.h"

void syntactic();
void matchToken(unsigned char);
char* newLabel();
int newTemporary(int);
void S();
void declaration();
void block();
void command();
void expr(int*, int*);
void A(int*, int*);
void B(int*, int*);
void C(int*, int*);
void D(int*, int*);

FILE* output;
char attribuitions[500000];
char variables[500000];

int label = 1;
int stack = STACKBASE;

int main(int argc, char *argv[])
{
	initTable();

	file = argv[1];
	output = fopen(argv[2], "w");
	line = 1;

	syntactic();

	return 0;
}

void syntactic()
{
	lex();
	S();
}

void S()
{
	unsigned int address = 0x4000;

	fprintf(output, "section	.data\n\tendl:\t db 0x0A\n");
	while (reg.token != BEGIN)
	{
		declaration(&address);
	}
	fprintf(output, "section	.bss\n\ttemporaries:\tresb 8388608\n");
	fprintf(output, "%s", variables);
	fprintf(output, "section	.text\nglobal	_start\n_start:\n");
	fprintf(output,  "%s", attribuitions);
	fflush(output);
	block();
	fprintf(output, "\tmov eax, 0x01\n\tint 0x80");
}

void declaration(unsigned int* address)
{
	unsigned char type, signal;
	Entry* entry;
#ifdef DEBUG
	printf("Declaration\n");
#endif
	if (reg.token == STRINGTOK || reg.token == INTTOK || reg.token == BYTETOK || reg.token == BOOLEANTOK)
	{
		type = reg.token;
		matchToken(reg.token);

		if (reg.entry->class == NULL) {
			reg.entry->class=VAR;
		} else {
			printf("%lu:%s.\n", line, alreadyDeclared);
			exit(ALREADYDECLARED);
		}

		reg.entry->address = *address;

		switch(type)
		{
			case STRINGTOK:
				reg.entry->type=STR;
				type = STR;
				sprintf(variables, "%s\t%s:\tresb 65536\n", variables, reg.lexem);
				break;
			case INTTOK:
				reg.entry->type=INT;
				type=INT;
				sprintf(variables, "%s\t%s:\tresd 1\n", variables, reg.lexem);
				break;
			case BYTETOK:
				reg.entry->type=BYTE;
				type=BYTE;
				sprintf(variables, "%s\t%s:\tresb 1\n", variables, reg.lexem);
				break;
			default:
				reg.entry->type=BOOL;
				type=BOOL;
				sprintf(variables, "%s\t%s:\tresb 1\n", variables, reg.lexem);
				break;
		}

		entry = reg.entry;

		matchToken(ID);

		if (reg.token == ATTR) {
			matchToken(ATTR);
			signal = -1;
			if(reg.token == PLUS || reg.token == MINUS) {
				signal = reg.token;
				matchToken(reg.token);
			}

			if (signal == PLUS || signal == MINUS) {
				if (reg.type != BYTE && reg.type != INT) {
					printf("%lu:%s.\n", line, incompatibleTypes);
					exit(INCOMPATIBLETYPES);
				}
				if (reg.type == BYTE && signal == MINUS) {
					reg.type = INT;
					type = INT;
				}
			}

			if (!(entry->type == reg.type || (entry->type == INT && reg.type == BYTE))) {
				printf("%lu:%s.\n", line, incompatibleTypes);
				exit(INCOMPATIBLETYPES);
			}

			if (type == STR) {
				sprintf(attribuitions, "%s\tmov ebx, %s\n", attribuitions, entry->lexem);
				for (int i = 1; i < strlen(reg.lexem)-1; i++) {
					sprintf(attribuitions, "%s\tmov al, %u\n", attribuitions, reg.lexem[i]);
					sprintf(attribuitions, "%s\tmov [ebx], al\n", attribuitions);
					sprintf(attribuitions, "%s\tadd ebx, 1\n", attribuitions);
				}
				sprintf(attribuitions, "%s\tmov al, 0\n", attribuitions);
				sprintf(attribuitions, "%s\tmov [ebx], al\n", attribuitions);
			} else if (type == INT) {
				sprintf(attribuitions, "%s\tmov eax, %s%d\n\tmov [%s], eax\n", attribuitions, (signal == MINUS ? "-" : "+"),  atoi(reg.lexem), entry->lexem);
			} else if (type == BYTE) {
				sprintf(attribuitions, "%s\tmov al, %d\n\tmov [%s], al\n", attribuitions, atoi(reg.lexem), entry->lexem);
			} else if (type == BOOL) {
				sprintf(attribuitions, "%s\tmov al, %d\n\tmov [%s], al\n", attribuitions, strncmp(reg.lexem,"TRUE", 4) == 0 ? 0xFF : 0x00, entry->lexem);
			}

			matchToken(LITERAL);
		}

		while(reg.token == COMMA)
		{
			matchToken(COMMA);

			if (reg.entry->class == NULL) {
				reg.entry->class=VAR;
			} else {
				printf("%lu:%s.\n", line, alreadyDeclared);
				exit(ALREADYDECLARED);
			}

			reg.entry->address = *address;
			reg.entry->type = type;

			switch(type)
			{
				case STR:
					sprintf(variables, "%s\t%s:\tresb 65536\n", variables, reg.lexem);
					break;
				case INT:
					sprintf(variables, "%s\t%s:\tresd 1\n", variables, reg.lexem);
					*address = *address + 2;
					break;
				default:
					sprintf(variables, "%s\t%s:\tresb 1\n", variables, reg.lexem);
					break;
			}

			entry = reg.entry;

			matchToken(ID);
			if (reg.token == ATTR) {
				matchToken(ATTR);
				signal = -1;

				if(reg.token == PLUS || reg.token == MINUS) {
					signal = reg.token;
					matchToken(reg.token);
				}

				if (signal == PLUS || signal == MINUS) {
					if (reg.type != BYTE && reg.type != INT) {
						printf("%lu:%s.\n", line, incompatibleTypes);
						exit(INCOMPATIBLETYPES);
					}
					if (reg.type == BYTE && signal == MINUS) {
						reg.type = INT;
						type = INT;
					}
				}

				if (!(entry->type == reg.type || (entry->type == INT && reg.type == BYTE))) {
					printf("%lu:%s.\n", line, incompatibleTypes);
					exit(INCOMPATIBLETYPES);
				}

				if (type == STR) {
					sprintf(attribuitions, "%s\tmov ebx, %s\n", attribuitions, entry->lexem);
					for (int i = 1; i < strlen(reg.lexem)-1; i++) {
						sprintf(attribuitions, "%s\tmov al, %u\n", attribuitions, reg.lexem[i]);
						sprintf(attribuitions, "%s\tmov [ebx], al\n", attribuitions);
						sprintf(attribuitions, "%s\tadd ebx, 1\n", attribuitions);
					}
					sprintf(attribuitions, "%s\tmov al, 0\n", attribuitions);
					sprintf(attribuitions, "%s\tmov [ebx], al\n", attribuitions);
				} else if (type == INT) {
					sprintf(attribuitions, "%s\tmov eax, %s%d\n\tmov [%s], eax\n", attribuitions, (signal == MINUS ? "-" : "+"),  atoi(reg.lexem), entry->lexem);
				} else if (type == BYTE) {
					sprintf(attribuitions, "%s\tmov al, %d\n\tmov [%s], al\n", attribuitions, atoi(reg.lexem), entry->lexem);
				} else if (type == BOOL) {
					sprintf(attribuitions, "%s\tmov al, %d\n\tmov [%s], al\n", attribuitions, strncmp(reg.lexem,"TRUE", 4) == 0 ? 0xFF : 0x00, entry->lexem);
				}

				matchToken(LITERAL);
			}
		}
	}
	else
	{
		matchToken(FINAL);

		if (reg.entry->class == NULL) {
			reg.entry->class = CONST;
		} else {
			printf("%lu:%s.\n", line, alreadyDeclared);
			exit(ALREADYDECLARED);
		}

		entry = reg.entry;

		matchToken(ID);
		matchToken(ATTR);

		signal = -1;

		if(reg.token == PLUS || reg.token == MINUS) {
			signal = reg.token;
			matchToken(reg.token);
		}

		if (signal == PLUS || signal == MINUS) {
			if (reg.type != BYTE && reg.token != INT) {
				printf("%lu:%s.\n", line, incompatibleTypes);
				exit(INCOMPATIBLETYPES);
			}
			if (reg.type == BYTE && signal == MINUS) {
				reg.type = INT;
				type = INT;
			}
		}

		entry->type = reg.type;
		entry->address = *address;

		switch(reg.type) {
			case STR:
				reg.lexem[0] = '\'';
				reg.lexem[strlen(reg.lexem)-1] = '\'';
				fprintf(output, "\t%s:\tdb %s,0\n", entry->lexem, reg.lexem);
				*address = *address + strlen(reg.lexem);
				break;
			case BYTE:
				fprintf(output, "\t%s:\tdb %u\n", entry->lexem, atoi(reg.lexem));
				*address = *address + 1;
				break;
			case BOOL:
				fprintf(output, "\t%s:\tdb %u\n", entry->lexem, (strncmp(reg.lexem, "TRUE", 4) == 0 ? 0xFF : 0x00));
				*address = *address + 1;
				break;
			default:
				fprintf(output, "\t%s:\tdd %s%d\n", entry->lexem, (signal == MINUS ? "-" : "+"), atoi(reg.lexem));
				*address = *address + 2;
				break;
		}

		matchToken(LITERAL);
	}
}

void block()
{
#ifdef DEBUG
	printf("Block\n");
#endif
	matchToken(BEGIN);
	while(reg.token != END)
	{
		command();
	}
	matchToken(END);
#ifdef DEBUG
	printf("End Block\n");
#endif
}

void command()
{
	int type, address;
#ifdef DEBUG
	printf("Command ");
#endif
	if(reg.token == ID)
	{
		if (reg.entry->class == NULL) {
			printf("%lu:%s.\n", line, undeclaredIdentifier);
			exit(UNDECLAREDIDENTIFIER);
		} else if (reg.entry->class == CONST) {
			printf("%lu:%s [%s].\n", line, incompatibleClass, reg.lexem);
			exit(INCOMPATIBLECLASS);
		}

		Entry* entry = reg.entry;

		matchToken(ID);
		matchToken(ATTR);
		int temporariesStack = stack;
		expr(&type, &address);

		if (!(entry->type == type || (entry->type == INT && type == BYTE))) {
			printf("%lu:%s.\n", line, incompatibleTypes);
			exit(INCOMPATIBLETYPES);
		}

		if (entry->type == INT) 
		{
			if (type == INT) 
			{
				fprintf(output, "\tmov eax, [temporaries+%u]\n", address);
				fprintf(output, "\tmov [%s], eax\n", entry->lexem);
			}
			else
			{
				fprintf(output, "\tmov eax, 0\n");
				fprintf(output, "\tmov al, [temporaries+%u]\n", address);
				fprintf(output, "\tmov [%s], eax\n", entry->lexem);
			}
		} 
		else if (entry->type == STR) 
		{
			fprintf(output, "\tmov eax, temporaries+%u\n", address);
			fprintf(output, "\tmov ebx, %s\n", entry->lexem);
			char* loop = newLabel();
			char* end = newLabel();
			fprintf(output, "%s:\n", loop);
			fprintf(output, "\tmov cl, [eax]\n");
			fprintf(output, "\tcmp cl, 0x00\n");
			fprintf(output, "\tje %s\n", end);
			fprintf(output, "\tmov [ebx], cl\n");
			fprintf(output, "\tadd eax, 1\n");
			fprintf(output, "\tadd ebx, 1\n");
			fprintf(output, "\tjmp %s\n", loop);
			fprintf(output, "%s:\n", end);
			fprintf(output, "\tmov [ebx], cl\n");
		} 
		else 
		{
			fprintf(output, "\tmov al, [temporaries+%u]\n\tmov [%s], al\n", address, entry->lexem);
		}

		stack = temporariesStack;

#ifdef DEBUG
	printf("\n");
#endif
	}
	else if(reg.token == WHILE)
	{
		matchToken(WHILE);

		char* loop = newLabel();
		char* end = newLabel();
		fprintf(output, "%s:\n", loop);

		int temporariesStack = stack;

		expr(&type, &address);
		if (type != BOOL)
		{
			printf("%lu:%s.\n", line, incompatibleTypes);
			exit(INCOMPATIBLETYPES);
		}

		fprintf(output, "\tmov al, [temporaries+%u]\n", address);
		fprintf(output, "\tcmp al, 0\n");
		fprintf(output, "\tje %s\n", end);

#ifdef DEBUG
	printf("\n");
#endif

		if (reg.token == BEGIN)
			block();
		else
			command();

		fprintf(output, "\tjmp %s\n", loop);
		fprintf(output, "%s:\n", end);
		stack = temporariesStack;
	}
	else if(reg.token == IF)
	{
		char* false = newLabel();
		char* end = newLabel();
		matchToken(IF);
		expr(&type, &address);

		if (type != BOOL)
		{
			printf("%lu:%s.\n", line, incompatibleTypes);
			exit(INCOMPATIBLETYPES);
		}

		fprintf(output, "\tmov al, [temporaries+%u]\n", address);
		fprintf(output, "\tcmp al, 0\n");
		fprintf(output, "\tje %s\n", false);

#ifdef DEBUG
	printf("\n");
#endif

		if (reg.token == BEGIN)
			block();
		else
			command();

		fprintf(output, "\tjmp %s\n", end);
		fprintf(output, "%s:\n", false);

		if (reg.token == ELSE)
		{
			matchToken(ELSE);
			if (reg.token == BEGIN)
				block();
			else
				command();
		}
		fprintf(output, "%s:\n", end);
	}
	else if(reg.token == READLN)
	{
		matchToken(READLN);

		int buffer = newTemporary(STR);

		fprintf(output, "\tmov eax, 3\n");
		fprintf(output, "\tmov ebx, 0\n");
		fprintf(output, "\tmov ecx, temporaries+%u\n", buffer);
		fprintf(output, "\tmov edx, 65536\n");
		fprintf(output, "\tint 0x80\n");

		if (reg.type == STR)
		{
			char* end = newLabel();
			char* loop = newLabel();
			fprintf(output, "\tmov eax, %s\n", reg.lexem);
			fprintf(output, "\tmov ebx, temporaries+%u\n", buffer);
			fprintf(output, "%s:\n", loop);
			fprintf(output, "\tmov cl, [ebx]\n");
			fprintf(output, "\tcmp cl, 0x0A\n");
			fprintf(output, "\tje %s\n", end);
			fprintf(output, "\tmov [eax], cl\n");
			fprintf(output, "\tadd eax, 1\n");
			fprintf(output, "\tadd ebx, 1\n");
			fprintf(output, "\tjmp %s\n", loop);
			fprintf(output, "%s:\n", end);
			fprintf(output, "\tmov cl, 0x00\n");
			fprintf(output, "\tmov [eax], cl\n");
		}
		else if (reg.type == BYTE)
		{
			char* loop = newLabel();
			char* end = newLabel();
			fprintf(output, "\tmov edi, temporaries+%u\n", buffer);
			fprintf(output, "\tmov al, 0\n");
			fprintf(output, "\tmov cl, 10\n");
			fprintf(output, "%s:\n", loop);
			fprintf(output, "\tmov bl, [edi]\n");
			fprintf(output, "\tcmp bl, 0x0A\n");
			fprintf(output, "\tje %s\n", end);
			fprintf(output, "\tmul cl\n");
			fprintf(output, "\tadd bl, -48\n");
			fprintf(output, "\tadd al, bl\n");
			fprintf(output, "\tadd edi, 1\n");
			fprintf(output, "\tjmp %s\n", loop);
			fprintf(output, "%s:\n", end);
			fprintf(output, "\tmov [%s], al\n", reg.lexem);
		}
		else if (reg.type == INT)
		{
			char* loop = newLabel();
			char* end = newLabel();
			char* positive = newLabel();
			fprintf(output, "\tmov edi, temporaries+%u\n", buffer);
			fprintf(output, "\tmov eax, 0\n");
			fprintf(output, "\tmov ecx, 10\n");
			fprintf(output, "\tmov esi, 1\n");
			fprintf(output, "\tmov bl, [edi]\n");
			fprintf(output, "\tcmp bl, 2Dh\n");
			fprintf(output, "\tjne %s\n", loop);
			fprintf(output, "\tmov esi, -1\n");
			fprintf(output, "\tadd edi, 1\n");
			fprintf(output, "\tmov bl, [edi]\n");
			fprintf(output, "%s:\n", loop);
			fprintf(output, "\tmov ebx, 0\n");
			fprintf(output, "\tmov bl, [edi]\n");
			fprintf(output, "\tcmp bl, 0x0A\n");
			fprintf(output, "\tje %s\n", end);
			fprintf(output, "\timul ecx\n");
			fprintf(output, "\tadd bl, -48\n");
			fprintf(output, "\tadd eax, ebx\n");
			fprintf(output, "\tadd edi, 1\n");
			fprintf(output, "\tjmp %s\n", loop);
			fprintf(output, "%s:\n", end);
			fprintf(output, "\tmov edx, esi\n");
			fprintf(output, "\timul edx\n");
			fprintf(output, "\tmov [%s], eax\n", reg.lexem);
		}

		matchToken(ID);
#ifdef DEBUG
	printf("\n");
#endif
	}
	else if (reg.token == WRITE)
	{
		matchToken(WRITE);
		int temporariesStack = stack;
		expr(&type, &address);

		if (type == STR) {
			char* loop = newLabel();
			char* end = newLabel();

			fprintf(output, "\tmov edx, 0\n");
			fprintf(output, "\tmov ebx, temporaries+%u\n", address);
			fprintf(output, "%s:\n", loop);
			fprintf(output, "\tmov al, [ebx]\n");
			fprintf(output, "\tcmp al, 0x00\n");
			fprintf(output, "\tje %s\n", end);
			fprintf(output, "\tadd ebx, 1\n");
			fprintf(output, "\tadd edx, 1\n");
			fprintf(output, "\tjmp %s\n", loop);
			fprintf(output, "%s:\n", end);
			fprintf(output, "\tmov eax, 4\n");
			fprintf(output, "\tmov ebx, 1\n");
			fprintf(output, "\tmov ecx, temporaries+%u\n", address);
			fprintf(output, "\tint 0x80\n");
		} else if (type == INT || type == BYTE) {
			int temporariesString = newTemporary(STR);
			char* positive = newLabel();
			char* digits = newLabel();
			char* printing = newLabel();
			fprintf(output, "\tmov edi, temporaries+%u\n", temporariesString);
			fprintf(output, "\tmov eax, 0\n");

			if (type == INT)
				fprintf(output, "\tmov eax, [temporaries+%u]\n", address);
			else
				fprintf(output, "\tmov al, [temporaries+%u]\n", address);

			fprintf(output, "\tmov cx, 0\n");
			fprintf(output, "\tmov esi, 0\n");
			fprintf(output, "\tcmp eax, 0\n");
			fprintf(output, "\tjge %s\n", positive);
			fprintf(output, "\tmov dl, 0x2D\n");
			fprintf(output, "\tmov [edi], dl\n");
			fprintf(output, "\tadd edi, 1\n");
			fprintf(output, "\tneg eax\n");
			fprintf(output, "\tadd esi, 1\n");
			fprintf(output, "%s:\n", positive);
			fprintf(output, "\tmov ebx, 10\n");
			fprintf(output, "%s:\n", digits);
			fprintf(output, "\tadd cx, 1\n");
			fprintf(output, "\tadd esi, 1\n");
			fprintf(output, "\tmov edx, 0\n");
			fprintf(output, "\tidiv ebx\n");
			fprintf(output, "\tpush dx\n");
			fprintf(output, "\tcmp eax, 0\n");
			fprintf(output, "\tjne %s\n", digits);
			fprintf(output, "%s:\n", printing);
			fprintf(output, "\tpop dx\n");
			fprintf(output, "\tadd dx, 48\n");
			fprintf(output, "\tmov [edi], dl\n");
			fprintf(output, "\tadd edi, 1\n");
			fprintf(output, "\tadd cx, -1\n");
			fprintf(output,	"\tcmp cx, 0\n");
			fprintf(output, "\tjne %s\n", printing);
			fprintf(output, "\tmov eax, 4\n");
			fprintf(output, "\tmov ebx, 1\n");
			fprintf(output, "\tmov ecx, temporaries+%u\n", temporariesString);
			fprintf(output, "\tmov edx, esi\n");
			fprintf(output, "\tint 0x80\n");
		} else {
			char* true = newLabel();
			char* print = newLabel();
			int temporariesOut = newTemporary(STR);
			fprintf(output, "\tmov al, [temporaries+%u]\n", address);
			fprintf(output, "\tcmp al, 0\n");
			fprintf(output, "\tjne %s\n", true);
			fprintf(output, "\tmov edi, temporaries+%u\n", temporariesOut);
			fprintf(output, "\tmov al, 70\n");
			fprintf(output, "\tmov [edi], al\n");
			fprintf(output, "\tadd edi, 1\n");
			fprintf(output, "\tmov al, 65\n");
			fprintf(output, "\tmov [edi], al\n");
			fprintf(output, "\tadd edi, 1\n");
			fprintf(output, "\tmov al, 76\n");
			fprintf(output, "\tmov [edi], al\n");
			fprintf(output, "\tadd edi, 1\n");
			fprintf(output, "\tmov al, 83\n");
			fprintf(output, "\tmov [edi], al\n");
			fprintf(output, "\tadd edi, 1\n");
			fprintf(output, "\tmov al, 69\n");
			fprintf(output, "\tmov [edi], al\n");
			fprintf(output, "\tadd edi, 1\n");
			fprintf(output, "\tmov al, 36\n");
			fprintf(output, "\tmov [edi], al\n");
			fprintf(output, "\tmov edx, 5\n");
			fprintf(output, "\tjmp %s\n", print);
			fprintf(output, "%s:\n", true);
			fprintf(output, "\tmov edi, temporaries+%u\n", temporariesOut);
			fprintf(output, "\tmov al, 84\n");
			fprintf(output, "\tmov [edi], al\n");
			fprintf(output, "\tadd edi, 1\n");
			fprintf(output, "\tmov al, 82\n");
			fprintf(output, "\tmov [edi], al\n");
			fprintf(output, "\tadd edi, 1\n");
			fprintf(output, "\tmov al, 85\n");
			fprintf(output, "\tmov [edi], al\n");
			fprintf(output, "\tadd edi, 1\n");
			fprintf(output, "\tmov al, 69\n");
			fprintf(output, "\tmov [edi], al\n");
			fprintf(output, "\tadd edi, 1\n");
			fprintf(output, "\tmov al, 36\n");
			fprintf(output, "\tmov [edi], al\n");
			fprintf(output, "\tmov edx, 4\n");
			fprintf(output, "%s:\n", print);
			fprintf(output, "\tmov eax, 4\n");
			fprintf(output, "\tmov ebx, 1\n");
			fprintf(output, "\tmov ecx, temporaries+%u\n", temporariesOut);
			fprintf(output, "\tint 0x80\n");
		}

		while (reg.token == COMMA)
		{
			matchToken(COMMA);
			expr(&type, &address);

			if (type == STR) {
				char* loop = newLabel();
				char* end = newLabel();

				fprintf(output, "\tmov edx, 0\n");
				fprintf(output, "\tmov ebx, temporaries+%u\n", address);
				fprintf(output, "%s:\n", loop);
				fprintf(output, "\tmov al, [ebx]\n");
				fprintf(output, "\tcmp al, 0x00\n");
				fprintf(output, "\tje %s\n", end);
				fprintf(output, "\tadd ebx, 1\n");
				fprintf(output, "\tadd edx, 1\n");
				fprintf(output, "\tjmp %s\n", loop);
				fprintf(output, "%s:\n", end);
				fprintf(output, "\tmov eax, 4\n");
				fprintf(output, "\tmov ebx, 1\n");
				fprintf(output, "\tmov ecx, temporaries+%u\n", address);
				fprintf(output, "\tint 0x80\n");
			} else if (type == INT || type == BYTE) {
				int temporariesString = newTemporary(STR);
				char* positive = newLabel();
				char* digits = newLabel();
				char* printing = newLabel();
				fprintf(output, "\tmov edi, temporaries+%u\n", temporariesString);
				fprintf(output, "\tmov eax, 0\n");

				if (type == INT)
					fprintf(output, "\tmov eax, [temporaries+%u]\n", address);
				else
					fprintf(output, "\tmov al, [temporaries+%u]\n", address);

				fprintf(output, "\tmov cx, 0\n");
				fprintf(output, "\tmov esi, 0\n");
				fprintf(output, "\tcmp eax, 0\n");
				fprintf(output, "\tjge %s\n", positive);
				fprintf(output, "\tmov dl, 0x2D\n");
				fprintf(output, "\tmov [edi], dl\n");
				fprintf(output, "\tadd edi, 1\n");
				fprintf(output, "\tneg eax\n");
				fprintf(output, "\tadd esi, 1\n");
				fprintf(output, "%s:\n", positive);
				fprintf(output, "\tmov ebx, 10\n");
				fprintf(output, "%s:\n", digits);
				fprintf(output, "\tadd cx, 1\n");
				fprintf(output, "\tadd esi, 1\n");
				fprintf(output, "\tmov edx, 0\n");
				fprintf(output, "\tidiv ebx\n");
				fprintf(output, "\tpush dx\n");
				fprintf(output, "\tcmp eax, 0\n");
				fprintf(output, "\tjne %s\n", digits);
				fprintf(output, "%s:\n", printing);
				fprintf(output, "\tpop dx\n");
				fprintf(output, "\tadd dx, 48\n");
				fprintf(output, "\tmov [edi], dl\n");
				fprintf(output, "\tadd edi, 1\n");
				fprintf(output, "\tadd cx, -1\n");
				fprintf(output,	"\tcmp cx, 0\n");
				fprintf(output, "\tjne %s\n", printing);
				fprintf(output, "\tmov eax, 4\n");
				fprintf(output, "\tmov ebx, 1\n");
				fprintf(output, "\tmov ecx, temporaries+%u\n", temporariesString);
				fprintf(output, "\tmov edx, esi\n");
				fprintf(output, "\tint 0x80\n");
			} else {
				char* true = newLabel();
				char* print = newLabel();
				int temporariesOut = newTemporary(STR);
				fprintf(output, "\tmov al, [temporaries+%u]\n", address);
				fprintf(output, "\tcmp al, 0\n");
				fprintf(output, "\tjne %s\n", true);
				fprintf(output, "\tmov edi, temporaries+%u\n", temporariesOut);
				fprintf(output, "\tmov al, 70\n");
				fprintf(output, "\tmov [edi], al\n");
				fprintf(output, "\tadd edi, 1\n");
				fprintf(output, "\tmov al, 65\n");
				fprintf(output, "\tmov [edi], al\n");
				fprintf(output, "\tadd edi, 1\n");
				fprintf(output, "\tmov al, 76\n");
				fprintf(output, "\tmov [edi], al\n");
				fprintf(output, "\tadd edi, 1\n");
				fprintf(output, "\tmov al, 83\n");
				fprintf(output, "\tmov [edi], al\n");
				fprintf(output, "\tadd edi, 1\n");
				fprintf(output, "\tmov al, 69\n");
				fprintf(output, "\tmov [edi], al\n");
				fprintf(output, "\tadd edi, 1\n");
				fprintf(output, "\tmov al, 36\n");
				fprintf(output, "\tmov [edi], al\n");
				fprintf(output, "\tmov edx, 5\n");
				fprintf(output, "\tjmp %s\n", print);
				fprintf(output, "%s:\n", true);
				fprintf(output, "\tmov edi, temporaries+%u\n", temporariesOut);
				fprintf(output, "\tmov al, 84\n");
				fprintf(output, "\tmov [edi], al\n");
				fprintf(output, "\tadd edi, 1\n");
				fprintf(output, "\tmov al, 82\n");
				fprintf(output, "\tmov [edi], al\n");
				fprintf(output, "\tadd edi, 1\n");
				fprintf(output, "\tmov al, 85\n");
				fprintf(output, "\tmov [edi], al\n");
				fprintf(output, "\tadd edi, 1\n");
				fprintf(output, "\tmov al, 69\n");
				fprintf(output, "\tmov [edi], al\n");
				fprintf(output, "\tadd edi, 1\n");
				fprintf(output, "\tmov al, 36\n");
				fprintf(output, "\tmov [edi], al\n");
				fprintf(output, "\tmov edx, 4\n");
				fprintf(output, "%s:\n", print);
				fprintf(output, "\tmov eax, 4\n");
				fprintf(output, "\tmov ebx, 1\n");
				fprintf(output, "\tmov ecx, temporaries+%u\n", temporariesOut);
				fprintf(output, "\tint 0x80\n");
			}
		}

		stack = temporariesStack;
#ifdef DEBUG
	printf("\n");
#endif
	}
	else
	{
		matchToken(WRITELN);

		int temporariesStack = stack;

		expr(&type, &address);

		if (type == STR) {
			char* loop = newLabel();
			char* end = newLabel();

			fprintf(output, "\tmov edx, 0\n");
			fprintf(output, "\tmov ebx, temporaries+%u\n", address);
			fprintf(output, "%s:\n", loop);
			fprintf(output, "\tmov al, [ebx]\n");
			fprintf(output, "\tcmp al, 0x00\n");
			fprintf(output, "\tje %s\n", end);
			fprintf(output, "\tadd ebx, 1\n");
			fprintf(output, "\tadd edx, 1\n");
			fprintf(output, "\tjmp %s\n", loop);
			fprintf(output, "%s:\n", end);
			fprintf(output, "\tmov eax, 4\n");
			fprintf(output, "\tmov ebx, 1\n");
			fprintf(output, "\tmov ecx, temporaries+%u\n", address);
			fprintf(output, "\tint 0x80\n");
		} else if (type == INT || type == BYTE) {
			int temporariesString = newTemporary(STR);
			char* positive = newLabel();
			char* digits = newLabel();
			char* printing = newLabel();
			fprintf(output, "\tmov edi, temporaries+%u\n", temporariesString);
			fprintf(output, "\tmov eax, 0\n");

			if (type == INT)
				fprintf(output, "\tmov eax, [temporaries+%u]\n", address);
			else
				fprintf(output, "\tmov al, [temporaries+%u]\n", address);

			fprintf(output, "\tmov cx, 0\n");
			fprintf(output, "\tmov esi, 0\n");
			fprintf(output, "\tcmp eax, 0\n");
			fprintf(output, "\tjge %s\n", positive);
			fprintf(output, "\tmov dl, 0x2D\n");
			fprintf(output, "\tmov [edi], dl\n");
			fprintf(output, "\tadd edi, 1\n");
			fprintf(output, "\tneg eax\n");
			fprintf(output, "\tadd esi, 1\n");
			fprintf(output, "%s:\n", positive);
			fprintf(output, "\tmov ebx, 10\n");
			fprintf(output, "%s:\n", digits);
			fprintf(output, "\tadd cx, 1\n");
			fprintf(output, "\tadd esi, 1\n");
			fprintf(output, "\tmov edx, 0\n");
			fprintf(output, "\tidiv ebx\n");
			fprintf(output, "\tpush dx\n");
			fprintf(output, "\tcmp eax, 0\n");
			fprintf(output, "\tjne %s\n", digits);
			fprintf(output, "%s:\n", printing);
			fprintf(output, "\tpop dx\n");
			fprintf(output, "\tadd dx, 48\n");
			fprintf(output, "\tmov [edi], dl\n");
			fprintf(output, "\tadd edi, 1\n");
			fprintf(output, "\tadd cx, -1\n");
			fprintf(output,	"\tcmp cx, 0\n");
			fprintf(output, "\tjne %s\n", printing);
			fprintf(output, "\tmov eax, 4\n");
			fprintf(output, "\tmov ebx, 1\n");
			fprintf(output, "\tmov ecx, temporaries+%u\n", temporariesString);
			fprintf(output, "\tmov edx, esi\n");
			fprintf(output, "\tint 0x80\n");
		} else {
			char* true = newLabel();
			char* print = newLabel();
			int temporariesOut = newTemporary(STR);
			fprintf(output, "\tmov al, [temporaries+%u]\n", address);
			fprintf(output, "\tcmp al, 0\n");
			fprintf(output, "\tjne %s\n", true);
			fprintf(output, "\tmov edi, temporaries+%u\n", temporariesOut);
			fprintf(output, "\tmov al, 70\n");
			fprintf(output, "\tmov [edi], al\n");
			fprintf(output, "\tadd edi, 1\n");
			fprintf(output, "\tmov al, 65\n");
			fprintf(output, "\tmov [edi], al\n");
			fprintf(output, "\tadd edi, 1\n");
			fprintf(output, "\tmov al, 76\n");
			fprintf(output, "\tmov [edi], al\n");
			fprintf(output, "\tadd edi, 1\n");
			fprintf(output, "\tmov al, 83\n");
			fprintf(output, "\tmov [edi], al\n");
			fprintf(output, "\tadd edi, 1\n");
			fprintf(output, "\tmov al, 69\n");
			fprintf(output, "\tmov [edi], al\n");
			fprintf(output, "\tadd edi, 1\n");
			fprintf(output, "\tmov al, 36\n");
			fprintf(output, "\tmov [edi], al\n");
			fprintf(output, "\tmov edx, 5\n");
			fprintf(output, "\tjmp %s\n", print);
			fprintf(output, "%s:\n", true);
			fprintf(output, "\tmov edi, temporaries+%u\n", temporariesOut);
			fprintf(output, "\tmov al, 84\n");
			fprintf(output, "\tmov [edi], al\n");
			fprintf(output, "\tadd edi, 1\n");
			fprintf(output, "\tmov al, 82\n");
			fprintf(output, "\tmov [edi], al\n");
			fprintf(output, "\tadd edi, 1\n");
			fprintf(output, "\tmov al, 85\n");
			fprintf(output, "\tmov [edi], al\n");
			fprintf(output, "\tadd edi, 1\n");
			fprintf(output, "\tmov al, 69\n");
			fprintf(output, "\tmov [edi], al\n");
			fprintf(output, "\tadd edi, 1\n");
			fprintf(output, "\tmov al, 36\n");
			fprintf(output, "\tmov [edi], al\n");
			fprintf(output, "\tmov edx, 4\n");
			fprintf(output, "%s:\n", print);
			fprintf(output, "\tmov eax, 4\n");
			fprintf(output, "\tmov ebx, 1\n");
			fprintf(output, "\tmov ecx, temporaries+%u\n", temporariesOut);
			fprintf(output, "\tint 0x80\n");
		}

		while (reg.token == COMMA)
		{
			matchToken(COMMA);
			expr(&type, &address);

			if (type == STR) {
				char* loop = newLabel();
				char* end = newLabel();

				fprintf(output, "\tmov edx, 0\n");
				fprintf(output, "\tmov ebx, temporaries+%u\n", address);
				fprintf(output, "%s:\n", loop);
				fprintf(output, "\tmov al, [ebx]\n");
				fprintf(output, "\tcmp al, 0x00\n");
				fprintf(output, "\tje %s\n", end);
				fprintf(output, "\tadd ebx, 1\n");
				fprintf(output, "\tadd edx, 1\n");
				fprintf(output, "\tjmp %s\n", loop);
				fprintf(output, "%s:\n", end);
				fprintf(output, "\tmov eax, 4\n");
				fprintf(output, "\tmov ebx, 1\n");
				fprintf(output, "\tmov ecx, temporaries+%u\n", address);
				fprintf(output, "\tint 0x80\n");
			} else if (type == INT || type == BYTE) {
				int temporariesString = newTemporary(STR);
				char* positive = newLabel();
				char* digits = newLabel();
				char* printing = newLabel();
				fprintf(output, "\tmov edi, temporaries+%u\n", temporariesString);
				fprintf(output, "\tmov eax, 0\n");

				if (type == INT)
					fprintf(output, "\tmov eax, [temporaries+%u]\n", address);
				else
					fprintf(output, "\tmov al, [temporaries+%u]\n", address);

				fprintf(output, "\tmov cx, 0\n");
				fprintf(output, "\tmov esi, 0\n");
				fprintf(output, "\tcmp eax, 0\n");
				fprintf(output, "\tjge %s\n", positive);
				fprintf(output, "\tmov dl, 0x2D\n");
				fprintf(output, "\tmov [edi], dl\n");
				fprintf(output, "\tadd edi, 1\n");
				fprintf(output, "\tneg eax\n");
				fprintf(output, "\tadd esi, 1\n");
				fprintf(output, "%s:\n", positive);
				fprintf(output, "\tmov ebx, 10\n");
				fprintf(output, "%s:\n", digits);
				fprintf(output, "\tadd cx, 1\n");
				fprintf(output, "\tadd esi, 1\n");
				fprintf(output, "\tmov dx, 0\n");
				fprintf(output, "\tidiv ebx\n");
				fprintf(output, "\tpush dx\n");
				fprintf(output, "\tcmp eax, 0\n");
				fprintf(output, "\tjne %s\n", digits);
				fprintf(output, "%s:\n", printing);
				fprintf(output, "\tpop dx\n");
				fprintf(output, "\tadd dx, 48\n");
				fprintf(output, "\tmov [edi], dl\n");
				fprintf(output, "\tadd edi, 1\n");
				fprintf(output, "\tadd cx, -1\n");
				fprintf(output,	"\tcmp cx, 0\n");
				fprintf(output, "\tjne %s\n", printing);
				fprintf(output, "\tmov eax, 4\n");
				fprintf(output, "\tmov ebx, 1\n");
				fprintf(output, "\tmov ecx, temporaries+%u\n", temporariesString);
				fprintf(output, "\tmov edx, esi\n");
				fprintf(output, "\tint 0x80\n");
			} else {
				char* true = newLabel();
				char* print = newLabel();
				int temporariesOut = newTemporary(STR);
				fprintf(output, "\tmov al, [temporaries+%u]\n", address);
				fprintf(output, "\tcmp al, 0\n");
				fprintf(output, "\tjne %s\n", true);
				fprintf(output, "\tmov edi, temporaries+%u\n", temporariesOut);
				fprintf(output, "\tmov al, 70\n");
				fprintf(output, "\tmov [edi], al\n");
				fprintf(output, "\tadd edi, 1\n");
				fprintf(output, "\tmov al, 65\n");
				fprintf(output, "\tmov [edi], al\n");
				fprintf(output, "\tadd edi, 1\n");
				fprintf(output, "\tmov al, 76\n");
				fprintf(output, "\tmov [edi], al\n");
				fprintf(output, "\tadd edi, 1\n");
				fprintf(output, "\tmov al, 83\n");
				fprintf(output, "\tmov [edi], al\n");
				fprintf(output, "\tadd edi, 1\n");
				fprintf(output, "\tmov al, 69\n");
				fprintf(output, "\tmov [edi], al\n");
				fprintf(output, "\tadd edi, 1\n");
				fprintf(output, "\tmov al, 36\n");
				fprintf(output, "\tmov [edi], al\n");
				fprintf(output, "\tmov edx, 5\n");
				fprintf(output, "\tjmp %s\n", print);
				fprintf(output, "%s:\n", true);
				fprintf(output, "\tmov edi, temporaries+%u\n", temporariesOut);
				fprintf(output, "\tmov al, 84\n");
				fprintf(output, "\tmov [edi], al\n");
				fprintf(output, "\tadd edi, 1\n");
				fprintf(output, "\tmov al, 82\n");
				fprintf(output, "\tmov [edi], al\n");
				fprintf(output, "\tadd edi, 1\n");
				fprintf(output, "\tmov al, 85\n");
				fprintf(output, "\tmov [edi], al\n");
				fprintf(output, "\tadd edi, 1\n");
				fprintf(output, "\tmov al, 69\n");
				fprintf(output, "\tmov [edi], al\n");
				fprintf(output, "\tadd edi, 1\n");
				fprintf(output, "\tmov al, 36\n");
				fprintf(output, "\tmov [edi], al\n");
				fprintf(output, "\tmov edx, 4\n");
				fprintf(output, "%s:\n", print);
				fprintf(output, "\tmov eax, 4\n");
				fprintf(output, "\tmov ebx, 1\n");
				fprintf(output, "\tmov ecx, temporaries+%u\n", temporariesOut);
				fprintf(output, "\tint 0x80\n");
			}
		}

		fprintf(output, "\tmov eax, 4\n");
		fprintf(output, "\tmov ebx, 1\n");
		fprintf(output, "\tmov ecx, endl\n");
		fprintf(output, "\tmov edx, 1\n");
		fprintf(output, "\tint 0x80\n");
#ifdef DEBUG
	printf("\n");
#endif

		stack = temporariesStack;
	}	
}

void expr(int* type, int* address)
{
#ifdef DEBUG
	printf("Expression ");
#endif
	A(type, address);
	int firstExprAdd = *address;
	int firstExprType = *type;

	if (reg.token == EQUAL || reg.token == DIFF || reg.token == MINOR || reg.token == GREATER || reg.token == MINEQ || reg.token == GREATEQ)
	{
		int token = reg.token;
		matchToken(reg.token);
		A(type, address);

		if (firstExprType == STR && *type != STR)
		{
			printf("%lu:%s.\n", line, incompatibleTypes);
			exit(INCOMPATIBLETYPES);
		}
		else if (firstExprType == BOOL && *type != BOOL)
		{
			printf("%lu:%s.\n", line, incompatibleTypes);
			exit(INCOMPATIBLETYPES);
		}
		else if (firstExprType == BYTE && (*type != INT && *type != BYTE))
		{
			printf("%lu:%s.\n", line, incompatibleTypes);
			exit(INCOMPATIBLETYPES);
		}
		else if (firstExprType == INT && (*type != INT && *type != BYTE))
		{
			printf("%lu:%s.\n", line, incompatibleTypes);
			exit(INCOMPATIBLETYPES);
		}
		else if (firstExprType == STR && token != EQUAL)
		{
			printf("%lu:%s.\n", line, incompatibleTypes);
			exit(INCOMPATIBLETYPES);
		}
		else if (firstExprType == BOOL && (token != EQUAL && token != DIFF))
		{
			printf("%lu:%s.\n", line, incompatibleTypes);
			exit(INCOMPATIBLETYPES);
		}
		else
		{	char* true = newLabel();
			char* end = newLabel();
			switch(token)
			{
				case EQUAL:
					if (*type == BOOL)
					{
						fprintf(output, "\tmov al, [temporaries+%u]\n", firstExprAdd);
						fprintf(output, "\tmov bl, [temporaries+%u]\n", *address);
						fprintf(output, "\tcmp al, bl\n");
						fprintf(output, "\tje %s\n", true);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", true);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "%s:\n", end);
						fprintf(output, "\tmov [temporaries+%u], al\n", *address);
					}
					else if (*type == INT || *type == BYTE)
					{
						if (firstExprType == BYTE && *type == BYTE)
						{
							fprintf(output, "\tmov al, [temporaries+%u]\n", firstExprAdd);
							fprintf(output, "\tmov bl, [temporaries+%u]\n", *address);
							fprintf(output, "\tcmp al, bl\n");
							fprintf(output, "\tje %s\n", true);
							fprintf(output, "\tmov al, 0\n");
							fprintf(output, "\tjmp %s\n", end);
							fprintf(output, "%s:\n", true);
							fprintf(output, "\tmov al, 255\n");
							fprintf(output, "%s:\n", end);
							fprintf(output, "\tmov [temporaries+%u], al\n", *address);
						}
						else if (firstExprType == INT && *type == BYTE)
						{
							fprintf(output, "\tmov eax, [temporaries+%u]\n", firstExprAdd);
							fprintf(output, "\tmov ebx, 0\n");
							fprintf(output, "\tmov bl, [temporaries+%u]\n", *address);
							fprintf(output, "\tcmp eax, ebx\n");
							fprintf(output, "\tje %s\n", true);
							fprintf(output, "\tmov al, 0\n");
							fprintf(output, "\tjmp %s\n", end);
							fprintf(output, "%s:\n", true);
							fprintf(output, "\tmov al, 255\n");
							fprintf(output, "%s:\n", end);
							fprintf(output, "\tmov [temporaries+%u], al\n", *address);
						}
						else if (firstExprType == BYTE && *type == INT)
						{
							fprintf(output, "\tmov eax, 0\n");
							fprintf(output, "\tmov al, [temporaries+%u]\n", firstExprAdd);
							fprintf(output, "\tmov ebx, [temporaries+%u]\n", *address);
							fprintf(output, "\tcmp eax, ebx\n");
							fprintf(output, "\tje %s\n", true);
							fprintf(output, "\tmov al, 0\n");
							fprintf(output, "\tjmp %s\n", end);
							fprintf(output, "%s:\n", true);
							fprintf(output, "\tmov al, 255\n");
							fprintf(output, "%s:\n", end);
							fprintf(output, "\tmov [temporaries+%u], al\n", *address);
						}
						else
						{
							fprintf(output, "\tmov eax, [temporaries+%u]\n", firstExprAdd);
							fprintf(output, "\tmov ebx, [temporaries+%u]\n", *address);
							fprintf(output, "\tcmp eax, ebx\n");
							fprintf(output, "\tje %s\n", true);
							fprintf(output, "\tmov al, 0\n");
							fprintf(output, "\tjmp %s\n", end);
							fprintf(output, "%s:\n", true);
							fprintf(output, "\tmov al, 255\n");
							fprintf(output, "%s:\n", end);
							fprintf(output, "\tmov [temporaries+%u], al\n", *address);
						}
					}
					else
					{
						char* equal = newLabel();
						char* notequal = newLabel();
						char* iterate = newLabel();
						char* firstended = newLabel();
						char* end = newLabel();
						fprintf(output, "\tmov edi, temporaries+%u\n", firstExprAdd);
						fprintf(output, "\tmov ebx, temporaries+%u\n", *address);
						fprintf(output, "%s:\n", iterate);
						fprintf(output, "\tmov al, [edi]\n");
						fprintf(output, "\tcmp al, 0x00\n");
						fprintf(output, "\tje %s\n", firstended);
						fprintf(output, "\tmov cl, [ebx]\n");
						fprintf(output, "\tcmp cl, 0x00\n");
						fprintf(output, "\tje %s\n", notequal);
						fprintf(output, "\tcmp al, cl\n");
						fprintf(output, "\tjne %s\n", notequal);
						fprintf(output, "\tadd ebx, 1\n");
						fprintf(output, "\tadd edi, 1\n");
						fprintf(output, "\tjmp %s\n", iterate);
						fprintf(output, "%s:\n", equal);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "\tmov [temporaries+%u], al\n", *address);
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", firstended);
						fprintf(output, "\tmov al, [ebx]\n");
						fprintf(output, "\tcmp al, 0x00\n");
						fprintf(output, "\tje %s\n", equal);
						fprintf(output, "%s:\n", notequal);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tmov [temporaries+%u], al\n", *address);
						fprintf(output, "%s:\n", end);
					}
					break;
				case DIFF:
					if (*type == BOOL)
					{
						fprintf(output, "\tmov al, [temporaries+%u]\n", firstExprAdd);
						fprintf(output, "\tmov bl, [temporaries+%u]\n", *address);
						fprintf(output, "\tcmp al, bl\n");
						fprintf(output, "\tjne %s\n", true);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", true);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "%s:\n", end);
						fprintf(output, "\tmov [temporaries+%u], al\n", *address);
					}
					else if (*type == INT || *type == BYTE)
					{
						if (firstExprType == BYTE && *type == BYTE)
						{
							fprintf(output, "\tmov al, [temporaries+%u]\n", firstExprAdd);
							fprintf(output, "\tmov bl, [temporaries+%u]\n", *address);
							fprintf(output, "\tcmp al, bl\n");
							fprintf(output, "\tjne %s\n", true);
							fprintf(output, "\tmov al, 0\n");
							fprintf(output, "\tjmp %s\n", end);
							fprintf(output, "%s:\n", true);
							fprintf(output, "\tmov al, 255\n");
							fprintf(output, "%s:\n", end);
							fprintf(output, "\tmov [temporaries+%u], al\n", *address);
						}
						else if (firstExprType == INT && *type == BYTE)
						{
							fprintf(output, "\tmov eax, [temporaries+%u]\n", firstExprAdd);
							fprintf(output, "\tmov ebx, 0\n");
							fprintf(output, "\tmov bl, [temporaries+%u]\n", *address);
							fprintf(output, "\tcmp eax, ebx\n");
							fprintf(output, "\tjne %s\n", true);
							fprintf(output, "\tmov al, 0\n");
							fprintf(output, "\tjmp %s\n", end);
							fprintf(output, "%s:\n", true);
							fprintf(output, "\tmov al, 255\n");
							fprintf(output, "%s:\n", end);
							fprintf(output, "\tmov [temporaries+%u], al\n", *address);
						}
						else if (firstExprType == BYTE && *type == INT)
						{
							fprintf(output, "\tmov eax, 0\n");
							fprintf(output, "\tmov al, [temporaries+%u]\n", firstExprAdd);
							fprintf(output, "\tmov ebx, [temporaries+%u]\n", *address);
							fprintf(output, "\tcmp eax, ebx\n");
							fprintf(output, "\tjne %s\n", true);
							fprintf(output, "\tmov al, 0\n");
							fprintf(output, "\tjmp %s\n", end);
							fprintf(output, "%s:\n", true);
							fprintf(output, "\tmov al, 255\n");
							fprintf(output, "%s:\n", end);
							fprintf(output, "\tmov [temporaries+%u], al\n", *address);
						}
						else
						{
							fprintf(output, "\tmov eax, [temporaries+%u]\n", firstExprAdd);
							fprintf(output, "\tmov ebx, [temporaries+%u]\n", *address);
							fprintf(output, "\tcmp eax, ebx\n");
							fprintf(output, "\tjne %s\n", true);
							fprintf(output, "\tmov al, 0\n");
							fprintf(output, "\tjmp %s\n", end);
							fprintf(output, "%s:\n", true);
							fprintf(output, "\tmov al, 255\n");
							fprintf(output, "%s:\n", end);
							fprintf(output, "\tmov [temporaries+%u], al\n", *address);
						}
					}
					break;
				case MINOR:
					if (firstExprType == BYTE && *type == BYTE)
					{
						fprintf(output, "\tmov al, [temporaries+%u]\n", firstExprAdd);
						fprintf(output, "\tmov bl, [temporaries+%u]\n", *address);
						fprintf(output, "\tcmp al, bl\n");
						fprintf(output, "\tjl %s\n", true);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", true);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "%s:\n", end);
						fprintf(output, "\tmov [temporaries+%u], al\n", *address);
					}
					else if (firstExprType == INT && *type == BYTE)
					{
						fprintf(output, "\tmov eax, [temporaries+%u]\n", firstExprAdd);
						fprintf(output, "\tmov ebx, 0\n");
						fprintf(output, "\tmov bl, [temporaries+%u]\n", *address);
						fprintf(output, "\tcmp eax, ebx\n");
						fprintf(output, "\tjl %s\n", true);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", true);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "%s:\n", end);
						fprintf(output, "\tmov [temporaries+%u], al\n", *address);
					}
					else if (firstExprType == BYTE && *type == INT)
					{
						fprintf(output, "\tmov eax, 0\n");
						fprintf(output, "\tmov al, [temporaries+%u]\n", firstExprAdd);
						fprintf(output, "\tmov ebx, [temporaries+%u]\n", *address);
						fprintf(output, "\tcmp eax, ebx\n");
						fprintf(output, "\tjl %s\n", true);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", true);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "%s:\n", end);
						fprintf(output, "\tmov [temporaries+%u], al\n", *address);
					}
					else
					{
						fprintf(output, "\tmov eax, [temporaries+%u]\n", firstExprAdd);
						fprintf(output, "\tmov ebx, [temporaries+%u]\n", *address);
						fprintf(output, "\tcmp eax, ebx\n");
						fprintf(output, "\tjl %s\n", true);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", true);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "%s:\n", end);
						fprintf(output, "\tmov [temporaries+%u], al\n", *address);
					}
					break;
				case GREATER:
					if (firstExprType == BYTE && *type == BYTE)
					{
						fprintf(output, "\tmov al, [temporaries+%u]\n", firstExprAdd);
						fprintf(output, "\tmov bl, [temporaries+%u]\n", *address);
						fprintf(output, "\tcmp al, bl\n");
						fprintf(output, "\tjg %s\n", true);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", true);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "%s:\n", end);
						fprintf(output, "\tmov [temporaries+%u], al\n", *address);
					}
					else if (firstExprType == INT && *type == BYTE)
					{
						fprintf(output, "\tmov eax, [temporaries+%u]\n", firstExprAdd);
						fprintf(output, "\tmov ebx, 0\n");
						fprintf(output, "\tmov bl, [temporaries+%u]\n", *address);
						fprintf(output, "\tcmp eax, ebx\n");
						fprintf(output, "\tjg %s\n", true);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", true);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "%s:\n", end);
						fprintf(output, "\tmov [temporaries+%u], al\n", *address);
					}
					else if (firstExprType == BYTE && *type == INT)
					{
						fprintf(output, "\tmov eax, 0\n");
						fprintf(output, "\tmov al, [temporaries+%u]\n", firstExprAdd);
						fprintf(output, "\tmov ebx, [temporaries+%u]\n", *address);
						fprintf(output, "\tcmp eax, ebx\n");
						fprintf(output, "\tjg %s\n", true);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", true);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "%s:\n", end);
						fprintf(output, "\tmov [temporaries+%u], al\n", *address);
					}
					else
					{
						fprintf(output, "\tmov eax, [temporaries+%u]\n", firstExprAdd);
						fprintf(output, "\tmov ebx, [temporaries+%u]\n", *address);
						fprintf(output, "\tcmp eax, ebx\n");
						fprintf(output, "\tjg %s\n", true);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", true);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "%s:\n", end);
						fprintf(output, "\tmov [temporaries+%u], al\n", *address);
					}
					break;
				case MINEQ:
					if (firstExprType == BYTE && *type == BYTE)
					{
						fprintf(output, "\tmov al, [temporaries+%u]\n", firstExprAdd);
						fprintf(output, "\tmov bl, [temporaries+%u]\n", *address);
						fprintf(output, "\tcmp al, bl\n");
						fprintf(output, "\tjle %s\n", true);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", true);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "%s:\n", end);
						fprintf(output, "\tmov [temporaries+%u], al\n", *address);
					}
					else if (firstExprType == INT && *type == BYTE)
					{
						fprintf(output, "\tmov eax, [temporaries+%u]\n", firstExprAdd);
						fprintf(output, "\tmov ebx, 0\n");
						fprintf(output, "\tmov bl, [temporaries+%u]\n", *address);
						fprintf(output, "\tcmp eax, ebx\n");
						fprintf(output, "\tjle %s\n", true);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", true);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "%s:\n", end);
						fprintf(output, "\tmov [temporaries+%u], al\n", *address);
					}
					else if (firstExprType == BYTE && *type == INT)
					{
						fprintf(output, "\tmov eax, 0\n");
						fprintf(output, "\tmov al, [temporaries+%u]\n", firstExprAdd);
						fprintf(output, "\tmov ebx, [temporaries+%u]\n", *address);
						fprintf(output, "\tcmp eax, ebx\n");
						fprintf(output, "\tjle %s\n", true);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", true);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "%s:\n", end);
						fprintf(output, "\tmov [temporaries+%u], al\n", *address);
					}
					else
					{
						fprintf(output, "\tmov eax, [temporaries+%u]\n", firstExprAdd);
						fprintf(output, "\tmov ebx, [temporaries+%u]\n", *address);
						fprintf(output, "\tcmp eax, ebx\n");
						fprintf(output, "\tjle %s\n", true);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", true);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "%s:\n", end);
						fprintf(output, "\tmov [temporaries+%u], al\n", *address);
					}
					break;
				case GREATEQ:
					if (firstExprType == BYTE && *type == BYTE)
					{
						fprintf(output, "\tmov al, [temporaries+%u]\n", firstExprAdd);
						fprintf(output, "\tmov bl, [temporaries+%u]\n", *address);
						fprintf(output, "\tcmp al, bl\n");
						fprintf(output, "\tjge %s\n", true);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", true);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "%s:\n", end);
						fprintf(output, "\tmov [temporaries+%u], al\n", *address);
					}
					else if (firstExprType == INT && *type == BYTE)
					{
						fprintf(output, "\tmov eax, [temporaries+%u]\n", firstExprAdd);
						fprintf(output, "\tmov ebx, 0\n");
						fprintf(output, "\tmov bl, [temporaries+%u]\n", *address);
						fprintf(output, "\tcmp eax, ebx\n");
						fprintf(output, "\tjge %s\n", true);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", true);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "%s:\n", end);
						fprintf(output, "\tmov [temporaries+%u], al\n", *address);
					}
					else if (firstExprType == BYTE && *type == INT)
					{
						fprintf(output, "\tmov eax, 0\n");
						fprintf(output, "\tmov al, [temporaries+%u]\n", firstExprAdd);
						fprintf(output, "\tmov ebx, [temporaries+%u]\n", *address);
						fprintf(output, "\tcmp eax, ebx\n");
						fprintf(output, "\tjge %s\n", true);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", true);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "%s:\n", end);
						fprintf(output, "\tmov [temporaries+%u], al\n", *address);
					}
					else
					{
						fprintf(output, "\tmov eax, [temporaries+%u]\n", firstExprAdd);
						fprintf(output, "\tmov ebx, [temporaries+%u]\n", *address);
						fprintf(output, "\tcmp eax, ebx\n");
						fprintf(output, "\tjge %s\n", true);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", true);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "%s:\n", end);
						fprintf(output, "\tmov [temporaries+%u], al\n", *address);
					}
					break;
			}
		}

		*type = BOOL;
	}
}

void A(int* type, int* address)
{
#ifdef DEBUG
	printf("A ");
#endif
	int token = -1;
	if(reg.token == PLUS || reg.token == MINUS) {
		token = reg.token;
		matchToken(reg.token);
	}

	B(type, address);

	if ( (token == MINUS || token == PLUS) && (*type != INT && *type != BYTE) )
	{
		printf("%lu:%s.\n", line, incompatibleTypes);
		exit(INCOMPATIBLETYPES);
	}


	if (token == MINUS)
	{
		if (*type == INT)
		{
			fprintf(output, "\tmov eax, [temporaries+%u]\n", *address);
			fprintf(output, "\tneg eax\n");
			fprintf(output, "\tmov [temporaries+%u], eax\n", *address);
		}
		else if (*type == BYTE)
		{
			fprintf(output, "\tmov eax, 0\n");
			fprintf(output, "\tmov al, [temporaries+%u]\n", *address);
			fprintf(output, "\tneg eax\n");
			int newLocation = newTemporary(INT);
			fprintf(output, "\tmov [temporaries+%u], eax\n", newLocation);
			*address = newLocation;
		}
		*type = INT;
	}

	while(reg.token == PLUS || reg.token == MINUS || reg.token == OR)
	{
		token = reg.token;

		if (token == MINUS && (*type != BYTE && *type != INT))
		{
			printf("%lu:%s.\n", line, incompatibleTypes);
			exit(INCOMPATIBLETYPES);
		}
		else if (token == PLUS && *type == BOOL)
		{
			printf("%lu:%s.\n", line, incompatibleTypes);
			exit(INCOMPATIBLETYPES);
		}
		else if (token == OR && *type != BOOL)
		{
			printf("%lu:%s.\n", line, incompatibleTypes);
			exit(INCOMPATIBLETYPES);
		}

		int firstExprAdd = *address;
		int firstExprType = *type;

		matchToken(reg.token);
		B(type, address);

		if (token == MINUS && (*type != BYTE && *type != INT))
		{
			printf("%lu:%s.\n", line, incompatibleTypes);
			exit(INCOMPATIBLETYPES);
		}
		else if (token == PLUS && *type == BOOL)
		{
			printf("%lu:%s.\n", line, incompatibleTypes);
			exit(INCOMPATIBLETYPES);
		}
		else if (token == OR && *type != BOOL)
		{
			printf("%lu:%s.\n", line, incompatibleTypes);
			exit(INCOMPATIBLETYPES);
		}

		if ((firstExprType == BYTE || firstExprType == INT) && (*type != BYTE && *type != INT))
		{
			printf("%lu:%s.\n", line, incompatibleTypes);
			exit(INCOMPATIBLETYPES);
		}

		switch(token)
		{
			case PLUS:
				if (firstExprType == BYTE && *type == BYTE)
				{
					fprintf(output, "\tmov al, [temporaries+%u]\n", firstExprAdd);
					fprintf(output, "\tmov bl, [temporaries+%u]\n", *address);
					fprintf(output, "\tadd al, bl\n");
					fprintf(output, "\tmov [temporaries+%u], al\n", *address);
				}
				else if (firstExprType == INT && *type == BYTE)
				{
					fprintf(output, "\tmov eax, [temporaries+%u]\n", firstExprAdd);
					fprintf(output, "\tmov ebx, 0\n");
					fprintf(output, "\tmov bl, [temporaries+%u]\n", *address);
					fprintf(output, "\tadd eax, ebx\n");
					fprintf(output, "\tmov [temporaries+%u], eax\n", firstExprAdd);
					*type = INT;
					*address = firstExprAdd;
				}
				else if (firstExprType == BYTE && *type == INT)
				{
					fprintf(output, "\tmov eax, 0\n");
					fprintf(output, "\tmov al, [temporaries+%u]\n", firstExprAdd);
					fprintf(output, "\tmov ebx, [temporaries+%u]\n", *address);
					fprintf(output, "\tadd eax, ebx\n");
					fprintf(output, "\tmov [temporaries+%u], eax\n", *address);
				}
				else if (firstExprType == INT && *type == INT)
				{
					fprintf(output, "\tmov eax, [temporaries+%u]\n", firstExprAdd);
					fprintf(output, "\tmov ebx, [temporaries+%u]\n", *address);
					fprintf(output, "\tadd eax, ebx\n");
					fprintf(output, "\tmov [temporaries+%u], eax\n", *address);
				}
				else
				{
					char* loop = newLabel();
					char* concat = newLabel();
					fprintf(output, "\tmov ebx, temporaries+%u\n", firstExprAdd);
					fprintf(output, "\tmov edi, temporaries+%u\n", *address);
					fprintf(output, "\tadd ebx, -1\n");
					fprintf(output, "%s:\n", loop);
					fprintf(output, "\tadd ebx, 1\n");
					fprintf(output, "\tmov al, [ebx]\n");
					fprintf(output, "\tcmp al, 0x00\n");
					fprintf(output, "\tjne %s\n", loop);
					fprintf(output, "%s:\n", concat);
					fprintf(output, "\tmov al, [edi]\n");
					fprintf(output, "\tmov [ebx], al\n");
					fprintf(output, "\tadd ebx, 1\n");
					fprintf(output, "\tadd edi, 1\n");
					fprintf(output, "\tcmp al, 0x00\n");
					fprintf(output, "\tjne %s\n", concat);

					*address = firstExprAdd;
				}
				break;
			case MINUS:
				if (firstExprType == BYTE && *type == BYTE)
				{
					fprintf(output, "\tmov al, [temporaries+%u]\n", firstExprAdd);
					fprintf(output, "\tmov bl, [temporaries+%u]\n", *address);
					fprintf(output, "\tsub al, bl\n");
					fprintf(output, "\tmov [temporaries+%u], al\n", *address);
				}
				else if (firstExprType == INT && *type == BYTE)
				{
					fprintf(output, "\tmov eax, [temporaries+%u]\n", firstExprAdd);
					fprintf(output, "\tmov ebx, 0\n");
					fprintf(output, "\tmov bl, [temporaries+%u]\n", *address);
					fprintf(output, "\tsub eax, ebx\n");
					fprintf(output, "\tmov [temporaries+%u], eax\n", firstExprAdd);
					*type = INT;
					*address = firstExprAdd;
				}
				else if (firstExprType == BYTE && *type == INT)
				{
					fprintf(output, "\tmov eax, 0\n");
					fprintf(output, "\tmov al, [temporaries+%u]\n", firstExprAdd);
					fprintf(output, "\tmov ebx, [temporaries+%u]\n", *address);
					fprintf(output, "\tsub eax, ebx\n");
					fprintf(output, "\tmov [temporaries+%u], eax\n", *address);
				}
				else if (firstExprType == INT && *type == INT)
				{
					fprintf(output, "\tmov eax, [temporaries+%u]\n", firstExprAdd);
					fprintf(output, "\tmov ebx, [temporaries+%u]\n", *address);
					fprintf(output, "\tsub eax, ebx\n");
					fprintf(output, "\tmov [temporaries+%u], eax\n", *address);
				}
				break;
			case OR:
				fprintf(output, "\tmov al, [temporaries+%u]\n", firstExprAdd);
				fprintf(output, "\tmov bl, [temporaries+%u]\n", *address);
				fprintf(output, "\tor al, bl\n");
				fprintf(output, "\tmov [temporaries+%u], al\n", *address);
				break;
		}
	}
}

void B(int* type, int* address)
{
#ifdef DEBUG
	printf("B ");
#endif
	C(type, address);

	while(reg.token == MULT || reg.token == DIVIDE || reg.token == AND)
	{
		int operation = reg.token;
		int firstExprAdd = *address;
		int firstExprType = *type;
		if ( ( (operation == MULT || operation == DIVIDE) && (*type == STR || *type == BOOL) )
		    || (operation == AND && *type != BOOL) ) 
		{
			printf("%lu:%s.\n", line, incompatibleTypes);
			exit(INCOMPATIBLETYPES);
		}

		matchToken(reg.token);

		C(type, address);

		if ( ( (operation == MULT || operation == DIVIDE) && (*type == STR || *type == BOOL) )
		    || (operation == AND && *type != BOOL) ) 
		{
			printf("%lu:%s.\n", line, incompatibleTypes);
			exit(INCOMPATIBLETYPES);
		}
		else
		{
			switch(operation)
			{
				case MULT:
					if (firstExprType == BYTE && *type == BYTE)
					{
						fprintf(output, "\tmov al, [temporaries+%u]\n", firstExprAdd);
						fprintf(output, "\tmov bl, [temporaries+%u]\n", *address);
						fprintf(output, "\tmul bl\n");
						fprintf(output, "\tmov [temporaries+%u], al\n", *address);
					}
					else if (firstExprType == INT && *type == INT)
					{
						fprintf(output, "\tmov eax, [temporaries+%u]\n", firstExprAdd);
						fprintf(output, "\tmov ebx, [temporaries+%u]\n", *address);
						fprintf(output, "\timul ebx\n");
						fprintf(output, "\tmov [temporaries+%u], eax\n", *address);
					}
					else if (firstExprType == INT && *type == BYTE)
					{
						fprintf(output, "\tmov eax, [temporaries+%u]\n", firstExprAdd);
						fprintf(output, "\tmov ebx, 0\n");
						fprintf(output, "\tmov bl, [temporaries+%u]\n", *address);
						fprintf(output, "\timul ebx\n");
						fprintf(output, "\tmov [temporaries+%u], eax\n", firstExprAdd);
						*type = INT;
						*address = firstExprAdd;
					}
					else
					{
						fprintf(output, "\tmov eax, 0\n");
						fprintf(output, "\tmov al, [temporaries+%u]\n", firstExprAdd);
						fprintf(output, "\tmov ebx, [temporaries+%u]\n", *address);
						fprintf(output, "\timul ebx\n");
						fprintf(output, "\tmov [temporaries+%u], eax\n", *address);
					}
					break;
				case DIVIDE:
					if (firstExprType == BYTE && *type == BYTE)
					{
						fprintf(output, "\tmov al, [temporaries+%u]\n", firstExprAdd);
						fprintf(output, "\tmov bl, [temporaries+%u]\n", *address);
						fprintf(output, "\tmov ah, 0\n");
						fprintf(output, "\tdiv bl\n");
						fprintf(output, "\tmov [temporaries+%u], al\n", *address);
					}
					else if (firstExprType == INT && *type == INT)
					{
						fprintf(output, "\tmov eax, [temporaries+%u]\n", firstExprAdd);
						fprintf(output, "\tmov ebx, [temporaries+%u]\n", *address);
						fprintf(output, "\tmov edx, 0\n");
						fprintf(output, "\tdiv ebx\n");
						fprintf(output, "\tmov [temporaries+%u], eax\n", *address);
					}
					else if (firstExprType == INT && *type == BYTE)
					{
						fprintf(output, "\tmov eax, [temporaries+%u]\n", firstExprAdd);
						fprintf(output, "\tmov ebx, 0\n");
						fprintf(output, "\tmov bl, [temporaries+%u]\n", *address);
						fprintf(output, "\tmov edx, 0\n");
						fprintf(output, "\tidiv ebx\n");
						fprintf(output, "\tmov [temporaries+%u], eax\n", firstExprAdd);
						*type = INT;
						*address = firstExprAdd;
					}
					else
					{
						fprintf(output, "\tmov eax, 0\n");
						fprintf(output, "\tmov al, [temporaries+%u]\n", firstExprAdd);
						fprintf(output, "\tmov ebx, [temporaries+%u]\n", *address);
						fprintf(output, "\tmov edx, 0\n");
						fprintf(output, "\tidiv ebx\n");
						fprintf(output, "\tmov [temporaries+%u], eax\n", *address);
					}
					break;
				case AND:
					fprintf(output, "\tmov al, [temporaries+%u]\n", firstExprAdd);
					fprintf(output, "\tmov bl, [temporaries+%u]\n", *address);
					fprintf(output, "\tand al, bl\n");
					fprintf(output, "\tmov [temporaries+%u], al\n", *address);
					break;
			}
		}
	}
}

void C(int* type, int* address)
{
	char not = 0;
#ifdef DEBUG
	printf("C ");
#endif
	if(reg.token == NOT)
	{
		not = 1;
		matchToken(reg.token);
	}
	D(type, address);

	if (not && *type != BOOL)
	{
		printf("%lu:%s.\n", line, incompatibleTypes);
		exit(INCOMPATIBLETYPES);
	}
	else if (not)
	{
		fprintf(output, "\tmov al, [temporaries+%u]\n", *address);
		fprintf(output, "\nnot al\n");
		fprintf(output, "\tmov [temporaries+%u], al\n", *address);
	}
}

void D(int* type, int* address)
{
#ifdef DEBUG
	printf("D ");
#endif
	if(reg.token == LEFTPAR)
	{
		matchToken(LEFTPAR);
		expr(type, address);
		matchToken(RIGHTPAR);
	}
	else if(reg.token == LITERAL)
	{
		*type = reg.type;
		*address = newTemporary(*type);

		if (*type == INT) {
			fprintf(output, "\tmov eax, %u\n", atoi(reg.lexem));
			fprintf(output, "\tmov [temporaries+%u], eax\n", *address);
		} else if (*type == STR) {
			fprintf(output, "\tmov ebx, temporaries+%u\n", *address);
			for (int i = 1; i < strlen(reg.lexem)-1; i++) {
				fprintf(output, "\tmov al, %u\n", reg.lexem[i]);
				fprintf(output, "\tmov [ebx], al\n");
				fprintf(output, "\tadd ebx, 1\n");
			}
			fprintf(output, "\tmov al, 0x00\n");
			fprintf(output, "\tmov [ebx], al\n");
		} else if (*type == BYTE) {
			fprintf(output, "\tmov al, %u\n", atoi(reg.lexem));
			fprintf(output, "\tmov [temporaries+%u], al\n", *address);
		} else {
			fprintf(output, "\tmov al, %u\n", (strncmp(reg.lexem, "TRUE", 4) == 0 ? 0xFF : 0x00));
			fprintf(output, "\tmov [temporaries+%u], al\n", *address);
		}

		matchToken(LITERAL);
	}
	else
	{
		if (reg.entry->class == NULL) {
			printf("%lu:%s.\n", line, undeclaredIdentifier);
			exit(UNDECLAREDIDENTIFIER);
		}

		*type = reg.entry->type;
		char* varLabel = reg.entry->lexem;

		int temporary = newTemporary(*type);

		if (*type == BYTE || *type == BOOL)
		{
			fprintf(output, "\tmov al, [%s]\n", varLabel);
			fprintf(output, "\tmov [temporaries+%u], al\n", temporary);
		}
		else if (*type == INT)
		{
			fprintf(output, "\tmov eax, [%s]\n", varLabel);
			fprintf(output, "\tmov [temporaries+%u], eax\n", temporary);
		}
		else if (*type == STR)
		{
			char* end = newLabel();
			char* loop = newLabel();
			fprintf(output, "\tmov eax, temporaries+%u\n", temporary);
			fprintf(output, "\tmov ebx, %s\n", varLabel);
			fprintf(output, "%s:\n", loop);
			fprintf(output, "\tmov cl, [ebx]\n");
			fprintf(output, "\tcmp cl, 0x00\n");
			fprintf(output, "\tje %s\n", end);
			fprintf(output, "\tmov [eax], cl\n");
			fprintf(output, "\tadd eax, 1\n");
			fprintf(output, "\tadd ebx, 1\n");
			fprintf(output, "\tjmp %s\n", loop);
			fprintf(output, "%s:\n", end);
			fprintf(output, "\tmov [eax], cl\n");
		}

		*address = temporary;

		matchToken(ID);
	}
}

void matchToken(char unsigned expected)
{
	if(reg.token == expected)
	{
		lex();
	}
	else
	{
		printf("%lu:%s [%s].\n", line, unexpectedToken, reg.lexem);
		exit(UNEXPECTEDTOKEN);
	}
}

char* newLabel() {
	int digits = label == 0 ? 1 : floor(log10(abs(label))) + 1;
	char* lbl = malloc(sizeof(char) * (digits + 2));
	sprintf(lbl, "Lbl%d", label);
	label++;
	return lbl;
}

int newTemporary(int type)
{
	int temporaries = stack;

	if (type == BOOL || type == BYTE)
		stack++;
	else if (type == INT)
		stack += 4;
	else if (type == STR)
		stack += 65536;

	return temporaries;
}