/****************************************
 * C-ploration 4 for CS 271
 * 
 * [NAME] $Sasha Hackenbruck$
 * [TERM] FALL $2023$
 * 
 ****************************************/
#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include "hack.h"

#define MAX_LINE_LENGTH  200
#define MAX_LABEL_LENGTH MAX_LINE_LENGTH - 2
#define MAX_HACK_ADDRESS INT16_MAX
#define MAX_INSTRUCTIONS MAX_HACK_ADDRESS

typedef int16_t hack_addr;
typedef int16_t opcode;

enum instr_type{
	Invalid = -1,
	Atype,
	Ctype,
};

typedef struct c_instruction{
	opcode a:1;
	opcode comp:7;
	opcode dest:4;
	opcode jump:4;
}c_instruction;
typedef struct a_instruction{
	union unionname{
		hack_addr address;
		char * label;
	}operand;
	bool is_addr;
}a_instruction;
typedef struct instruction{
	union instructiontype{
		a_instruction a;
		c_instruction c;
	}instr;
	enum instr_type field;
}instruction;


/** function prototypes **/
char *strip(char *s);

int parse(FILE * file, instruction *instructions);
void add_predefined_symbols();

bool is_Atype(const char *);
bool is_label(const char *);
bool is_Ctype(const char *);
bool parse_A_instruction(const char *line, a_instruction *instr);
void parse_C_instruction(char *line, c_instruction *instr);

opcode instruction_to_opcode(c_instruction instr);

void assemble(const char * file_name, instruction* instructions, int num_instructions);

char *extract_label(char *, char*);
#endif