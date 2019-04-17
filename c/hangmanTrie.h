#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "utils/sllist.h"

#define ALPHABET_SIZE 26

#define CHAR_TO_INDEX(x) ((int)x - (int)'a') 
#define INDEX_TO_CHAR(x) (char)((int)x + (int)'a')

// node in the trie
int counter = 0;
typedef unsigned char byte;

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
// typedef struct PtrNode {

//     AlphaNode* alpha_node_ptr;
//     struct PtrNode* next;

// } PtrNode;

// function declarations
AlphaNode* new_alpha_node(); // initialize and return a new alpha node
Trie *init_trie(); //initialize and return a new trie
void insertTrie(Trie *arg_trie, char *arg_word); // inserts given word into given trie
void eliminate_paths(Trie* trie, SLList** letter_freq, char bad_letter); // eliminate all paths that start with/ follow from the incorrect guess
SLList* highestFreqLetter(Trie* trie, SLList **letter_freq, bool* guessLetters);
// PtrNode* initNode(AlphaNode* data); 
void preOrder(AlphaNode* root, SLList** letter_freq);
void insort(SLList* List,  void* data);
void elimDown(AlphaNode* root); // eliminate path down
void elimUp(AlphaNode* root); // eliminate path up
bool isChineseNode(AlphaNode* root); // return true if only one child
 
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
void insertTrie(Trie *arg_trie, char *arg_word) {

    // variable declarations
    AlphaNode* new_node;                  // pointer for mallocing new nodes
    AlphaNode* current_node = arg_trie->root; // node pointer for traversal
    char current_letter;                  // stores letter as word is traversed

    // for each character in the word
    for (int i = 0; i < strlen(arg_word); i++) {

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

} // end insertTrie function

// eliminate all paths that start with/ follow from the incorrect guess
void eliminate_paths(Trie* trie, SLList** letter_freq, char bad_letter) {

  // recursively go through tree

  /// for every node equal to bad_letter (i.e. every ptr in "b" list)
    SLList* bad_roots = letter_freq[CHAR_TO_INDEX(bad_letter)];
    for(Node* bad_root = bad_roots->head; bad_root != NULL; bad_root = bad_root->next) {
        elimUp((AlphaNode*)bad_root->data); /// mark all ancestors with is_candidate false
        elimDown((AlphaNode*)bad_root->data); /// mark is_candidate false

    }

    return;

} // close eliminate_ancestors

void elimUp(AlphaNode* root) { // root not included
    for(AlphaNode* tmp = root->parent; tmp->letter != '#'; tmp = tmp->parent) {
        if(!isChineseNode(tmp)) break;
        tmp->is_candidate = false;
    }
}

void elimDown(AlphaNode* root) { // root included
    if(root == NULL) return;
    root->is_candidate = false;
    for(uint i = 0; i < ALPHABET_SIZE; ++i)
        elimDown(root->children[i]);
}

SLList* highestFreqLetter(Trie* trie, SLList** letter_freq, bool* guessedLetters) {
    printf("Guessing... \n");
    AlphaNode* start = trie->root;
    byte max = 0;

    for(byte i = 0; i < ALPHABET_SIZE; ++i) {
        preOrder(start->children[i], letter_freq);
    }

    for(byte i = 0; i < ALPHABET_SIZE; ++i) {
        if(letter_freq[i]->size > letter_freq[max]->size && !guessedLetters[i]) {
            max = i;
        }
    }

    return letter_freq[max];
}

void preOrder(AlphaNode* root, SLList** letter_freq) {
    if(root == NULL) return;
    if(root->is_candidate) {
        printf(" curr node: %c  number: %d\n", root->letter, counter++);
        insort(letter_freq[CHAR_TO_INDEX(root->letter)], root);
    }
    for(uint i = 0; i < ALPHABET_SIZE; ++i) {
        preOrder(root->children[i], letter_freq);
    }
}

// bool search(struct TrieNode *root, const char *key) 
// { 
//     int level; 
//     int length = strlen(key); 
//     int index; 
//     AlphaNode *pCrawl = root; 
  
//     for (level = 0; level < length; level++) 
//     { 
//         index = CHAR_TO_INDEX(key[level]); 
  
//         if (!pCrawl->children[index]) 
//             return false; 
  
//         pCrawl = pCrawl->children[index]; 
//     } 
  
//     return (pCrawl != NULL && pCrawl->end_of_word); 
// } 

/* insertTrie a node with data at index */
void insort(SLList* List,  void* data) {
    // printf("gotcha bitch!\n");
    Node* new_node = initNode(data);
    if(List->size == 0) { //case 1: empty list
        List->head = new_node;
        new_node->next = NULL;
    } 
    // else if(strcmp((char*)List->head->data, data) > 0) { // inserting before the head
    else if(((AlphaNode*)List->head->data)->depth > ((AlphaNode*)data)->depth) {
        new_node->next = List->head;
        List->head = new_node;
    }   
    else { // case 3: insertTrie sort
        // for(prev = List->head; strcmp((char*)prev->next->data, data) < 0; prev = prev->next);
        Node* prev = List->head;
        while(prev->next != NULL && 
        ((AlphaNode*)prev->next->data)->depth > ((AlphaNode*)data)->depth) {
            prev = prev->next;
        }
        new_node->next = prev->next;
        prev->next = new_node;
    }

    ++List->size;
}

bool isChineseNode(AlphaNode* root) { // sorry Mr Chan
    byte counter = 0;
    for(byte i = 0; i < ALPHABET_SIZE; ++i) {
        if(root->children[i] != NULL) ++counter;
    }
    return (counter == 1);
}