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

	fprintf(output, "sseg SEGMENT STACK\n\tbyte 4000h DUP(?)\nsseg ENDS\n\ndseg SEGMENT PUBLIC\n\tbyte 4000h DUP(?)\n");
	while (reg.token != BEGIN)
	{
		declaration(&address);
	}
	fprintf(output, "dseg ENDS\n\ncseg SEGMENT PUBLIC\n\tASSUME CS:cseg, DS:dseg\n\nstrt:\n");
	fprintf(output,  "%s", attribuitions);

	block();
	fprintf(output, "\tmov ah, 4Ch\n\tint 21h\ncseg ENDS\nEND strt");
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
			printf("%lu:identificador ja declarado %s.\n", line, reg.lexem);
			exit(ALREADYDECLARED);
		}

		reg.entry->address = *address;

		switch(type)
		{
			case STRINGTOK:
				reg.entry->type=STR;
				type = STR;
				fprintf(output, "\tbyte 100h DUP(?)\n");
				*address = *address + 0x100;
				break;
			case INTTOK:
				reg.entry->type=INT;
				type=INT;
				fprintf(output, "\tsword ?\n");
				*address = *address + 2;
				break;
			case BYTETOK:
				reg.entry->type=BYTE;
				type=BYTE;
				fprintf(output, "\tbyte ?\n");
				*address = *address + 1;
				break;
			default:
				reg.entry->type=BOOL;
				type=BOOL;
				fprintf(output, "\tbyte ?\n");
				*address = *address + 1;
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
				if (reg.type != BYTE && reg.token != INT) {
					printf("%lu:tipos incompatíveis.\n", line);
					exit(INCOMPATIBLETYPES);
				}
				if (reg.type == BYTE && signal == MINUS) {
					reg.type = INT;
					type = INT;
				}
			}

			if (!(entry->type == reg.type || (entry->type == INT && reg.type == BYTE))) {
				printf("%lu:tipos incompatíveis.\n", line);
				exit(INCOMPATIBLETYPES);
			}

			if (type == STR) {
				sprintf(attribuitions, "%s\tmov bx, %u\n", attribuitions, entry->address);
				for (int i = 1; i < strlen(reg.lexem)-1; i++) {
					sprintf(attribuitions, "%s\tmov al, %u\n", attribuitions, reg.lexem[i]);
					sprintf(attribuitions, "%s\tmov DS:[bx], al\n", attribuitions);
					sprintf(attribuitions, "%s\tadd bx, 1\n", attribuitions);
				}
				sprintf(attribuitions, "%s\tmov al, 24h\n", attribuitions);
				sprintf(attribuitions, "%s\tmov DS:[bx], al\n", attribuitions);
			} else if (type == INT) {
				sprintf(attribuitions, "%s\tmov ax, %s%d\n\tmov DS:[%u], ax\n", attribuitions, (signal == MINUS ? "-" : "+"),  atoi(reg.lexem), entry->address);
			} else if (type == BYTE) {
				sprintf(attribuitions, "%s\tmov al, %d\n\tmov DS:[%u], al\n", attribuitions, atoi(reg.lexem), entry->address);
			} else if (type == BOOL) {
				sprintf(attribuitions, "%s\tmov al, %d\n\tmov DS:[%u], al\n", attribuitions, strncmp(reg.lexem,"TRUE", 4) == 0 ? 0xFF : 0x00, entry->address);
			}

			matchToken(LITERAL);
		}

		while(reg.token == COMMA)
		{
			matchToken(COMMA);

			if (reg.entry->class == NULL) {
				reg.entry->class=VAR;
			} else {
				printf("%lu:identificador ja declarado %s.\n", line, reg.lexem);
				exit(ALREADYDECLARED);
			}

			reg.entry->address = *address;
			reg.entry->type = type;

			switch(type)
			{
				case STR:
					fprintf(output, "\tbyte 100h DUP(?)\n");
					*address = *address + 0x100;
					break;
				case INT:
					fprintf(output, "\tsword ?\n");
					*address = *address + 2;
					break;
				default:
					fprintf(output, "\tbyte ?\n");
					*address = *address + 1;
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
						printf("%lu:tipos incompatíveis.\n", line);
						exit(INCOMPATIBLETYPES);
					}
					if (reg.type == BYTE && signal == MINUS) {
						reg.type = INT;
						type = INT;
					}
				}

				if (!(entry->type == reg.type || (entry->type == INT && reg.type == BYTE))) {
					printf("%lu:tipos incompatíveis.\n", line);
					exit(INCOMPATIBLETYPES);
				}

				if (type == STR) {
					sprintf(attribuitions, "%s\tmov bx, %u\n", attribuitions, entry->address);
					for (int i = 1; i < strlen(reg.lexem)-1; i++) {
						sprintf(attribuitions, "%s\tmov al, %u\n", attribuitions, reg.lexem[i]);
						sprintf(attribuitions, "%s\tmov DS:[bx], al\n", attribuitions);
						sprintf(attribuitions, "%s\tadd bx, 1\n", attribuitions);
					}
					sprintf(attribuitions, "%s\tmov al, 24h\n", attribuitions);
					sprintf(attribuitions, "%s\tmov DS:[bx], al\n", attribuitions);
				} else if (type == INT) {
					sprintf(attribuitions, "%s\tmov ax, %s%d\n\tmov DS:[%u], ax\n", attribuitions, (signal == MINUS ? "-" : "+"),  atoi(reg.lexem), entry->address);
				} else if (type == BYTE) {
					sprintf(attribuitions, "%s\tmov al, %d\n\tmov DS:[%u], al\n", attribuitions, atoi(reg.lexem), entry->address);
				} else if (type == BOOL) {
					sprintf(attribuitions, "%s\tmov al, %d\n\tmov DS:[%u], al\n", attribuitions, strncmp(reg.lexem,"TRUE", 4) == 0 ? 0xFF : 0x00, entry->address);
				}

				matchToken(LITERAL);
			}
		}

		matchToken(SEMICOLON);
	}
	else
	{
		matchToken(FINAL);

		if (reg.entry->class == NULL) {
			reg.entry->class = CONST;
		} else {
			printf("%lu:identificador ja declarado %s.\n", line, reg.lexem);
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
				printf("%lu:tipos incompatíveis.\n", line);
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
				reg.lexem[strlen(reg.lexem)-1] = '$';
				fprintf(output, "\tbyte %s\"\n", reg.lexem);
				*address = *address + strlen(reg.lexem);
				break;
			case BYTE:
				fprintf(output, "\tbyte %u\n", atoi(reg.lexem));
				*address = *address + 1;
				break;
			case BOOL:
				fprintf(output, "\tbyte %u\n", (strncmp(reg.lexem, "TRUE", 4) == 0 ? 0xFF : 0x00));
				*address = *address + 1;
				break;
			default:
				fprintf(output, "\tsword %s%d\n", (signal == MINUS ? "-" : "+"), atoi(reg.lexem));
				*address = *address + 2;
				break;
		}

		matchToken(LITERAL);
		matchToken(SEMICOLON);
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
			printf("%lu:identificador nao declarado %s.\n", line, reg.lexem);
			exit(UNDECLAREDIDENTIFIER);
		} else if (reg.entry->class == CONST) {
			printf("%lu:classe de identificador incompativel %s.\n", line, reg.lexem);
			exit(UNDECLAREDIDENTIFIER);
		}

		Entry* entry = reg.entry;

		matchToken(ID);
		matchToken(ATTR);
		int tmpStack = stack;
		expr(&type, &address);

		if (!(entry->type == type || (entry->type == INT && type == BYTE))) {
			printf("%lu:tipos incompatíveis.\n", line);
			exit(INCOMPATIBLETYPES);
		}

		if (entry->type == INT) {
			if (type == INT) 
			{
				fprintf(output, "\tmov al, DS:[%u]\n", address);
				fprintf(output, "\tmov ah, DS:[%u]\n", address+1);
				fprintf(output, "\tmov DS:[%u], al\n", entry->address);
				fprintf(output, "\tmov DS:[%u], ah\n", entry->address+1);
			}
			else
			{
				fprintf(output, "\tmov al, DS:[%u]\n", address);
				fprintf(output, "\tmov ah, 0\n");
				fprintf(output, "\tmov DS:[%u], al\n", entry->address);
				fprintf(output, "\tmov DS:[%u], ah\n", entry->address+1);
			}
		} else if (entry->type == STR) {
			fprintf(output, "\tmov bx, %u\n", address);
			fprintf(output, "\tmov di, %u\n", entry->address);
			char* loop = newLabel();
			char* end = newLabel();
			fprintf(output, "%s:\n", loop);
			fprintf(output, "\tmov cl, DS:[bx]\n");
			fprintf(output, "\tcmp cl, 24h\n");
			fprintf(output, "\tje %s\n", end);
			fprintf(output, "\tmov DS:[di], cl\n");
			fprintf(output, "\tadd di, 1\n");
			fprintf(output, "\tadd bx, 1\n");
			fprintf(output, "\tjmp %s\n", loop);
			fprintf(output, "%s:\n", end);
			fprintf(output, "\tmov DS:[di], cl\n");
		} else {
			fprintf(output, "\tmov al, DS:[%u]\n\tmov DS:[%u], al\n", address, entry->address);
		}

		matchToken(SEMICOLON);

		stack = tmpStack;

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

		int tmpStack = stack;

		expr(&type, &address);
		if (type != BOOL)
		{
			printf("%lu:tipos incompatíveis.\n", line);
			exit(INCOMPATIBLETYPES);
		}

		fprintf(output, "\tmov al, DS:[%u]\n", address);
		fprintf(output, "\tmov ah, 0\n");
		fprintf(output, "\tcmp ax, 0\n");
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
		stack = tmpStack;
	}
	else if(reg.token == IF)
	{
		char* false = newLabel();
		char* end = newLabel();
		matchToken(IF);
		expr(&type, &address);

		if (type != BOOL)
		{
			printf("%lu:tipos incompatíveis.\n", line);
			exit(INCOMPATIBLETYPES);
		}

		fprintf(output, "\tmov al, DS:[%u]\n", address);
		fprintf(output, "\tmov ah, 0\n");
		fprintf(output, "\tcmp ax, 0\n");
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
	else if(reg.token == SEMICOLON)
	{
		matchToken(SEMICOLON);
#ifdef DEBUG
	printf("\n");
#endif
	}
	else if(reg.token == READLN)
	{
		matchToken(READLN);\

		int buffer = newTemporary(BUFFER);

		fprintf(output, "\tmov dx, %u\n", buffer);
		fprintf(output, "\tmov al, 0FFh\n");
		fprintf(output, "\tmov DS:[%u], al\n", buffer);
		fprintf(output, "\tmov ah, 0Ah\n");
		fprintf(output, "\tint 21h\n");
		fprintf(output, "\tmov ah, 02h\n");
		fprintf(output, "\tmov dl, 0Dh\n");
		fprintf(output, "\tint 21h\n");
		fprintf(output, "\tmov dl, 0Ah\n");
		fprintf(output, "\tint 21h\n");

		if (reg.type == STR)
		{
			char* end = newLabel();
			char* loop = newLabel();
			fprintf(output, "\tmov di, %u\n", reg.entry->address);
			fprintf(output, "\tmov bx, %u\n", buffer+2);
			fprintf(output, "%s:\n", loop);
			fprintf(output, "\tmov al, DS:[bx]\n");
			fprintf(output, "\tcmp al, 0Dh\n");
			fprintf(output, "\tje %s\n", end);
			fprintf(output, "\tmov DS:[di], al\n");
			fprintf(output, "\tadd di, 1\n");
			fprintf(output, "\tadd bx, 1\n");
			fprintf(output, "\tjmp %s\n", loop);
			fprintf(output, "%s:\n", end);
			fprintf(output, "\tmov al, 24h\n");
			fprintf(output, "\tmov DS:[di], al\n");
		}
		else if (reg.type == BYTE)
		{
			char* loop = newLabel();
			char* end = newLabel();
			fprintf(output, "\tmov di, %u\n", buffer+2);
			fprintf(output, "\tmov al, 0\n");
			fprintf(output, "\tmov cl, 10\n");
			fprintf(output, "%s:\n", loop);
			fprintf(output, "\tmov bl, DS:[di]\n");
			fprintf(output, "\tcmp bl, 0Dh\n");
			fprintf(output, "\tje %s\n", end);
			fprintf(output, "\tmul cl\n");
			fprintf(output, "\tadd bl, -48\n");
			fprintf(output, "\tadd al, bl\n");
			fprintf(output, "\tadd di, 1\n");
			fprintf(output, "\tjmp %s\n", loop);
			fprintf(output, "%s:\n", end);
			fprintf(output, "\tmov DS:[%u], al\n", reg.entry->address);
		}
		else if (reg.type == INT)
		{
			char* loop = newLabel();
			char* end = newLabel();
			char* positive = newLabel();
			fprintf(output, "\tmov di, %u\n", buffer+2);
			fprintf(output, "\tmov ax, 0\n");
			fprintf(output, "\tmov cx, 10\n");
			fprintf(output, "\tmov dx, 1\n");
			fprintf(output, "\tmov bl, DS:[di]\n");
			fprintf(output, "\tcmp bl, 2Dh\n");
			fprintf(output, "\tjne %s\n", positive);
			fprintf(output, "\tmov dx, -1\n");
			fprintf(output, "\tadd di, 1\n");
			fprintf(output, "\tmov bl, DS:[di]\n");
			fprintf(output, "%s:\n", positive);
			fprintf(output, "\tpush dx\n");
			fprintf(output, "\tmov dx, 0\n");
			fprintf(output, "%s:\n", loop);
			fprintf(output, "\tmov bl, DS:[di]\n");
			fprintf(output, "\tcmp bl, 0Dh\n");
			fprintf(output, "\tje %s\n", end);
			fprintf(output, "\timul cx\n");
			fprintf(output, "\tadd bl, -48\n");
			fprintf(output, "\tmov bh, 0\n");
			fprintf(output, "\tadd ax, bx\n");
			fprintf(output, "\tadd di, 1\n");
			fprintf(output, "\tjmp %s\n", loop);
			fprintf(output, "%s:\n", end);
			fprintf(output, "\tpop cx\n");
			fprintf(output, "\timul cx\n");
			fprintf(output, "\tmov DS:[%u], al\n", reg.entry->address);
			fprintf(output, "\tmov DS:[%u], ah\n", reg.entry->address+1);
		}

		matchToken(ID);
		matchToken(SEMICOLON);
#ifdef DEBUG
	printf("\n");
#endif
	}
	else if (reg.token == WRITE)
	{
		matchToken(WRITE);
		int tmpStack = stack;
		expr(&type, &address);

		if (type == STR) {
			fprintf(output, "\tmov dx, %u\n", address);
			fprintf(output, "\tmov ah, 09h\n");
			fprintf(output, "\tint 21h\n");
		} else if (type == INT || type == BYTE) {
			int tmpString = newTemporary(STR);
			char* positive = newLabel();
			char* digits = newLabel();
			char* printing = newLabel();
			fprintf(output, "\tmov di, %u\n", tmpString);
			fprintf(output, "\tmov al, DS:[%u]\n", address);

			if (type == INT)
				fprintf(output, "\tmov ah, DS:[%u]\n", address+1);
			else
				fprintf(output, "\tmov ah, 0\n");

			fprintf(output, "\tmov cx, 0\n");
			fprintf(output, "\tcmp ax, 0\n");
			fprintf(output, "\tjge %s\n", positive);
			fprintf(output, "\tmov bl, 2Dh\n");
			fprintf(output, "\tmov DS:[di], bl\n");
			fprintf(output, "\tadd di, 1\n");
			fprintf(output, "\tneg ax\n");
			fprintf(output, "%s:\n", positive);
			fprintf(output, "\tmov bx, 10\n");
			fprintf(output, "%s:\n", digits);
			fprintf(output, "\tadd cx, 1\n");
			fprintf(output, "\tmov dx, 0\n");
			fprintf(output, "\tidiv bx\n");
			fprintf(output, "\tpush dx\n");
			fprintf(output, "\tcmp ax, 0\n");
			fprintf(output, "\tjne %s\n", digits);
			fprintf(output, "%s:\n", printing);
			fprintf(output, "\tpop dx\n");
			fprintf(output, "\tadd dx, 30h\n");
			fprintf(output, "\tmov DS:[di], dl\n");
			fprintf(output, "\tadd di, 1\n");
			fprintf(output, "\tadd cx, -1\n");
			fprintf(output,	"\tcmp cx, 0\n");
			fprintf(output, "\tjne %s\n", printing);
			fprintf(output, "\tmov dl, 24h\n");
			fprintf(output, "\tmov DS:[di], dl\n");
			fprintf(output, "\tmov dx, %u\n", tmpString);
			fprintf(output, "\tmov ah, 09h\n");
			fprintf(output, "\tint 21h\n");
		} else {
			char* true = newLabel();
			char* print = newLabel();
			int tmpOut = newTemporary(STR);
			fprintf(output, "\tmov al, DS:[%u]\n", address);
			fprintf(output, "\tcmp al, 0\n");
			fprintf(output, "\tjne %s\n", true);
			fprintf(output, "\tmov di, %u\n", tmpOut);
			fprintf(output, "\tmov ax, 70\n");
			fprintf(output, "\tmov DS:[di], ax\n");
			fprintf(output, "\tadd di, 1\n");
			fprintf(output, "\tmov ax, 65\n");
			fprintf(output, "\tmov DS:[di], ax\n");
			fprintf(output, "\tadd di, 1\n");
			fprintf(output, "\tmov ax, 76\n");
			fprintf(output, "\tmov DS:[di], ax\n");
			fprintf(output, "\tadd di, 1\n");
			fprintf(output, "\tmov ax, 83\n");
			fprintf(output, "\tmov DS:[di], ax\n");
			fprintf(output, "\tadd di, 1\n");
			fprintf(output, "\tmov ax, 69\n");
			fprintf(output, "\tmov DS:[di], ax\n");
			fprintf(output, "\tadd di, 1\n");
			fprintf(output, "\tmov ax, 36\n");
			fprintf(output, "\tmov DS:[di], ax\n");
			fprintf(output, "\tjmp %s\n", print);
			fprintf(output, "%s:\n", true);
			fprintf(output, "\tmov di, %u\n", tmpOut);
			fprintf(output, "\tmov ax, 84\n");
			fprintf(output, "\tmov DS:[di], ax\n");
			fprintf(output, "\tadd di, 1\n");
			fprintf(output, "\tmov ax, 82\n");
			fprintf(output, "\tmov DS:[di], ax\n");
			fprintf(output, "\tadd di, 1\n");
			fprintf(output, "\tmov ax, 85\n");
			fprintf(output, "\tmov DS:[di], ax\n");
			fprintf(output, "\tadd di, 1\n");
			fprintf(output, "\tmov ax, 69\n");
			fprintf(output, "\tmov DS:[di], ax\n");
			fprintf(output, "\tadd di, 1\n");
			fprintf(output, "\tmov ax, 36\n");
			fprintf(output, "\tmov DS:[di], ax\n");
			fprintf(output, "%s:\n", print);
			fprintf(output, "\tmov dx, %u\n", tmpOut);
			fprintf(output, "\tmov ah, 09h\n");
			fprintf(output, "\tint 21h\n");
		}

		while (reg.token == COMMA)
		{
			matchToken(COMMA);
			expr(&type, &address);

			if (type == STR) {
				fprintf(output, "\tmov dx, %u\n", address);
				fprintf(output, "\tmov ah, 09h\n");
				fprintf(output, "\tint 21h\n");
			} else if (type == INT || type == BYTE) {
				int tmpString = newTemporary(STR);
				char* positive = newLabel();
				char* digits = newLabel();
				char* printing = newLabel();
				fprintf(output, "\tmov di, %u\n", tmpString);
				fprintf(output, "\tmov al, DS:[%u]\n", address);

				if (type == INT)
					fprintf(output, "\tmov ah, DS:[%u]\n", address+1);
				else
					fprintf(output, "\tmov ah, 0\n");

				fprintf(output, "\tmov cx, 0\n");
				fprintf(output, "\tcmp ax, 0\n");
				fprintf(output, "\tjge %s\n", positive);
				fprintf(output, "\tmov bl, 2Dh\n");
				fprintf(output, "\tmov DS:[di], bl\n");
				fprintf(output, "\tadd di, 1\n");
				fprintf(output, "\tneg ax\n");
				fprintf(output, "%s:\n", positive);
				fprintf(output, "\tmov bx, 10\n");
				fprintf(output, "%s:\n", digits);
				fprintf(output, "\tadd cx, 1\n");
				fprintf(output, "\tmov dx, 0\n");
				fprintf(output, "\tidiv bx\n");
				fprintf(output, "\tpush dx\n");
				fprintf(output, "\tcmp ax, 0\n");
				fprintf(output, "\tjne %s\n", digits);
				fprintf(output, "%s:\n", printing);
				fprintf(output, "\tpop dx\n");
				fprintf(output, "\tadd dx, 30h\n");
				fprintf(output, "\tmov DS:[di], dl\n");
				fprintf(output, "\tadd di, 1\n");
				fprintf(output, "\tadd cx, -1\n");
				fprintf(output,	"\tcmp cx, 0\n");
				fprintf(output, "\tjne %s\n", printing);
				fprintf(output, "\tmov dl, 24h\n");
				fprintf(output, "\tmov DS:[di], dl\n");
				fprintf(output, "\tmov dx, %u\n", tmpString);
				fprintf(output, "\tmov ah, 09h\n");
				fprintf(output, "\tint 21h\n");
			} else {
				char* true = newLabel();
				char* print = newLabel();
				int tmpOut = newTemporary(STR);
				fprintf(output, "\tmov al, DS:[%u]\n", address);
				fprintf(output, "\tcmp al, 0\n");
				fprintf(output, "\tjne %s\n", true);
				fprintf(output, "\tmov di, %u\n", tmpOut);
				fprintf(output, "\tmov ax, 70\n");
				fprintf(output, "\tmov DS:[di], ax\n");
				fprintf(output, "\tadd di, 1\n");
				fprintf(output, "\tmov ax, 65\n");
				fprintf(output, "\tmov DS:[di], ax\n");
				fprintf(output, "\tadd di, 1\n");
				fprintf(output, "\tmov ax, 76\n");
				fprintf(output, "\tmov DS:[di], ax\n");
				fprintf(output, "\tadd di, 1\n");
				fprintf(output, "\tmov ax, 83\n");
				fprintf(output, "\tmov DS:[di], ax\n");
				fprintf(output, "\tadd di, 1\n");
				fprintf(output, "\tmov ax, 69\n");
				fprintf(output, "\tmov DS:[di], ax\n");
				fprintf(output, "\tadd di, 1\n");
				fprintf(output, "\tmov ax, 36\n");
				fprintf(output, "\tmov DS:[di], ax\n");
				fprintf(output, "\tjmp %s\n", print);
				fprintf(output, "%s:\n", true);
				fprintf(output, "\tmov di, %u\n", tmpOut);
				fprintf(output, "\tmov ax, 84\n");
				fprintf(output, "\tmov DS:[di], ax\n");
				fprintf(output, "\tadd di, 1\n");
				fprintf(output, "\tmov ax, 82\n");
				fprintf(output, "\tmov DS:[di], ax\n");
				fprintf(output, "\tadd di, 1\n");
				fprintf(output, "\tmov ax, 85\n");
				fprintf(output, "\tmov DS:[di], ax\n");
				fprintf(output, "\tadd di, 1\n");
				fprintf(output, "\tmov ax, 69\n");
				fprintf(output, "\tmov DS:[di], ax\n");
				fprintf(output, "\tadd di, 1\n");
				fprintf(output, "\tmov ax, 36\n");
				fprintf(output, "\tmov DS:[di], ax\n");
				fprintf(output, "%s:\n", print);
				fprintf(output, "\tmov dx, %u\n", tmpOut);
				fprintf(output, "\tmov ah, 09h\n");
				fprintf(output, "\tint 21h\n");
			}
		}

		matchToken(SEMICOLON);

		stack = tmpStack;
#ifdef DEBUG
	printf("\n");
#endif
	}
	else
	{
		matchToken(WRITELN);

		int tmpStack = stack;

		expr(&type, &address);

		if (type == STR) {
			fprintf(output, "\tmov dx, %u\n", address);
			fprintf(output, "\tmov ah, 09h\n");
			fprintf(output, "\tint 21h\n");
		} else if (type == INT || type == BYTE) {
			int tmpString = newTemporary(STR);
			char* positive = newLabel();
			char* digits = newLabel();
			char* printing = newLabel();
			fprintf(output, "\tmov di, %u\n", tmpString);
			fprintf(output, "\tmov al, DS:[%u]\n", address);

			if (type == INT)
				fprintf(output, "\tmov ah, DS:[%u]\n", address+1);
			else
				fprintf(output, "\tmov ah, 0\n");

			fprintf(output, "\tmov cx, 0\n");
			fprintf(output, "\tcmp ax, 0\n");
			fprintf(output, "\tjge %s\n", positive);
			fprintf(output, "\tmov bl, 2Dh\n");
			fprintf(output, "\tmov DS:[di], bl\n");
			fprintf(output, "\tadd di, 1\n");
			fprintf(output, "\tneg ax\n");
			fprintf(output, "%s:\n", positive);
			fprintf(output, "\tmov bx, 10\n");
			fprintf(output, "%s:\n", digits);
			fprintf(output, "\tadd cx, 1\n");
			fprintf(output, "\tmov dx, 0\n");
			fprintf(output, "\tidiv bx\n");
			fprintf(output, "\tpush dx\n");
			fprintf(output, "\tcmp ax, 0\n");
			fprintf(output, "\tjne %s\n", digits);
			fprintf(output, "%s:\n", printing);
			fprintf(output, "\tpop dx\n");
			fprintf(output, "\tadd dx, 30h\n");
			fprintf(output, "\tmov DS:[di], dl\n");
			fprintf(output, "\tadd di, 1\n");
			fprintf(output, "\tadd cx, -1\n");
			fprintf(output,	"\tcmp cx, 0\n");
			fprintf(output, "\tjne %s\n", printing);
			fprintf(output, "\tmov dl, 24h\n");
			fprintf(output, "\tmov DS:[di], dl\n");
			fprintf(output, "\tmov dx, %u\n", tmpString);
			fprintf(output, "\tmov ah, 09h\n");
			fprintf(output, "\tint 21h\n");
		} else {
			char* true = newLabel();
			char* print = newLabel();
			int tmpOut = newTemporary(STR);
			fprintf(output, "\tmov al, DS:[%u]\n", address);
			fprintf(output, "\tcmp al, 0\n");
			fprintf(output, "\tjne %s\n", true);
			fprintf(output, "\tmov di, %u\n", tmpOut);
			fprintf(output, "\tmov ax, 70\n");
			fprintf(output, "\tmov DS:[di], ax\n");
			fprintf(output, "\tadd di, 1\n");
			fprintf(output, "\tmov ax, 65\n");
			fprintf(output, "\tmov DS:[di], ax\n");
			fprintf(output, "\tadd di, 1\n");
			fprintf(output, "\tmov ax, 76\n");
			fprintf(output, "\tmov DS:[di], ax\n");
			fprintf(output, "\tadd di, 1\n");
			fprintf(output, "\tmov ax, 83\n");
			fprintf(output, "\tmov DS:[di], ax\n");
			fprintf(output, "\tadd di, 1\n");
			fprintf(output, "\tmov ax, 69\n");
			fprintf(output, "\tmov DS:[di], ax\n");
			fprintf(output, "\tadd di, 1\n");
			fprintf(output, "\tmov ax, 36\n");
			fprintf(output, "\tmov DS:[di], ax\n");
			fprintf(output, "\tjmp %s\n", print);
			fprintf(output, "%s:\n", true);
			fprintf(output, "\tmov di, %u\n", tmpOut);
			fprintf(output, "\tmov ax, 84\n");
			fprintf(output, "\tmov DS:[di], ax\n");
			fprintf(output, "\tadd di, 1\n");
			fprintf(output, "\tmov ax, 82\n");
			fprintf(output, "\tmov DS:[di], ax\n");
			fprintf(output, "\tadd di, 1\n");
			fprintf(output, "\tmov ax, 85\n");
			fprintf(output, "\tmov DS:[di], ax\n");
			fprintf(output, "\tadd di, 1\n");
			fprintf(output, "\tmov ax, 69\n");
			fprintf(output, "\tmov DS:[di], ax\n");
			fprintf(output, "\tadd di, 1\n");
			fprintf(output, "\tmov ax, 36\n");
			fprintf(output, "\tmov DS:[di], ax\n");
			fprintf(output, "%s:\n", print);
			fprintf(output, "\tmov dx, %u\n", tmpOut);
			fprintf(output, "\tmov ah, 09h\n");
			fprintf(output, "\tint 21h\n");
		}

		while (reg.token == COMMA)
		{
			matchToken(COMMA);
			expr(&type, &address);

			if (type == STR) {
				fprintf(output, "\tmov dx, %u\n", address);
				fprintf(output, "\tmov ah, 09h\n");
				fprintf(output, "\tint 21h\n");
			} else if (type == INT || type == BYTE) {
				int tmpString = newTemporary(STR);
				char* positive = newLabel();
				char* digits = newLabel();
				char* printing = newLabel();
				fprintf(output, "\tmov di, %u\n", tmpString);
				fprintf(output, "\tmov al, DS:[%u]\n", address);

				if (type == INT)
					fprintf(output, "\tmov ah, DS:[%u]\n", address+1);
				else
					fprintf(output, "\tmov ah, 0\n");

				fprintf(output, "\tmov cx, 0\n");
				fprintf(output, "\tcmp ax, 0\n");
				fprintf(output, "\tjge %s\n", positive);
				fprintf(output, "\tmov bl, 2Dh\n");
				fprintf(output, "\tmov DS:[di], bl\n");
				fprintf(output, "\tadd di, 1\n");
				fprintf(output, "\tneg ax\n");
				fprintf(output, "%s:\n", positive);
				fprintf(output, "\tmov bx, 10\n");
				fprintf(output, "%s:\n", digits);
				fprintf(output, "\tadd cx, 1\n");
				fprintf(output, "\tmov dx, 0\n");
				fprintf(output, "\tidiv bx\n");
				fprintf(output, "\tpush dx\n");
				fprintf(output, "\tcmp ax, 0\n");
				fprintf(output, "\tjne %s\n", digits);
				fprintf(output, "%s:\n", printing);
				fprintf(output, "\tpop dx\n");
				fprintf(output, "\tadd dx, 30h\n");
				fprintf(output, "\tmov DS:[di], dl\n");
				fprintf(output, "\tadd di, 1\n");
				fprintf(output, "\tadd cx, -1\n");
				fprintf(output,	"\tcmp cx, 0\n");
				fprintf(output, "\tjne %s\n", printing);
				fprintf(output, "\tmov dl, 24h\n");
				fprintf(output, "\tmov DS:[di], dl\n");
				fprintf(output, "\tmov dx, %u\n", tmpString);
				fprintf(output, "\tmov ah, 09h\n");
				fprintf(output, "\tint 21h\n");
			} else {
				char* true = newLabel();
				char* print = newLabel();
				int tmpOut = newTemporary(STR);
				fprintf(output, "\tmov al, DS:[%u]\n", address);
				fprintf(output, "\tcmp al, 0\n");
				fprintf(output, "\tjne %s\n", true);
				fprintf(output, "\tmov di, %u\n", tmpOut);
				fprintf(output, "\tmov ax, 70\n");
				fprintf(output, "\tmov DS:[di], ax\n");
				fprintf(output, "\tadd di, 1\n");
				fprintf(output, "\tmov ax, 65\n");
				fprintf(output, "\tmov DS:[di], ax\n");
				fprintf(output, "\tadd di, 1\n");
				fprintf(output, "\tmov ax, 76\n");
				fprintf(output, "\tmov DS:[di], ax\n");
				fprintf(output, "\tadd di, 1\n");
				fprintf(output, "\tmov ax, 83\n");
				fprintf(output, "\tmov DS:[di], ax\n");
				fprintf(output, "\tadd di, 1\n");
				fprintf(output, "\tmov ax, 69\n");
				fprintf(output, "\tmov DS:[di], ax\n");
				fprintf(output, "\tadd di, 1\n");
				fprintf(output, "\tmov ax, 36\n");
				fprintf(output, "\tmov DS:[di], ax\n");
				fprintf(output, "\tjmp %s\n", print);
				fprintf(output, "%s:\n", true);
				fprintf(output, "\tmov di, %u\n", tmpOut);
				fprintf(output, "\tmov ax, 84\n");
				fprintf(output, "\tmov DS:[di], ax\n");
				fprintf(output, "\tadd di, 1\n");
				fprintf(output, "\tmov ax, 82\n");
				fprintf(output, "\tmov DS:[di], ax\n");
				fprintf(output, "\tadd di, 1\n");
				fprintf(output, "\tmov ax, 85\n");
				fprintf(output, "\tmov DS:[di], ax\n");
				fprintf(output, "\tadd di, 1\n");
				fprintf(output, "\tmov ax, 69\n");
				fprintf(output, "\tmov DS:[di], ax\n");
				fprintf(output, "\tadd di, 1\n");
				fprintf(output, "\tmov ax, 36\n");
				fprintf(output, "\tmov DS:[di], ax\n");
				fprintf(output, "%s:\n", print);
				fprintf(output, "\tmov dx, %u\n", tmpOut);
				fprintf(output, "\tmov ah, 09h\n");
				fprintf(output, "\tint 21h\n");
			}
		}

		matchToken(SEMICOLON);


		fprintf(output, "\tmov ah, 02h\n");
		fprintf(output, "\tmov dl, 0Dh\n");
		fprintf(output, "\tint 21h\n");
		fprintf(output, "\tmov dl, 0Ah\n");
		fprintf(output, "\tint 21h\n");
#ifdef DEBUG
	printf("\n");
#endif

		stack = tmpStack;
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
			printf("%lu:tipos incompativeis.\n", line);
			exit(INCOMPATIBLETYPES);
		}
		else if (firstExprType == BOOL && *type != BOOL)
		{
			printf("%lu:tipos incompativeis.\n", line);
			exit(INCOMPATIBLETYPES);
		}
		else if (firstExprType == BYTE && (*type != INT && *type != BYTE))
		{
			printf("%lu:tipos incompativeis.\n", line);
			exit(INCOMPATIBLETYPES);
		}
		else if (firstExprType == INT && (*type != INT && *type != BYTE))
		{
			printf("%lu:tipos incompativeis.\n", line);
			exit(INCOMPATIBLETYPES);
		}
		else if (firstExprType == STR && token != EQUAL)
		{
			printf("%lu:tipos incompativeis.\n", line);
			exit(INCOMPATIBLETYPES);
		}
		else if (firstExprType == BOOL && (token != EQUAL && token != DIFF))
		{
			printf("%lu:tipos incompativeis.\n", line);
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
						fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
						fprintf(output, "\tmov bl, DS:[%u]\n", *address);
						fprintf(output, "\tcmp al, bl\n");
						fprintf(output, "\tje %s\n", true);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", true);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "%s:\n", end);
						fprintf(output, "\tmov DS:[%u], al\n", *address);
					}
					else if (*type == INT || *type == BYTE)
					{
						if (firstExprType == BYTE && *type == BYTE)
						{
							fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
							fprintf(output, "\tmov bl, DS:[%u]\n", *address);
							fprintf(output, "\tcmp al, bl\n");
							fprintf(output, "\tje %s\n", true);
							fprintf(output, "\tmov al, 0\n");
							fprintf(output, "\tjmp %s\n", end);
							fprintf(output, "%s:\n", true);
							fprintf(output, "\tmov al, 255\n");
							fprintf(output, "%s:\n", end);
							fprintf(output, "\tmov DS:[%u], al\n", *address);
						}
						else if (firstExprType == INT && *type == BYTE)
						{
							fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
							fprintf(output, "\tmov ah, DS:[%u]\n", firstExprAdd+1);
							fprintf(output, "\tmov bl, DS:[%u]\n", *address);
							fprintf(output, "\tmov bh, 0\n");
							fprintf(output, "\tcmp ax, bx\n");
							fprintf(output, "\tje %s\n", true);
							fprintf(output, "\tmov al, 0\n");
							fprintf(output, "\tjmp %s\n", end);
							fprintf(output, "%s:\n", true);
							fprintf(output, "\tmov al, 255\n");
							fprintf(output, "%s:\n", end);
							fprintf(output, "\tmov DS:[%u], al\n", *address);
						}
						else if (firstExprType == BYTE && *type == INT)
						{
							fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
							fprintf(output, "\tmov ah, 0\n");
							fprintf(output, "\tmov bl, DS:[%u]\n", *address);
							fprintf(output, "\tmov bh, DS:[%u]\n", *address+1);
							fprintf(output, "\tcmp ax, bx\n");
							fprintf(output, "\tje %s\n", true);
							fprintf(output, "\tmov al, 0\n");
							fprintf(output, "\tjmp %s\n", end);
							fprintf(output, "%s:\n", true);
							fprintf(output, "\tmov al, 255\n");
							fprintf(output, "%s:\n", end);
							fprintf(output, "\tmov DS:[%u], al\n", *address);
						}
						else
						{
							fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
							fprintf(output, "\tmov ah, DS:[%u]\n", firstExprAdd+1);
							fprintf(output, "\tmov bl, DS:[%u]\n", *address);
							fprintf(output, "\tmov bh, DS:[%u]\n", *address+1);
							fprintf(output, "\tcmp ax, bx\n");
							fprintf(output, "\tje %s\n", true);
							fprintf(output, "\tmov al, 0\n");
							fprintf(output, "\tjmp %s\n", end);
							fprintf(output, "%s:\n", true);
							fprintf(output, "\tmov al, 255\n");
							fprintf(output, "%s:\n", end);
							fprintf(output, "\tmov DS:[%u], al\n", *address);
						}
					}
					else
					{
						char* equal = newLabel();
						char* notequal = newLabel();
						char* iterate = newLabel();
						char* firstended = newLabel();
						char* end = newLabel();
						fprintf(output, "\tmov di, %u\n", firstExprAdd);
						fprintf(output, "\tmov bx, %u\n", *address);
						fprintf(output, "%s:\n", iterate);
						fprintf(output, "\tmov al, DS:[di]\n");
						fprintf(output, "\tmov ah, 0\n");
						fprintf(output, "\tcmp al, 24h\n");
						fprintf(output, "\tje %s\n", firstended);
						fprintf(output, "\tmov cl, DS:[bx]\n");
						fprintf(output, "\tmov ch, 0\n");
						fprintf(output, "\tcmp cl, 24h\n");
						fprintf(output, "\tje %s\n", notequal);
						fprintf(output, "\tcmp ax, cx\n");
						fprintf(output, "\tjne %s\n", notequal);
						fprintf(output, "\tadd bx, 1\n");
						fprintf(output, "\tadd di, 1\n");
						fprintf(output, "\tjmp %s\n", iterate);
						fprintf(output, "%s:\n", equal);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "\tmov DS:[%u], al\n", *address);
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", notequal);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tmov DS:[%u], al\n", *address);
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", firstended);
						fprintf(output, "\tmov al, DS:[bx]\n");
						fprintf(output, "\tcmp al, 24h\n");
						fprintf(output, "\tje %s\n", equal);
						fprintf(output, "%s:\n", end);
					}
					break;
				case DIFF:
					if (*type == BOOL)
					{
						fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
						fprintf(output, "\tmov bl, DS:[%u]\n", *address);
						fprintf(output, "\tcmp al, bl\n");
						fprintf(output, "\tjne %s\n", true);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", true);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "%s:\n", end);
						fprintf(output, "\tmov DS:[%u], al\n", *address);
					}
					else if (*type == INT || *type == BYTE)
					{
						if (firstExprType == BYTE && *type == BYTE)
						{
							fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
							fprintf(output, "\tmov bl, DS:[%u]\n", *address);
							fprintf(output, "\tcmp al, bl\n");
							fprintf(output, "\tjne %s\n", true);
							fprintf(output, "\tmov al, 0\n");
							fprintf(output, "\tjmp %s\n", end);
							fprintf(output, "%s:\n", true);
							fprintf(output, "\tmov al, 255\n");
							fprintf(output, "%s:\n", end);
							fprintf(output, "\tmov DS:[%u], al\n", *address);
						}
						else if (firstExprType == INT && *type == BYTE)
						{
							fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
							fprintf(output, "\tmov ah, DS:[%u]\n", firstExprAdd+1);
							fprintf(output, "\tmov bl, DS:[%u]\n", *address);
							fprintf(output, "\tmov bh, 0\n");
							fprintf(output, "\tcmp ax, bx\n");
							fprintf(output, "\tjne %s\n", true);
							fprintf(output, "\tmov al, 0\n");
							fprintf(output, "\tjmp %s\n", end);
							fprintf(output, "%s:\n", true);
							fprintf(output, "\tmov al, 255\n");
							fprintf(output, "%s:\n", end);
							fprintf(output, "\tmov DS:[%u], al\n", *address);
						}
						else if (firstExprType == BYTE && *type == INT)
						{
							fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
							fprintf(output, "\tmov ah, 0\n");
							fprintf(output, "\tmov bl, DS:[%u]\n", *address);
							fprintf(output, "\tmov bh, DS:[%u]\n", *address+1);
							fprintf(output, "\tcmp ax, bx\n");
							fprintf(output, "\tjne %s\n", true);
							fprintf(output, "\tmov al, 0\n");
							fprintf(output, "\tjmp %s\n", end);
							fprintf(output, "%s:\n", true);
							fprintf(output, "\tmov al, 255\n");
							fprintf(output, "%s:\n", end);
							fprintf(output, "\tmov DS:[%u], al\n", *address);
						}
						else
						{
							fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
							fprintf(output, "\tmov ah, DS:[%u]\n", firstExprAdd+1);
							fprintf(output, "\tmov bl, DS:[%u]\n", *address);
							fprintf(output, "\tmov bh, DS:[%u]\n", *address+1);
							fprintf(output, "\tcmp ax, bx\n");
							fprintf(output, "\tjne %s\n", true);
							fprintf(output, "\tmov al, 0\n");
							fprintf(output, "\tjmp %s\n", end);
							fprintf(output, "%s:\n", true);
							fprintf(output, "\tmov al, 255\n");
							fprintf(output, "%s:\n", end);
							fprintf(output, "\tmov DS:[%u], al\n", *address);
						}
					}
					break;
				case MINOR:
					if (firstExprType == BYTE && *type == BYTE)
					{
						fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
						fprintf(output, "\tmov bl, DS:[%u]\n", *address);
						fprintf(output, "\tcmp al, bl\n");
						fprintf(output, "\tjl %s\n", true);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", true);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "%s:\n", end);
						fprintf(output, "\tmov DS:[%u], al\n", *address);
					}
					else if (firstExprType == INT && *type == BYTE)
					{
						fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
						fprintf(output, "\tmov ah, DS:[%u]\n", firstExprAdd+1);
						fprintf(output, "\tmov bl, DS:[%u]\n", *address);
						fprintf(output, "\tmov bh, 0\n");
						fprintf(output, "\tcmp ax, bx\n");
						fprintf(output, "\tjl %s\n", true);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", true);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "%s:\n", end);
						fprintf(output, "\tmov DS:[%u], al\n", *address);
					}
					else if (firstExprType == BYTE && *type == INT)
					{
						fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
						fprintf(output, "\tmov ah, 0\n");
						fprintf(output, "\tmov bl, DS:[%u]\n", *address);
						fprintf(output, "\tmov bh, DS:[%u]\n", *address+1);
						fprintf(output, "\tcmp ax, bx\n");
						fprintf(output, "\tjl %s\n", true);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", true);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "%s:\n", end);
						fprintf(output, "\tmov DS:[%u], al\n", *address);
					}
					else
					{
						fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
						fprintf(output, "\tmov ah, DS:[%u]\n", firstExprAdd+1);
						fprintf(output, "\tmov bl, DS:[%u]\n", *address);
						fprintf(output, "\tmov bh, DS:[%u]\n", *address+1);
						fprintf(output, "\tcmp ax, bx\n");
						fprintf(output, "\tjl %s\n", true);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", true);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "%s:\n", end);
						fprintf(output, "\tmov DS:[%u], al\n", *address);
					}
					break;
				case GREATER:
					if (firstExprType == BYTE && *type == BYTE)
					{
						fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
						fprintf(output, "\tmov bl, DS:[%u]\n", *address);
						fprintf(output, "\tcmp al, bl\n");
						fprintf(output, "\tjg %s\n", true);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", true);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "%s:\n", end);
						fprintf(output, "\tmov DS:[%u], al\n", *address);
					}
					else if (firstExprType == INT && *type == BYTE)
					{
						fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
						fprintf(output, "\tmov ah, DS:[%u]\n", firstExprAdd+1);
						fprintf(output, "\tmov bl, DS:[%u]\n", *address);
						fprintf(output, "\tmov bh, 0\n");
						fprintf(output, "\tcmp ax, bx\n");
						fprintf(output, "\tjg %s\n", true);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", true);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "%s:\n", end);
						fprintf(output, "\tmov DS:[%u], al\n", *address);
					}
					else if (firstExprType == BYTE && *type == INT)
					{
						fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
						fprintf(output, "\tmov ah, 0\n");
						fprintf(output, "\tmov bl, DS:[%u]\n", *address);
						fprintf(output, "\tmov bh, DS:[%u]\n", *address+1);
						fprintf(output, "\tcmp ax, bx\n");
						fprintf(output, "\tjg %s\n", true);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", true);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "%s:\n", end);
						fprintf(output, "\tmov DS:[%u], al\n", *address);
					}
					else
					{
						fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
						fprintf(output, "\tmov ah, DS:[%u]\n", firstExprAdd+1);
						fprintf(output, "\tmov bl, DS:[%u]\n", *address);
						fprintf(output, "\tmov bh, DS:[%u]\n", *address+1);
						fprintf(output, "\tcmp ax, bx\n");
						fprintf(output, "\tjg %s\n", true);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", true);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "%s:\n", end);
						fprintf(output, "\tmov DS:[%u], al\n", *address);
					}
					break;
				case MINEQ:
					if (firstExprType == BYTE && *type == BYTE)
					{
						fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
						fprintf(output, "\tmov bl, DS:[%u]\n", *address);
						fprintf(output, "\tcmp al, bl\n");
						fprintf(output, "\tjle %s\n", true);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", true);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "%s:\n", end);
						fprintf(output, "\tmov DS:[%u], al\n", *address);
					}
					else if (firstExprType == INT && *type == BYTE)
					{
						fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
						fprintf(output, "\tmov ah, DS:[%u]\n", firstExprAdd+1);
						fprintf(output, "\tmov bl, DS:[%u]\n", *address);
						fprintf(output, "\tmov bh, 0\n");
						fprintf(output, "\tcmp ax, bx\n");
						fprintf(output, "\tjle %s\n", true);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", true);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "%s:\n", end);
						fprintf(output, "\tmov DS:[%u], al\n", *address);
					}
					else if (firstExprType == BYTE && *type == INT)
					{
						fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
						fprintf(output, "\tmov ah, 0\n");
						fprintf(output, "\tmov bl, DS:[%u]\n", *address);
						fprintf(output, "\tmov bh, DS:[%u]\n", *address+1);
						fprintf(output, "\tcmp ax, bx\n");
						fprintf(output, "\tjle %s\n", true);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", true);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "%s:\n", end);
						fprintf(output, "\tmov DS:[%u], al\n", *address);
					}
					else
					{
						fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
						fprintf(output, "\tmov ah, DS:[%u]\n", firstExprAdd+1);
						fprintf(output, "\tmov bl, DS:[%u]\n", *address);
						fprintf(output, "\tmov bh, DS:[%u]\n", *address+1);
						fprintf(output, "\tcmp ax, bx\n");
						fprintf(output, "\tjle %s\n", true);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", true);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "%s:\n", end);
						fprintf(output, "\tmov DS:[%u], al\n", *address);
					}
					break;
				case GREATEQ:
					if (firstExprType == BYTE && *type == BYTE)
					{
						fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
						fprintf(output, "\tmov bl, DS:[%u]\n", *address);
						fprintf(output, "\tcmp al, bl\n");
						fprintf(output, "\tjge %s\n", true);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", true);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "%s:\n", end);
						fprintf(output, "\tmov DS:[%u], al\n", *address);
					}
					else if (firstExprType == INT && *type == BYTE)
					{
						fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
						fprintf(output, "\tmov ah, DS:[%u]\n", firstExprAdd+1);
						fprintf(output, "\tmov bl, DS:[%u]\n", *address);
						fprintf(output, "\tmov bh, 0\n");
						fprintf(output, "\tcmp ax, bx\n");
						fprintf(output, "\tjge %s\n", true);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", true);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "%s:\n", end);
						fprintf(output, "\tmov DS:[%u], al\n", *address);
					}
					else if (firstExprType == BYTE && *type == INT)
					{
						fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
						fprintf(output, "\tmov ah, 0\n");
						fprintf(output, "\tmov bl, DS:[%u]\n", *address);
						fprintf(output, "\tmov bh, DS:[%u]\n", *address+1);
						fprintf(output, "\tcmp ax, bx\n");
						fprintf(output, "\tjge %s\n", true);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", true);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "%s:\n", end);
						fprintf(output, "\tmov DS:[%u], al\n", *address);
					}
					else
					{
						fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
						fprintf(output, "\tmov ah, DS:[%u]\n", firstExprAdd+1);
						fprintf(output, "\tmov bl, DS:[%u]\n", *address);
						fprintf(output, "\tmov bh, DS:[%u]\n", *address+1);
						fprintf(output, "\tcmp ax, bx\n");
						fprintf(output, "\tjge %s\n", true);
						fprintf(output, "\tmov al, 0\n");
						fprintf(output, "\tjmp %s\n", end);
						fprintf(output, "%s:\n", true);
						fprintf(output, "\tmov al, 255\n");
						fprintf(output, "%s:\n", end);
						fprintf(output, "\tmov DS:[%u], al\n", *address);
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
		printf("%lu:tipos incompativeis.\n", line);
		exit(INCOMPATIBLETYPES);
	}


	if (token == MINUS)
	{
		if (*type == INT)
		{
			fprintf(output, "\tmov al, DS:[%u]\n", *address);
			fprintf(output, "\tmov ah, DS:[%u]\n", *address+1);
			fprintf(output, "\tneg ax\n");
			fprintf(output, "\tmov DS:[%u], al\n", *address);
			fprintf(output, "\tmov DS:[%u], ah\n", *address+1);
		}
		else if (*type == BYTE)
		{
			fprintf(output, "\tmov al, DS:[%u]\n", *address);
			fprintf(output, "\tmov ah, 0\n");
			fprintf(output, "\tneg ax\n");
			int newLocation = newTemporary(INT);
			fprintf(output, "\tmov DS:[%u], al\n", newLocation);
			fprintf(output, "\tmov DS:[%u], ah\n", newLocation+1);
			*address = newLocation;
		}
		*type = INT;
	}

	while(reg.token == PLUS || reg.token == MINUS || reg.token == OR)
	{
		token = reg.token;

		if (token == MINUS && (*type != BYTE && *type != INT))
		{
			printf("%lu:tipos incompativeis.\n", line);
			exit(INCOMPATIBLETYPES);
		}
		else if (token == PLUS && *type == BOOL)
		{
			printf("%lu:tipos incompativeis.\n", line);
			exit(INCOMPATIBLETYPES);
		}
		else if (token == OR && *type != BOOL)
		{
			printf("%lu:tipos incompativeis.\n", line);
			exit(INCOMPATIBLETYPES);
		}

		int firstExprAdd = *address;
		int firstExprType = *type;

		matchToken(reg.token);
		B(type, address);

		if (token == MINUS && (*type != BYTE && *type != INT))
		{
			printf("%lu:tipos incompativeis.\n", line);
			exit(INCOMPATIBLETYPES);
		}
		else if (token == PLUS && *type == BOOL)
		{
			printf("%lu:tipos incompativeis.\n", line);
			exit(INCOMPATIBLETYPES);
		}
		else if (token == OR && *type != BOOL)
		{
			printf("%lu:tipos incompativeis.\n", line);
			exit(INCOMPATIBLETYPES);
		}

		if ((firstExprType == BYTE || firstExprType == INT) && (*type != BYTE && *type != INT))
		{
			printf("%lu:tipos incompativeis.\n", line);
			exit(INCOMPATIBLETYPES);
		}

		switch(token)
		{
			case PLUS:
				if (firstExprType == BYTE && *type == BYTE)
				{
					fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
					fprintf(output, "\tmov bl, DS:[%u]\n", *address);
					fprintf(output, "\tadd al, bl\n");
					fprintf(output, "\tmov DS:[%u], al\n", *address);
				}
				else if (firstExprType == INT && *type == BYTE)
				{
					fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
					fprintf(output, "\tmov ah, DS:[%u]\n", firstExprAdd+1);
					fprintf(output, "\tmov bl, DS:[%u]\n", *address);
					fprintf(output, "\tmov bh, 0\n");
					fprintf(output, "\tadd ax, bx\n");
					fprintf(output, "\tmov DS:[%u], al\n", *address);
					fprintf(output, "\tmov DS:[%u], ah\n", *address+1);
					*type = INT;
				}
				else if (firstExprType == BYTE && *type == INT)
				{
					fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
					fprintf(output, "\tmov ah, 0\n");
					fprintf(output, "\tmov bl, DS:[%u]\n", *address);
					fprintf(output, "\tmov bh, DS:[%u]\n", *address+1);
					fprintf(output, "\tadd ax, bx\n");
					fprintf(output, "\tmov DS:[%u], al\n", *address);
					fprintf(output, "\tmov DS:[%u], ah\n", *address+1);
				}
				else if (firstExprType == INT && *type == INT)
				{
					fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
					fprintf(output, "\tmov ah, DS:[%u]\n", firstExprAdd+1);
					fprintf(output, "\tmov bl, DS:[%u]\n", *address);
					fprintf(output, "\tmov bh, DS:[%u]\n", *address+1);
					fprintf(output, "\tadd ax, bx\n");
					fprintf(output, "\tmov DS:[%u], al\n", *address);
					fprintf(output, "\tmov DS:[%u], ah\n", *address+1);
				}
				else
				{
					char* loop = newLabel();
					char* concat = newLabel();
					fprintf(output, "\tmov bx, %u\n", firstExprAdd);
					fprintf(output, "\tmov di, %u\n", *address);
					fprintf(output, "\tadd bx, -1\n");
					fprintf(output, "%s:\n", loop);
					fprintf(output, "\tadd bx, 1\n");
					fprintf(output, "\tmov al, DS:[bx]\n");
					fprintf(output, "\tcmp al, 24h\n");
					fprintf(output, "\tjne %s\n", loop);
					fprintf(output, "%s:\n", concat);
					fprintf(output, "\tmov al, DS:[di]\n");
					fprintf(output, "\tmov DS:[bx], al\n");
					fprintf(output, "\tadd bx, 1\n");
					fprintf(output, "\tadd di, 1\n");
					fprintf(output, "\tcmp al, 24h\n");
					fprintf(output, "\tjne %s\n", concat);

					*address = firstExprAdd;
				}
				break;
			case MINUS:
				if (firstExprType == BYTE && *type == BYTE)
				{
					fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
					fprintf(output, "\tmov bl, DS:[%u]\n", *address);
					fprintf(output, "\tsub al, bl\n");
					fprintf(output, "\tmov DS:[%u], al\n", *address);
				}
				else if (firstExprType == INT && *type == BYTE)
				{
					fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
					fprintf(output, "\tmov ah, DS:[%u]\n", firstExprAdd+1);
					fprintf(output, "\tmov bl, DS:[%u]\n", *address);
					fprintf(output, "\tmov bh, 0\n");
					fprintf(output, "\tsub ax, bx\n");
					fprintf(output, "\tmov DS:[%u], al\n", *address);
					fprintf(output, "\tmov DS:[%u], ah\n", *address+1);
					*type = INT;
				}
				else if (firstExprType == BYTE && *type == INT)
				{
					fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
					fprintf(output, "\tmov ah, 0\n");
					fprintf(output, "\tmov bl, DS:[%u]\n", *address);
					fprintf(output, "\tmov bh, DS:[%u]\n", *address+1);
					fprintf(output, "\tsub ax, bx\n");
					fprintf(output, "\tmov DS:[%u], al\n", *address);
					fprintf(output, "\tmov DS:[%u], ah\n", *address+1);
				}
				else
				{
					fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
					fprintf(output, "\tmov ah, DS:[%u]\n", firstExprAdd+1);
					fprintf(output, "\tmov bl, DS:[%u]\n", *address);
					fprintf(output, "\tmov bh, DS:[%u]\n", *address+1);
					fprintf(output, "\tsub ax, bx\n");
					fprintf(output, "\tmov DS:[%u], al\n", *address);
					fprintf(output, "\tmov DS:[%u], ah\n", *address+1);
				}
				break;
			case OR:
				fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
				fprintf(output, "\tmov bl, DS:[%u]\n", *address);
				fprintf(output, "\tor al, bl\n");
				fprintf(output, "\tmov DS:[%u], al\n", *address);
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
			printf("%lu:tipos incompativeis.\n", line);
			exit(INCOMPATIBLETYPES);
		}

		matchToken(reg.token);

		C(type, address);

		if ( ( (operation == MULT || operation == DIVIDE) && (*type == STR || *type == BOOL) )
		    || (operation == AND && *type != BOOL) ) 
		{
			printf("%lu:tipos incompativeis.\n", line);
			exit(INCOMPATIBLETYPES);
		}
		else
		{
			switch(operation)
			{
				case MULT:
					if (firstExprType == BYTE && *type == BYTE)
					{
						fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
						fprintf(output, "\tmov bl, DS:[%u]\n", *address);
						fprintf(output, "\tmul bl\n");
						fprintf(output, "\tmov DS:[%u], al\n", *address);
					}
					else if (firstExprType == INT && *type == INT)
					{
						fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
						fprintf(output, "\tmov ah, DS:[%u]\n", firstExprAdd+1);
						fprintf(output, "\tmov bl, DS:[%u]\n", *address);
						fprintf(output, "\tmov bh, DS:[%u]\n", *address+1);
						fprintf(output, "\timul bx\n");
						fprintf(output, "\tmov DS:[%u], al\n", *address);
						fprintf(output, "\tmov DS:[%u], ah\n", *address+1);
					}
					else if (firstExprType == INT && *type == BYTE)
					{
						fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
						fprintf(output, "\tmov ah, DS:[%u]\n", firstExprAdd+1);
						fprintf(output, "\tmov bl, DS:[%u]\n", *address);
						fprintf(output, "\tmov bh, 0\n");
						fprintf(output, "\timul bx\n");
						fprintf(output, "\tmov DS:[%u], al\n", *address);
						fprintf(output, "\tmov DS:[%u], ah\n", *address+1);
						*type = INT;
					}
					else
					{
						fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
						fprintf(output, "\tmov ah, 0\n");
						fprintf(output, "\tmov bl, DS:[%u]\n", *address);
						fprintf(output, "\tmov bh, DS:[%u]\n", *address+1);
						fprintf(output, "\timul bx\n");
						fprintf(output, "\tmov DS:[%u], al\n", *address);
						fprintf(output, "\tmov DS:[%u], ah\n", *address+1);
					}
					break;
				case DIVIDE:
					if (firstExprType == BYTE && *type == BYTE)
					{
						fprintf(output, "\tmov dx, 0\n");
						fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
						fprintf(output, "\tmov ah, 0\n");
						fprintf(output, "\tmov bl, DS:[%u]\n", *address);
						fprintf(output, "\tmov bh, 0\n");
						fprintf(output, "\tidiv bx\n");
						fprintf(output, "\tmov DS:[%u], al\n", *address);
					}
					else if (firstExprType == INT && *type == INT)
					{
						fprintf(output, "\tmov dx, 0\n");
						fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
						fprintf(output, "\tmov ah, DS:[%u]\n", firstExprAdd+1);
						fprintf(output, "\tmov bl, DS:[%u]\n", *address);
						fprintf(output, "\tmov bh, DS:[%u]\n", *address+1);
						fprintf(output, "\tidiv bx\n");
						fprintf(output, "\tmov DS:[%u], al\n", *address);
						fprintf(output, "\tmov DS:[%u], ah\n", *address+1);
					}
					else if (firstExprType == INT && *type == BYTE)
					{
						fprintf(output, "\tmov dx, 0\n");
						fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
						fprintf(output, "\tmov ah, DS:[%u]\n", firstExprAdd+1);
						fprintf(output, "\tmov bl, DS:[%u]\n", *address);
						fprintf(output, "\tmov bh, 0\n");
						fprintf(output, "\tidiv bx\n");
						fprintf(output, "\tmov DS:[%u], al\n", *address);
						fprintf(output, "\tmov DS:[%u], ah\n", *address+1);
						*type = INT;
					}
					else
					{
						fprintf(output, "\tmov dx, 0\n");
						fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
						fprintf(output, "\tmov ah, 0\n");
						fprintf(output, "\tmov bl, DS:[%u]\n", *address);
						fprintf(output, "\tmov bh, DS:[%u]\n", *address+1);
						fprintf(output, "\tidiv bx\n");
						fprintf(output, "\tmov DS:[%u], al\n", *address);
						fprintf(output, "\tmov DS:[%u], ah\n", *address+1);
					}
					break;
				case AND:
					fprintf(output, "\tmov al, DS:[%u]\n", firstExprAdd);
					fprintf(output, "\tmov bl, DS:[%u]\n", *address);
					fprintf(output, "\tand al, bl\n");
					fprintf(output, "\tmov DS:[%u], al\n", *address);
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
		printf("%lu:tipos incompativeis.\n", line);
		exit(INCOMPATIBLETYPES);
	}
	else if (not)
	{
		fprintf(output, "\tmov al, DS:[%u]\n", *address);
		fprintf(output, "\nnot al\n");
		fprintf(output, "\tmov DS:[%u], al\n", *address);
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
			fprintf(output, "\tmov al, %u\n", atoi(reg.lexem) & 0x000000FF);
			fprintf(output, "\tmov ah, %u\n", (atoi(reg.lexem) & 0x0000FF00) >> 8);
			fprintf(output, "\tmov DS:[%u], al\n", *address);
			fprintf(output, "\tmov DS:[%u], ah\n", *address + 1);
		} else if (*type == STR) {
			fprintf(output, "\tmov bx, %u\n", *address);
			for (int i = 1; i < strlen(reg.lexem)-1; i++) {
				fprintf(output, "\tmov al, %u\n", reg.lexem[i]);
				fprintf(output, "\tmov DS:[bx], al\n");
				fprintf(output, "\tadd bx, 1\n");
			}
			fprintf(output, "\tmov al, 24h\n");
			fprintf(output, "\tmov DS:[bx], al\n");
		} else if (*type == BYTE) {
			fprintf(output, "\tmov al, %u\n", atoi(reg.lexem) & 0x000000FF);
			fprintf(output, "\tmov DS:[%u], al\n", *address);
		} else {
			fprintf(output, "\tmov al, %u\n", (strncmp(reg.lexem, "TRUE", 4) == 0 ? 0xFF : 0x00));
			fprintf(output, "\tmov DS:[%u], al\n", *address);
		}

		matchToken(LITERAL);
	}
	else
	{
		if (reg.entry->class == NULL) {
			printf("%lu:identificador nao declarado %s.\n", line, reg.lexem);
			exit(UNDECLAREDIDENTIFIER);
		}

		*type = reg.entry->type;
		*address = reg.entry->address;

		int temporary = newTemporary(*type);

		if (*type == BYTE || *type == BOOL)
		{
			fprintf(output, "\tmov al, DS:[%u]\n", *address);
			fprintf(output, "\tmov DS:[%u], al\n", temporary);
		}
		else if (*type == INT)
		{
			fprintf(output, "\tmov al, DS:[%u]\n", *address);
			fprintf(output, "\tmov ah, DS:[%u]\n", *address+1);
			fprintf(output, "\tmov DS:[%u], al\n", temporary);
			fprintf(output, "\tmov DS:[%u], ah\n", temporary+1);
		}
		else if (*type == STR)
		{
			char* end = newLabel();
			char* loop = newLabel();
			fprintf(output, "\tmov di, %u\n", temporary);
			fprintf(output, "\tmov bx, %u\n", *address);
			fprintf(output, "%s:\n", loop);
			fprintf(output, "\tmov al, DS:[bx]\n");
			fprintf(output, "\tcmp al, 24h\n");
			fprintf(output, "\tje %s\n", end);
			fprintf(output, "\tmov DS:[di], al\n");
			fprintf(output, "\tadd di, 1\n");
			fprintf(output, "\tadd bx, 1\n");
			fprintf(output, "\tjmp %s\n", loop);
			fprintf(output, "%s:\n", end);
			fprintf(output, "\tmov DS:[di], al\n");
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
		printf("%lu:token nao esperado %s.\n", line, reg.lexem);
		exit(UNSPECTEDTOKEN);
	}
}

char* newLabel() {
	int digits = label == 0 ? 1 : floor(log10(abs(label))) + 1;
	char* lbl = malloc(sizeof(char) * (digits + 2));
	sprintf(lbl, "R%d", label);
	label++;
	return lbl;
}

int newTemporary(int type)
{
	int tmp = stack;

	if (type == BOOL || type == BYTE)
		stack++;
	else if (type == INT)
		stack += 2;
	else if (type == STR)
		stack += 0x100;
	else if (type == BUFFER)
		stack += 0x103;

	return tmp;
}