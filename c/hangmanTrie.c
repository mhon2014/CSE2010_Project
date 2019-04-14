#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ALPHABET_SIZE 26

#define INDEX(x) ((int)x - (int)'a')

//still using this one?
// typedef struct{
//     struct node *children;
//     bool path;
// }edge;

typedef struct node{

  char letter;

  struct node *parent,
              *next,
            //   *children;
              *children[ALPHABET_SIZE];
  unsigned int depth;

  bool EndOfWord;

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

//use array for 26 possible letters on each node
//or linked structure?



Node *node(){
    Node *newNode = (Node*)malloc(sizeof(Node));
    // newNode->letter = NULL;
    newNode->parent = NULL;
    newNode->next = NULL;
    // newNode->children = NULL;
    newNode->depth = 0;
    newNode->EndOfWord = false;
    for(int i = 0; i < ALPHABET_SIZE; i++){
        newNode->children[i] = NULL;
    } //I don't like this;
    return newNode;
}

Trie *initTrie(){ //initialize the trie.
    Trie *newTrie = (Trie*)malloc(sizeof(Trie));
    newTrie->root = node();
    newTrie->nodeCount = 1;
}

void insert(Trie *arg_trie, char *arg_word){

    Node *iterator = arg_trie->root; 

    //refactor to make it look nice pls
    for(int i = 0;i < strlen(arg_word)-2; ++i){

        //if empty
        //oh god this is disgusting.....
        if(!iterator->children[INDEX(arg_word[i])]){
            //initialise the new node
            Node *newNode = node();
            newNode->letter = arg_word[i];
        }
        iterator->children[INDEX(arg_word[i])];
        //some other conditions.....
        iterator->EndOfWord = true;
    }
}
