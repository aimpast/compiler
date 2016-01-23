#include<stdio.h>
#include<memory.h>
#include<stdlib.h>
#include "def.h"

/* Prototype Declaration */
tree *getTree();
static void stdexp(tree **stack,tree *next);
static tree *abstraction(tree *target);
static void CreateFacLeaf(tree **stack);
static void CreateBranch(tree **stack,Token type);
static void CreateLeaf(tree **stack,Token type);

void prnToken(Token tok);
tree *getToken();
int prnTreeAndFreeup(tree *node);

/* Global Variable */
static int offset=0;

static void CreateBranch(tree **stack,Token type){
  /* EXP  -> EXP + TERM
     TERM -> TERM * FACTOR
   */
  tree *branch;
  branch = (tree*)malloc(sizeof(tree));
  branch->type = type;
  branch->value.right = abstraction(stack[offset]);
  branch->left = abstraction(stack[offset-2]);
  stack[offset-=2] = branch;
}

static void CreateLeaf(tree **stack,Token type){
  /* EXP  -> TERM
     TERM -> FACTOR
   */
  tree *leaf;
  leaf = (tree*)malloc(sizeof(tree));
  leaf->type = type;
  leaf->value.right = NULL;
  leaf->left = abstraction(stack[offset]);
  stack[offset] = leaf;
}

static tree *abstraction(tree *target){
  tree *leaf;
  if(!target->value.right){
    leaf = target->left;
    free(target);
  }
  else {
    leaf = target;
  }

  return leaf;
}

static void CreateFacLeaf(tree **stack){
  offset--;
  CreateLeaf(stack,Factor);
  offset--;
  stack[offset]=stack[offset+1];
}

#define AllExpBool(p) (p==AddExp||p==SubExp||p==Exp)
#define AllTermBool(p) (p==MulTerm||p==DivTerm||p==Term)
#define ExpOpe(p) (p==PLUS||p==MINUS)
#define TermOpe(p) (p==STAR||p==SLASH)

static void FactorProcessing(tree **stack,tree *next){
  if(offset>=2 &&
     stack[offset-2]->type==LPAR && stack[offset]->type==RPAR &&
     AllExpBool(stack[offset-1]->type) ){
    CreateFacLeaf(stack);
    //    puts("Factor -> (Exp)");
  }
  else {
    switch(stack[offset]->type){
    case ID: case NUM:
      //      if(stack[offset]->type == ID)puts("Factor -> ID");
      //      if(stack[offset]->type == NUM)puts("Factor -> NUM");
      CreateLeaf(stack,Factor);
      break;
    }
  }
}

static void TermProcessing(tree **stack,tree *next){
  if(offset>=2 &&
     AllTermBool(stack[offset-2]->type) && stack[offset]->type==Factor){
    switch(stack[offset-1]->type){
    case STAR:
      CreateBranch(stack,MulTerm);
      //      puts("Term -> Term * Factor");
      break;
    case SLASH:
      CreateBranch(stack,DivTerm);
      //      puts("Term -> Term / Factor");
      break;
    }
  }
  else if(stack[offset]->type == Factor){
      CreateLeaf(stack,Term);
      //      puts("Term -> Factor");
  }
}

static void ExpProcessing(tree **stack,tree *next){
  if(offset>=2 &&
     AllExpBool(stack[offset-2]->type) && AllTermBool(stack[offset]->type) && !TermOpe(next->type) ){
       switch(stack[offset-1]->type){
       case PLUS:
	 CreateBranch(stack,AddExp);
	 //	 puts("Exp -> Term + Factor");
	 break;
       case MINUS:
	 CreateBranch(stack,SubExp);
	 //	 puts("Exp -> Term - Factor");
	 break;
       }
  }
  else if(AllTermBool(stack[offset]->type) && !TermOpe(next->type)) {
       CreateLeaf(stack,Exp);
       //       puts("Exp -> Term");
     }
}

#undef AllExpBool
#undef AllTermBool
#undef ExpOpe
#undef TermOpe

static void stdexp(tree **stack, tree *next){
  FactorProcessing(stack,next);
  TermProcessing(stack,next);
  ExpProcessing(stack,next);
}

/* SLR(1) Syntax Analyzer */
tree *getTree() {
  tree **stack, *nextTok, *root;

  //initialize
  stack = (tree**)malloc(sizeof(tree*)*64);
  
  nextTok = getToken();
  while((stack[offset]=nextTok)->type){
    nextTok=getToken();
    stdexp(stack, nextTok);
    offset++;
  }

  root=stack[0];
  free(stack);

  return root;
}


