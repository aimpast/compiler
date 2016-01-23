#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "def.h"

/* Prototype declaration */
tree *getToken();
static tree *CreateTokenNode(Token tok, const char *instr, int word_count);

//デバッグ用プログラム
const char pgm[]="3*9+(4+2*3)-2*3\0";

static Token getKeyword(char *name){
  /* Reserved keywords */
  const char ResWrds[3][5]={
    "dec\0","char\0","real\0"
  };
  const Token ResTok[3]={
    DECTYPE,CHARTYPE,REALTYPE
  };

  Token result = ID;
  int i=3;

  while(i--)
    if(!strcmp(name,&ResWrds[i][0])) result = ResTok[i];

  return result;
}

static char *getString(const char *instr, int word_count){
  char *name;
  name = (char*)calloc(word_count+1,sizeof(char));
  return strncpy(name,instr,word_count);
}

static int GET_NUMBER_OF_FLAG(unsigned int flag){
  int i,j=1,result=0;
  for(i=0;i<32;i++){
    if(flag&j)result++;
    j*=2;
  }
  return result;
}

static bool GET_NUMBER_TOK(int flag,int offset,int i){
  return ((flag&2)==2 || i==0)&&
    (pgm[offset]>='0'&&pgm[offset]<='9');
}

/*static bool GET_EQU_TOK(int flag,int offset,int i){
  return i==0&&pgm[offset]=='=';
  }*/

static bool GET_ID_TOK(int flag,int offset,int i){
  return ( (flag&1) || i==0)&&
         (
	  (pgm[offset]>='a' && pgm[offset]>='z')||
	  (pgm[offset]>='A' && pgm[offset]>='Z')||
	  (i>0 && pgm[offset]>='0' && pgm[offset]<='9')
	 );
}

static Token flag2tok(int flag){
  Token tok;
  switch(flag)
    {
    case 1:
      tok=ID;
      break;
    case 2:
      tok=NUM;
      break;
    case 4:
      tok=EQU;
      break;
    case 8:
      tok=PLUS;
      break;
    case 16:
      tok=MINUS;
      break;
    case 32:
      tok=STAR;
      break;
    case 64:
      tok=SLASH;
      break;
    case 256:
      tok=LPAR;
      break;
    case 512:
      tok=RPAR;
      break;
    case 1024:
      tok=WHITE_SPACE;
      break;
    default:
      tok=NIL;
      break;
    }
  return tok;
}

tree *getToken(){
  typedef int FLAG;
  static int offset,prevoffset;
  Token token;
  FLAG prevflag=0,flag=0;
  int i=0;

  prevoffset=offset;
  do {
    prevflag=flag;

    if( GET_ID_TOK(flag,offset,i) )
      flag|=1;
    else flag&=~1;

    if( GET_NUMBER_TOK(flag,offset,i) )
      flag|=2;
    else flag&=~2;

    if(i!=0&&(flag&8)==8&&pgm[offset]=='=')
      flag|=128;
    else flag&=~128;
 
    if(i==0&&pgm[offset]=='=')
      flag|=4;
    else flag&=~4;

    if(i==0&&pgm[offset]=='+')
      flag|=8;
    else flag&=~8;

    if(i==0&&pgm[offset]=='-')
      flag|=16;
    else flag&=~16;

    if(i==0&&pgm[offset]=='*')
      flag|=32;
    else flag&=~32;

    if(i==0&&pgm[offset]=='/')
      flag|=64;
    else flag&=~64;

    if(i==0&&pgm[offset]=='(')
      flag|=256;
    else flag&=~256;

    if(i==0&&pgm[offset]==')')
      flag|=512;
    else flag&=~512;

    if(((flag&1024)==1024 || i==0)&&(pgm[offset]==' '||pgm[offset]=='\t')){
      flag|=1024;
    }else flag&=~1024;

    if(flag==0&&prevflag==0)break;

    if(flag!=0)
      offset++;

    i++;
  }while(!(flag==0 && GET_NUMBER_OF_FLAG(prevflag)==1));
  token=flag2tok(prevflag);

  if(token==WHITE_SPACE)
    return getToken();
 
  return CreateTokenNode(token, &pgm[prevoffset], offset-prevoffset);
}

static tree *CreateTokenNode(Token tok, const char *instr, int word_count){
  char *value;
  tree *n;

  n = (tree*)malloc(sizeof(tree));

  value = getString(instr,word_count);

  /* get token if it is reserved identifier */
  if(tok==ID)
    tok=getKeyword(value);

  n->type = tok;
  switch(tok){
  case ID:
    n->value.name = value;
    break;
  case NUM:
    n->value.ival = atoi(value);
    free(value);
    break;
  default:
    free(value);
    break;
  }

  return n;
}

