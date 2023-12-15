/****************************************
 * C-ploration 4 for CS 271
 * 
 * [NAME] $Sasha Hackenbruck$
 * [TERM] FALL $2023$
 * 
 ****************************************/
#include "parser.h"
#include "symtable.h"
#include "error.h"
#define MAX_INSTRUCTION_COUNT 30000

int main(int argc, const char *argv[])
{		
	instruction *instructions = malloc(MAX_INSTRUCTION_COUNT * sizeof(instruction));
	if (!(argc == 2)){
		// incorrect number of arguments
    	exit_program(EXIT_INCORRECT_ARGUMENTS, argv[0]);   
	}
	FILE *fin = fopen( argv[1], "r" );
	if (fin == NULL){
		exit_program(EXIT_CANNOT_OPEN_FILE, argv[1]);
	}
	int num_instructions = parse(fin, instructions);
	//symtable_print_labels();
	fclose(fin);
	assemble(argv[1], instructions, num_instructions);	
	free(instructions);
}


