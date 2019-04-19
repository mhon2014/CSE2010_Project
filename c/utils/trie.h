#ifndef TRIE_H
#define TRIE_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "sllist.h"

#define ALPHABET_SIZE 26

#define CHAR_TO_INDEX(x) ((int)x - (int)'a') 
#define INDEX_TO_CHAR(x) (char)((int)x + (int)'a')

// node in the trie
int counter = 0;
typedef unsigned char byte;

typedef struct anode {

    char letter;
    bool is_candidate, 
         end_of_word;
    byte depth;
    struct anode *parent;
    SLList *children; // 26 children at most
  
} ANode;

// trie data type
typedef struct {

    ANode *root;
    uint nodeCount;

} Trie;

// node in list, storing pointer addresses
// typedef struct PtrNode {

//     ANode* alpha_node_ptr;
//     struct PtrNode* next;

// } PtrNode;

// function declarations
ANode* init_anode(char a); // initialize and return a new alpha node
Trie *init_trie(); //initialize and return a new trie
void insertTrie(Trie *arg_trie, char *arg_word); // inserts given word into given trie
void eliminate_paths(Trie* trie, SLList** letter_freq, char bad_letter); // eliminate all paths that start with/ follow from the incorrect guess
char highestFreqLetter(Trie* trie, SLList **letter_freq, bool* guessLetters);
// PtrNode* initNode(ANode* data); 
void preOrder(ANode* root, SLList** letter_freq);
void insortAnode(SLList* List,  void* data);
void insortAnodeAlph(SLList* List,  void* data);
void elimDown(ANode* root); // eliminate path down
void elimUp(ANode* root); // eliminate path up
bool hasOneChild(ANode* root); // return true if only one child
ANode* findAnode(SLList* children, char letter);
void resetPaths(ANode* root);
 
// initialize and return a new alpha node
ANode* init_anode(char val) {

    ANode* new_node = (ANode*)malloc(sizeof(ANode)); 
    // initialize other node fields
    new_node->parent = NULL;
    new_node->letter = val;
    new_node->depth = 0;
    new_node->is_candidate = true;
    new_node->end_of_word = false;
    new_node->children = initList();

    return new_node;

}

//initialize and return a new trie
Trie *init_trie() { 

    Trie *newTrie = (Trie*)malloc(sizeof(Trie));

    newTrie->root = init_anode('#');  // initialize root node
    // newTrie->root->letter = '#';       // special character for root
    newTrie->nodeCount = 1;            // number of nodes in tree
    return newTrie;

} // end init_trie

// inserts given word into given trie
void insertTrie(Trie *arg_trie, char *arg_word) {

    // variable declarations
    ANode* new_node;                  // pointer for mallocing new nodes
    ANode* current_node = arg_trie->root; // node pointer for traversal
    char current_letter;                  // stores letter as word is traversed

    // for each character in the word
    for (int i = 0; i < strlen(arg_word); i++) {

        current_letter = arg_word[i];

        //if current node doesn't have this letter as child, initialize it
        // if((current_node->children[CHAR_TO_INDEX(current_letter)]) == NULL) {
        if(findAnode(current_node->children, current_letter) == NULL) {
            //initialise a new node
            new_node = init_anode(current_letter); // initialize and allocate new node
            new_node->parent = current_node; // make it child of current node
            // new_node->letter = current_letter; 
            new_node->depth = i+1;  // child level is one below current level
            // current_node->children[CHAR_TO_INDEX(current_letter)] = new_node; // make parent point to new node
            insortAnodeAlph(current_node->children, new_node);
            arg_trie->nodeCount++; // increment size of trie
  
        } // end if
        // current_node = current_node->children[CHAR_TO_INDEX(current_letter)]; // move to next level in trie
        current_node = findAnode(current_node->children, current_letter);
    
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
        elimUp((ANode*)bad_root->data); /// mark all ancestors with is_candidate false
        elimDown((ANode*)bad_root->data); /// mark is_candidate false

    }

    return;

} // close eliminate_ancestors

void elimUp(ANode* root) { // root not included
    for(ANode* tmp = root->parent; tmp->letter != '#'; tmp = tmp->parent) {
        if(!hasOneChild(tmp)) break;
        tmp->is_candidate = false;
    }
}

void elimDown(ANode* root) { // root included
    if(root == NULL) return;

    root->is_candidate = false;
    // for(uint i = 0; i < ALPHABET_SIZE; ++i)
    //     elimDown(root->children[i]);

    for(Node* cursor = root->children->head; cursor != NULL; cursor = cursor->next) {
        ANode *child = (ANode*)cursor->data;
        elimDown(child);
    }
}

