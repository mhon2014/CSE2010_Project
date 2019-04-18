/*****************************************
 * avl.h 
 * Interface of avl binary search tree
 * Josias Moukpe
 ****************************************/

#ifndef AVL_H
#define AVL_H 

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "trie.h"

typedef struct avlnode TNode;
typedef struct avltree Tree;

struct avlnode
{   
    byte    key;
    void    *data;
    TNode   *par, // pointer to par
            *left, //pointer to left 
            *right; // pointer to right

};

struct avltree {

    TNode* root;
    uint size;
};


/*********************************
 * PROTOTYPES
 * *****************************/

TNode* initTNode(byte key, void *data);
Tree* initTree();
void insertInTree(Tree* avl, char *word);
Trie findTrie(Tree* avl, uint word_length);


/********************************
 * DEFS
 * *****************************/


TNode* initTNode(byte key, void *data) {
    // TNode* new_node = (TNode*)malloc(sizeof(TNode));
    // new_node->data = data;
    // new_node->
}


Tree* initTree();
void insertInTree(Tree* avl, char *word);
Trie findTrie(Tree* avl, uint word_length);

#endif // AVL_H