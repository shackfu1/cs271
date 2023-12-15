/****************************************
 * C-ploration 4 for CS 271
 * 
 * [NAME] $Sasha Hackenbruck$
 * [TERM] FALL $2023$
 * 
 ****************************************/
#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define SYMBOL_TABLE_SIZE 1000

typedef int16_t hack_addr;

struct Symbol{
	char* name;
	hack_addr addr;
};

int hash(char *str);
struct Symbol *symtable_find(char * key);
void symtable_insert(char* key, hack_addr addr);
void symtable_display_table();
void symtable_print_labels();

#endif