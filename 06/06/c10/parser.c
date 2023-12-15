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

#define MAX_LINE_LENGTH  200
#define NUM_PREDEFINED_SYMBOLS 23

/* Function: strip
 * -------------
 * remove whitespace and comments from a line
 *
 * s: the char* string to strip
 *
 * returns: the stripped char* string
 */
char *strip(char *s){	

	char s_new[strlen(s) + 1];
	int i = 0;
	for (char *s2 = s; *s2; s2++) { 
   		if (*s2 == '/' && *(s2+1) == '/'){
   			break;
   		}else if(!isspace(*s2)){
   			s_new[i++] = *s2;
   		}
	}
	s_new[i] = '\0';
    strcpy(s,s_new);
    return s;	
}

/* Function: parse
 * -------------
 * iterate each line in the file and strip whitespace and comments. 
 *
 * file: pointer to FILE to parse
 *
 * returns: nothing
 */
int parse(FILE * file, instruction *instructions){
	instruction instr;
	char line[MAX_LINE_LENGTH] = {0};
	int line_num = 0;
	int instr_num = 0;
	add_predefined_symbols();
	while (fgets(line, sizeof(line), file)){
		if (instr_num > MAX_INSTRUCTIONS){
			exit_program(EXIT_TOO_MANY_INSTRUCTIONS, MAX_INSTRUCTIONS + 1);
		}
		strip(line);
		if (!*line){
			continue;
		}
		line_num++;
		bool Atype = is_Atype(line);
		bool label = is_label(line);
		char inst_type;
		if (Atype){
			inst_type = 'A';
			if (!parse_A_instruction(line, &instr.instr.a)){
			    exit_program(EXIT_INVALID_A_INSTR, line_num, line);
			}
			instr.field = Atype;
			char *line_cut = line + 1;
			printf("A: %s\n", line_cut);
		}else if(label){
			inst_type = 'L';
			char line_new[strlen(line) + 1];
			extract_label(line,line_new);
			if (isalpha(*line_new) == 0){
				exit_program(EXIT_INVALID_LABEL, line_num + 2, line_new);
			}
			if (symtable_find(line_new) != NULL){
				exit_program(EXIT_SYMBOL_ALREADY_EXISTS, line_num, line_new);
			}
			strcpy(line,line_new);
			symtable_insert(line,instr_num);
			continue;
		}else{
			inst_type = 'C';
			char tmp_line[MAX_LINE_LENGTH] = {0};
			strcpy(tmp_line,line);
			parse_C_instruction(tmp_line, &instr.instr.c);
			if (instr.instr.c.dest == -1){
				exit_program(EXIT_INVALID_C_DEST, line_num + 2, line);
			}else if (instr.instr.c.comp == -1){
				exit_program(EXIT_INVALID_C_COMP, line_num + 2, line);
			}else if (instr.instr.c.jump == -1){
				exit_program(EXIT_INVALID_C_JUMP, line_num + 2, line);
			}
			printf("C: d=%d, c=%d, j=%d\n", instr.instr.c.dest, instr.instr.c.comp, instr.instr.c.jump);
		}
		//printf("%c",inst_type);
		//printf("%s", "  ");
		//printf("%s\n", line);
		//printf("%u: %c  %s\n", instr_num, inst_type, line);
		if (inst_type != 'L'){
			instructions[instr_num++] = instr;
		}
	}
	return instr_num;
}

void add_predefined_symbols(){
	for(int count = 0;count < NUM_PREDEFINED_SYMBOLS;count++){
		struct predefined_symbol symbol;
		symbol = predefined_symbols[count];
		symtable_insert(symbol.name,symbol.address);
	}
}

bool is_Atype(const char*line){
	if (*line == '@'){
		return true;
	}else{
		return false;
	}
}

bool is_label(const char*line){
	if (*line != '(' || *(line+strlen(line)-1) !=')'){
		return false;
	}else{
		return true;
	}
}

bool parse_A_instruction(const char *line, a_instruction *instr){
	char *s = malloc(sizeof(line));
	s[0] = *(line+1);
	char *s_end = malloc(sizeof(line));
	s_end = NULL;
	long result = strtol(s, &s_end, 10);
	if (s == s_end){
		instr->operand.label = malloc(sizeof(line));
		strcpy(s,instr->operand.label);
		instr->is_addr = false;
	}else if(*s_end != 0){
		return false;
	}else{
		instr->operand.address = result;
		instr->is_addr = true;
	}
	return true;
}

void parse_C_instruction(char *line, c_instruction *instr){
	char *token;
	char *token2;
	char *token3;
	char linecopy[MAX_LINE_LENGTH];
	strcpy(linecopy,line);
	//printf("%s","line:");
	//printf("%s\n",line);
	token = strtok(line,";");
	token2 = strtok(token,"=");
	int a = 0;
	//printf("%s","dest token:");
	//printf("%s\n", token2);
	//printf("%d\n", str_to_destid(token2));
	int tempdest = str_to_destid(token2);
	int tempcomp = str_to_compid(token2,&a);
	token2 = strtok(NULL, "=");
	if (token2){
		//printf("%s","comp token:");
		//printf("%s\n", token2);
		//printf("%d\n", str_to_compid(token2,&a));
		instr->dest = tempdest;
		instr->comp = str_to_compid(token2,&a);
		//printf("%d\n", a);
		instr->a = a;
		instr->jump = 0;
	}else{
		instr->a = a;
		instr->dest = 0;
		instr->comp = tempcomp;
		token3 = strtok(linecopy, ";");
		token3 = strtok(NULL, ";");
		//printf("%s","jump token:");
		if (token3){
			//printf("%s\n", token3);
			//printf("%d\n", str_to_jumpid(token3));
			instr->jump = str_to_jumpid(token3);
		}
	}
	//printf( " %s\n", token2);
}

char *extract_label(char *line, char* label){
	int i = 0;
	for (char *line2 = line; *line2; line2++) { 
   		if(*line2 != '(' && *line2 != ')'){
   			label[i++] = *line2;
   		}
	}
	label[i] = '\0';
	return label;
}

opcode instruction_to_opcode(c_instruction instr){
	opcode op = 0;
	op |= (7 << 13);
	return op;
}

void assemble(const char * file_name, instruction* instructions, int num_instructions){

}