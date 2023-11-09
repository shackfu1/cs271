/****************************************
 * C-ploration 4 for CS 271
 * 
 * [NAME] $Sasha Hackenbruck$
 * [TERM] FALL $2023$
 * 
 ****************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_LINE_LENGTH  200
#define MAX_LABEL_LENGTH MAX_LINE_LENGTH - 2

typedef int16_t hack_addr;
typedef int16_t opcode;

enum instr_type{
	Invalid = -1,
	Atype,
	Ctype,
};

typedef struct c_instruction{
	opcode a:1;
	opcode comp:3;
	opcode dest:5;
	opcode jump:5;
}c_instruction;
typedef struct a_instruction{
	union unionname{
		hack_addr adress;
		char * label;
	}adress,label;
	bool is_addr;
}a_instruction;
typedef struct instruction{
	union instructiontype{
		a_instruction a;
		c_instruction c;
	}a,c;
	enum instr_type field;
}instruction;


/** function prototypes **/
char *strip(char *s);

void parse(FILE * file);

bool is_Atype(const char *);
bool is_label(const char *);
bool is_Ctype(const char *);

char *extract_label(char *, char*);