#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#include "def.h"

void prnToken(Token tok){
  switch(tok){
  case NUM:
    printf("NUMBER");
    break;
  case PLUS:
    printf("+");
    break;
  case MINUS:
    printf("-");
    break;
  case STAR:
    printf("*");
    break;
  case SLASH:
    printf("/");
    break;
  case LPAR:
    printf("(");
    break;
  case RPAR:
    printf(")");
    break;
  case NIL:
    printf("\\0");
    break;

  case Exp: case AddExp: case SubExp:
    printf("EXP");
    break;
  case Term: case MulTerm: case DivTerm:
    printf("TERM");
    break;
  case Factor:
    printf("FACTOR");
    break;
  }
}

static void ShowBranch(Token type){
  switch(type){
  case AddExp:
    puts("Exp(+)");
    break;
  case SubExp:
    puts("Exp(-)");
    break;
  case Exp:
    puts("Exp");
    break;

  case MulTerm:
    puts("Term(*)");
    break;
  case DivTerm:
    puts("Term(/)");
    break;
  case Term:
    puts("Term");
    break;

  case Factor:
    puts("Factor");
    break;
  }   
}

int prnTreeAndFreeup(tree *node){
  static int nest=0;
  int i;

  for(i=0;i<nest;i++)printf("  ");

  switch(node->type){

  case AddExp: case SubExp:
  case MulTerm: case DivTerm:
  case Exp: case Term: case Factor:
    ShowBranch(node->type);
    
    nest++;
    prnTreeAndFreeup(node->value.right);
    prnTreeAndFreeup(node->left);
    nest--;
    break;

  case NUM:
    printf("NUMBER(%d)\n",node->value.ival);
    break;

  case ID:
    printf("ID(%s)\n",node->value.name);
    break;
  }
  free(node);
  return 0;
}