char highestFreqLetter(Trie* trie, SLList** letter_freq, bool* guessedLetters) {
    // printf("Guessing... \n");
    // ANode* start = trie->root;
    byte max = 0;

    // for(byte i = 0; i < ALPHABET_SIZE; ++i) {
    //     preOrder(start->children[i], letter_freq, word_len);
    // }
    for(Node* cursor = trie->root->children->head; cursor != NULL; cursor = cursor->next) {
        ANode *child = (ANode*)cursor->data;
        preOrder(child, letter_freq);
    }

    // return max freq letter
    for(byte i = 0; i < ALPHABET_SIZE; ++i) {
        if(letter_freq[i]->size > letter_freq[max]->size && !guessedLetters[i]) {
            max = i;
        }
    }

    return INDEX_TO_CHAR(max);
}

void preOrder(ANode* root, SLList** letter_freq) {
    if(root == NULL) return;
    // if(root->depth > word_len) return;
    if(root->is_candidate) {
        // printf(" curr node: %c  number: %d\n", root->letter, counter++);
        insortAnode(letter_freq[CHAR_TO_INDEX(root->letter)], root);
    }
    // for(uint i = 0; i < ALPHABET_SIZE; ++i) {
    //     preOrder(root->children[i], letter_freq, word_len);
    // }
    for(Node* cursor = root->children->head; cursor != NULL; cursor = cursor->next) {
        ANode *child = (ANode*)cursor->data;
        preOrder(child, letter_freq);
    }
}

// bool search(struct TrieNode *root, const char *key) 
// { 
//     int level; 
//     int length = strlen(key); 
//     int index; 
//     ANode *pCrawl = root; 
  
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
void insortAnode(SLList* List,  void* data) {
    // printf("gotcha bitch!\n");
    Node* new_node = initNode(data);
    if(List->size == 0) { //case 1: empty list
        List->head = new_node;
        new_node->next = NULL;
    } 
    // else if(strcmp((char*)List->head->data, data) > 0) { // inserting before the head
    else if(((ANode*)List->head->data)->depth > ((ANode*)data)->depth) {
        new_node->next = List->head;
        List->head = new_node;
    }   
    else { // case 3: insertTrie sort
        // for(prev = List->head; strcmp((char*)prev->next->data, data) < 0; prev = prev->next);
        Node* prev = List->head;
        while(prev->next != NULL && 
        ((ANode*)prev->next->data)->depth > ((ANode*)data)->depth) {
            prev = prev->next;
        }
        new_node->next = prev->next;
        prev->next = new_node;
    }

    ++List->size;
}

/* insertTrie a node with data at index */
void insortAnodeAlph(SLList* List,  void* data) {
    // printf("gotcha bitch!\n");
    Node* new_node = initNode(data);
    if(List->size == 0) { //case 1: empty list
        List->head = new_node;
        new_node->next = NULL;
    } 
    // else if(strcmp((char*)List->head->data, data) > 0) { // inserting before the head
    else if(((ANode*)List->head->data)->letter > ((ANode*)data)->letter) {
        new_node->next = List->head;
        List->head = new_node;
    }   
    else { // case 3: insertTrie sort
        // for(prev = List->head; strcmp((char*)prev->next->data, data) < 0; prev = prev->next);
        Node* prev = List->head;
        while(prev->next != NULL && 
        ((ANode*)prev->next->data)->letter > ((ANode*)data)->letter) {
            prev = prev->next;
        }
        new_node->next = prev->next;
        prev->next = new_node;
    }

    ++List->size;
}

bool hasOneChild(ANode* root) { // sorry Mr Chan
    // byte counter = 0;
    // for(byte i = 0; i < ALPHABET_SIZE; ++i) {
    //     if(root->children[i] != NULL) ++counter;
    // }
    return (root->children->size == 1);
}

ANode* findAnode(SLList* children, char letter) {
    if(children->size == 0) return NULL;
    for(Node* cursor = children->head; cursor != NULL; cursor = cursor->next) {
        ANode *child = (ANode*)cursor->data;
        if(child->letter == letter) return child;
    }
    return NULL;
}

void resetPaths(ANode* root) {
    if(root == NULL) return;
    root->is_candidate = true;
    // for(uint i = 0; i < ALPHABET_SIZE; ++i) {
    //     preOrder(root->children[i], letter_freq, word_len);
    // }
    for(Node* cursor = root->children->head; cursor != NULL; cursor = cursor->next) {
        ANode *child = (ANode*)cursor->data;
        resetPaths(child);
    }
}
#endif // TRIE_H  