#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ALPHABET_SIZE 26

#define CHAR_TO_INDEX(x) ((int)x - (int)'a')
#define INDEX_TO_CHAR(x) (char)((int)x + (int)'a')

//still using this one?
// typedef struct{
//     struct node *children;
//     bool path;
// }edge;


//suppose to be function defintions only but I got lazy


typedef struct node{

  char letter;

  struct node *parent,
            //   *next, //if using linked list
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
    // array of nodes / depth??
    //max depth????
}Trie;

//SUPER ROUGH DRAFT pls don't hurt me

//use array for 26 possible letters on each node
//or linked structure?



Node *node(){
    Node *newNode = (Node*)malloc(sizeof(Node));
    // newNode->letter = NULL;
    newNode->parent = NULL;
    // newNode->children = NULL;
    newNode->depth = 0;
    newNode->EndOfWord = false;
    for(int i = 0; i < ALPHABET_SIZE; i++){
        newNode->children[i] = NULL;
    } //I don't like this AT ALL
    return newNode;
}


Trie *initTrie(){ //initialize the trie.
    Trie *newTrie = (Trie*)malloc(sizeof(Trie));
    newTrie->root = node();
    newTrie->nodeCount = 1;
    return newTrie;
}

void insert(Trie *arg_trie, char *arg_word){

    Node *iterator = arg_trie->root; 
    // printf("\nlength: %lu - ASCII value: %d - word: %s \n", strlen(arg_word)-2, arg_word[0], arg_word);

    // return;
    //refactor to make it look nice pls
    for(int i = 0;i < strlen(arg_word)-2; ++i){
        //if empty
        //oh god this is disgusting.....
        if(!iterator->children[CHAR_TO_INDEX(arg_word[i])]){
            //initialise the new node
            printf("parent: %c - depth: %d\n", iterator->letter, iterator->depth);
            Node *newNode = node();
            newNode->parent = iterator;
            newNode->letter = arg_word[i];
            newNode->depth = i+1;
            iterator->children[CHAR_TO_INDEX(arg_word[i])] = newNode;
            arg_trie->nodeCount++;
        }
        iterator = iterator->children[CHAR_TO_INDEX(arg_word[i])];
        // printf("letter: %c\n", iterator->letter);

        iterator->EndOfWord = true;
    }
}