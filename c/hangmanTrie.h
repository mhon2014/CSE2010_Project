#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ALPHABET_SIZE 26

#define CHAR_TO_INDEX(x) ((int)x - (int)'a') 
#define INDEX_TO_CHAR(x) (char)((int)x + (int)'a')

// node in the trie
typedef struct alphaNode {

  char letter;
  struct alphaNode *parent, *children[ALPHABET_SIZE]; // 26 children 
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


// initialize and return a new alpha node
AlphaNode* new_alpha_node() {

    AlphaNode* new_node = (AlphaNode*) malloc(sizeof(AlphaNode)); 

    // initialize parent and children pointers
    new_node->parent = NULL;
    for(int i = 0; i < ALPHABET_SIZE; i++){
        new_node->children[i] = NULL;
    } 

    // initialize other node fields
    new_node->depth = 0;
    new_node->is_candidate = true;
    new_node->end_of_word = false;

    return new_node;

}

//initialize and return a new trie
Trie *init_trie() { 

    Trie *newTrie = (Trie*)malloc(sizeof(Trie));

    newTrie->root = new_alpha_node();  // initialize root node
    newTrie->root->letter = '#';       // special character for root
    newTrie->nodeCount = 1;            // number of nodes in tree

    return newTrie;

} // end init_trie

// inserts given word into given trie
void insert(Trie *arg_trie, char *arg_word) {

    // variable declarations
    AlphaNode* new_node;                  // pointer for mallocing new nodes
    AlphaNode* current_node = arg_trie->root; // node pointer for traversal
    char current_letter;                  // stores letter as word is traversed

    // for each character in the word
    for (int i = 0; i < strlen(arg_word)-1; i++) {

        current_letter = arg_word[i];

        //if current node doesn't have this letter as child, initialize it
        if( (current_node->children[CHAR_TO_INDEX(current_letter)]) == NULL) {
            
            //initialise a new node
            new_node = new_alpha_node(); // initialize and allocate new node
            new_node->parent = current_node; // make it child of current node
            new_node->letter = current_letter; 
            new_node->depth = i+1;  // child level is one below current level
            current_node->children[CHAR_TO_INDEX(current_letter)] = new_node; // make parent point to new node
            arg_trie->nodeCount++; // increment size of trie
  
        } // end if

        current_node = current_node->children[CHAR_TO_INDEX(current_letter)]; // move to next level in trie
    
    } // end for loop

    current_node->end_of_word = true; // mark node of last letter as end_of_word
    
    return;

} // end insert function

// eliminate all paths that start with/ follow from the incorrect guess
void eliminate_ancestors(char bad_letter) {

  // recursively go through tree

  /// for every node equal to bad_letter (i.e. every ptr in "b" list)
  
      /// mark is_candidate false

      /// mark all ancestors with is_candidate false

  return;

} // close eliminate_ancestors


