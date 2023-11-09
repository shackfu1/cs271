/****************************************
 * C-ploration 4 for CS 271
 * 
 * [NAME] $Sasha Hackenbruck$
 * [TERM] FALL $2023$
 * 
 ****************************************/
#include "parser.h"

#define MAX_LINE_LENGTH  200

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
void parse(FILE * file){
	char line[MAX_LINE_LENGTH] = {0};
	while (fgets(line, sizeof(line), file)){
		strip(line);
		if (!*line){
			continue;
		}
		bool Atype = is_Atype(line);
		bool label = is_label(line);
		char inst_type;
		if (Atype){
			//strcat(inst_type,'A');
			inst_type = 'A';
		}else if(label){
			//strcat(inst_type,'L');
			inst_type = 'L';
			char line_new[strlen(line) + 1];
			extract_label(line,line_new);
			strcpy(line,line_new);
		}else{
			//strcat(inst_type,'C');
			inst_type = 'C';
		}
		printf("%c",inst_type);
		printf("%s", "  ");
		printf("%s\n", line);
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