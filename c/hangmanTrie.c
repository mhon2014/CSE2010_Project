#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define alphabetSize 26

//still using this one?
// typedef struct{
//     struct node *children;
//     bool path;
// }edge;

typedef struct node{

  char letter;

  struct node *parent,
              *next,
              *children;

  unsigned int depth;

  bool EndOfWord

}Node;

//DLL, not used yet
typedef struct{
    Node *head,
         *tail;

    unsigned int size;
}List;

typedef struct {
    Node *root;
    unsigned int nodeCount;
    //max depth????
}Trie;

//SUPER ROUGH DRAFT pls don't hurt me



Node *node(){
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->letter = NULL;
    newNode->parent = NULL;
    newNode->next = NULL;
    newNode->children = NULL;
    newNode->depth = 0;
    newNode->EndOfWord = false;
    return newNode;
}

Trie *initTrie(){ //initialize the trie.
    Trie *newTrie = (Trie*)malloc(sizeof(Trie));
    newTrie->root = node();
    newTrie->nodeCount = 1;
}

void insert(Trie *arg_trie, char *arg_letter){

    //refactor to make it look nice pls
    for(int i = 0;i < strlen(arg_letter)-2; ++i){

        //if empty
        if(arg_trie->root->children == NULL){
            //initialise the new node
            Node *newNode = node();
            newNode->letter = arg_letter[i];
        }
        //some other conditions.....
    }
     //to mark the end_string flag for this string
    newNode->EndOfWord =true;
}

}