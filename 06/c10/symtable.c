/****************************************
 * C-ploration 4 for CS 271
 * 
 * [NAME] $Sasha Hackenbruck$
 * [TERM] FALL $2023$
 * 
 ****************************************/

#define SYMBOL_TABLE_SIZE 1000

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "symtable.h"

struct Symbol* hashArray[SYMBOL_TABLE_SIZE]; 

int hash(char *str){
	unsigned long hashnum = 5381;
    int c;

    while ((c = *str++)){
        hashnum = ((hashnum << 5) + hashnum) + c; /* hashnum * 33 + c */
    }
    //hashnum = hashnum % SYMBOL_TABLE_SIZE;
    return hashnum % SYMBOL_TABLE_SIZE;
}

struct Symbol *symtable_find(char * key){
	//get the hash 
   int hashIndex = hash(key);  
	
   //move in array until an empty 
   while(hashArray[hashIndex] != NULL) {
	
      if(strcmp(hashArray[hashIndex]->name,key)){
         return hashArray[hashIndex]; 
      }
			
      //go to next cell
      ++hashIndex;
		
      //wrap around the table
      hashIndex %= SYMBOL_TABLE_SIZE;
   }        
	
   return NULL;  
}

void symtable_insert(char* key, hack_addr addr){
	struct Symbol *item = (struct Symbol*) malloc(sizeof(struct Symbol));
   item->addr = addr;  
   item->name = strdup(key);

   //get the hash 
   int hashIndex = hash(key);
   //move in array until an empty or deleted cell
   while(hashArray[hashIndex] != NULL && hashArray[hashIndex]->name) {
      //go to next cell
      ++hashIndex;
		
      //wrap around the table
      hashIndex %= SYMBOL_TABLE_SIZE;
   }
   hashArray[hashIndex] = item;
}

void symtable_display_table(){
	int i = 0;
	
   for(i = 0; i<SYMBOL_TABLE_SIZE; i++) {
	
      if(hashArray[i] != NULL)
         printf(" (%d,%s)",hashArray[i]->addr,hashArray[i]->name);
      else
         printf(" ~~ ");
   }
	
   printf("\n");
}

void symtable_print_labels(){
	int i = 0;    
   for(i = 0; i<SYMBOL_TABLE_SIZE; i++) {
        if(hashArray[i] != NULL){
             printf("{%s,%d}\n",hashArray[i]->name,hashArray[i]->addr);
        }
   }
}