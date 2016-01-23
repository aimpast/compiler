#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "def.h"
#include "database.h"

/* Prototype Initlization */
void initDatabase(void);
void closeDatabase(void);
char *chkIDConflict(const char *name);
char *insertVariable(const char *name);
static hash getHash(const char *name);
static char *insertTable(const char *name,Token type,hash offset);

/* Global static Variable Declaration */
static table **database;
static char *namespace;

void initDatabase(void){
  database = (table**)calloc(MAX_OF_TABLE, sizeof(table*) );
  namespace = (char*)malloc(sizeof(char)*(MAX_OF_WORD_COUNT+1)*MAX_OF_TABLE);
}

void closeDatabase(void){
  int pointer=MAX_OF_TABLE;
  while(pointer--)
    if(database[pointer]) free(database[pointer]);
  free(database);
  free(namespace);
}

/* identifier is regarded as variable, and will insert into namespace */

char *insertVariable(const char *name){
  char *lpnamespace;
  lpnamespace = insertTable(name,VARIABLE,getHash(name));
  return lpnamespace;
}


/* Check identifier conflict */
char *chkIDConflict(const char *name){
  hash address = getHash(name);
  while(database[address]){
    if(!strcmp(database[address]->name,name))
      return database[address]->name;
    address++;
  }
  return NULL;
}

/* Get hash number from identifiers name */
static hash getHash(const char *name){
  hash hnumber=0;
  int i=0;

  while(name[i])
    hnumber+=pow(2,8-i%8)*name[i++];
  hnumber%=MAX_OF_TABLE;

  return hnumber;
}


/* insert Table infomation */
static char *insertTable(const char *name,Token type,hash offset){
  /*
    return value
     NULL:Table has already duplicated.
     POINTER:get the address to insert table 
   */
  static int char_offset=0;

  while(database[offset]){
    if(!strcmp(database[offset]->name,name))
      return NULL;
    offset++;
  }
  
  database[offset] = (table*)malloc(sizeof(table));
  database[offset]->type = type;
  database[offset]->name = &namespace[char_offset];
  database[offset]->address = 0;

  /* Copy to namespace*/
  strcpy(&namespace[char_offset],name);
  char_offset += strlen(name)+1;

  return database[offset]->name;
}

/*
int main(){
  char *var="Hi\0";
  char *s1;

  initDatabase();


  printf("%p\n",insertVariable(var));
  printf("%p\n",insertVariable(var));


  closeDatabase();
  return 0;
  }*/
