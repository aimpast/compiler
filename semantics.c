#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "def.h"

/* Prototype initlization */
void SemanticalCheck(tree *root);
bool DeclVariable(tree *left, tree *right);
bool StorVariable(tree *nd, char *varname);

/* Semantical check the tree */
void SemanticalCheck(tree *root){
  
  switch(root->type){
  case EXP: case TERM: case FACTOR:
    SemanticalCheck(root->value.right);
    SemanticalCheck(root->left);
    break;

  case ID:
    if(!StorVariable(root, root->value.name))
      printf("error:'%s' undeclared\n",root->value.name);
    break;

  case DECTYPE: case REALTYPE: case CHARTYPE:
    if(!DeclVariable(root->left, root->value.right))
      printf("error:'%s' has been already declared\n",root->left->value.name);
    break;
  }
}

/* Declaration Variable or output error */
bool DeclVariable(tree *left, tree *right){
  char *lpnamespace;
  if(!(lpnamespace = insertVariable(left->value.name)))
    return False;

  left->value.name = lpnamespace;

  return True;
}

/* Store variable or output error */
bool StorVariable(tree *nd, char *name){
  char *lpnamespace;
  if(!(lpnamespace = chkIDConflict(name)))
    return False;

  nd->value.name=lpnamespace;
  return True;
}

int main(){
  char *name;
  name = (char*)malloc(sizeof(char)*4);
  name = strcpy(name,"Foo\0");
  tree *n1,*n2,*n3,*n4,*n5;

  n1=(tree*)malloc(sizeof(tree));
  n2=(tree*)malloc(sizeof(tree));
  n3=(tree*)malloc(sizeof(tree));
  n4=(tree*)malloc(sizeof(tree));

  n1->type = EXP;
  n1->left = n4;
  n1->value.right = n2;

  n2->type = DECTYPE;
  n2->value.right=NULL;
  n2->left = n3;

  n3->type = ID;
  n3->value.name = name;

  n4->type = ID;
  n4->value.right=name;

  initDatabase();
  SemanticalCheck(n1);
  closeDatabase();
  return 0;
}
