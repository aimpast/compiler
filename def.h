
//Defined
typedef enum {
  /* terminal codes defined */
  NIL=0,ID,NUM,PLUS,MINUS,STAR,SLASH,PCT,LPAR,RPAR,EQU,

  /* Variable types defined */
  DECTYPE,CHARTYPE,REALTYPE,

  /* non-terminal codes defined */
  WHITE_SPACE,VARIABLE,
  AddExp,SubExp,Exp,
  MulTerm,DivTerm,Term,
  Factor
}Token;

typedef struct _tree {
  Token type;
  union _val {
    char *name;
    int ival;
    struct _tree *right;
  }value;
  struct _tree *left;
}tree;

typedef tree node;
typedef enum {
  True=1,
  False=0
} bool;
