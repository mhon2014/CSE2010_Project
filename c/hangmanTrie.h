#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ALPHABET_SIZE 26

#define CHAR_TO_INDEX(x) ((int)x - (int)'a')
#define INDEX_TO_CHAR(x) (char)((int)x + (int)'a')

// node in the trie
typedef struct AlphaNode {

  char letter;
  struct AlphaNode *parent, *children[ALPHABET_SIZE];
  unsigned int depth;
  bool is_candidate; 
  bool end_of_word;

} AlphaNode;

// trie data type
typedef struct {

    AlphaNode *root;
    unsigned int nodeCount;

} Trie;

// node in list, storing pointer addresses
typedef struct PtrNode {

    AlphaNode* alpha_node_ptr;
    struct PtrNode* next;

 } PtrNode;

// function declarations
AlphaNode* new_alpha_node(); // initialize and return a new alpha node
Trie *init_trie(); //initialize and return a new trie
void insert(Trie *arg_trie, char *arg_word); // inserts given word into given trie
void eliminate_descendants(char bad_letter); // eliminate all paths that start with/ follow from the incorrect guess