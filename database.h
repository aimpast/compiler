
#define MAX_OF_TABLE 1024     /* Max of the lookup table */
#define MAX_OF_WORD_COUNT 31  /* Max of word count in identifier */

typedef struct {
  Token type;                 /* Identifiers type */
  char *name;                 /* Pointer to namespace */
  unsigned long int address;  /* Variable Address */
}table;

typedef unsigned int hash;
