/*****************************************
 * sllist.h 
 * Interface of linked list
 * Josias Moukpe
 ****************************************/


#ifndef SLLIST_H
#define SLLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SIZE 32 //size of data stored in each node

typedef unsigned int uint; //unsigned integer alias
typedef struct node Node_t; // node alias
typedef struct sllist SLList_t; // list alias

struct node 
{
    void* data; // data carried by node
    struct node *next; // pointer to next
};

struct sllist 
{
    uint size; // size of the list
    struct node* head; // pointer to parent
};


/****************************************
 * METHODS PROTOTYPES
 ***************************************/ 

// NODE METHODS
Node_t* initNode(void* data);
void destroyNode(Node_t* to_del); // clear memory of ode

//LIST METHODS

SLList_t* initList(); // initialize a list of TNodes
SLList_t* parseWords(char* line); //parse the input into SLList_t
Node_t* traverse(SLList_t* List,  uint index);  //return the pointer to the node previous to the node at index 
void destroyList(SLList_t* to_del); //clear memory of Taxonomic node
void insert(SLList_t* List, uint index, void* data); //_insert a node with payload data at position index
void insort(SLList_t* List, void* data); //_insert a node with payload data at position index
void pushfront(SLList_t* List, void* data); //_insert node at the front of the list
void pushback(SLList_t* List, void* data); //_insert node at the end of the list
void printlist(SLList_t* List); //print all elements in the list
void* suppress(SLList_t* List, uint index);  //deletes a node at position index
void* popfront(SLList_t* List);  //delete node at the back of the list
void* popback(SLList_t* List);  //delete node at the front of the list
void* getAt(SLList_t* List, uint index); //get the data at index 


/****************************************
 * Node_t Methods
 ***************************************/ 


/* Creates a new node for the tree */
Node_t* initNode(void* data) {
    Node_t* new_node = (Node_t*)malloc(sizeof(Node_t));
    // new_node->data = malloc(SIZE*sizeof(char));
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

/* Destroy to_del node */ 
void destroyNode(Node_t* to_del) {
   free(to_del);
}




/*******************************************
 * SLList_t Methods
 ******************************************/


/* get data at index */
void* getAt(SLList_t* List,  uint index) {
    return (traverse(List, index)->data);
}


/* initialize size to 0 and head to NULL */
SLList_t* initList(void) {
   SLList_t* List = (SLList_t*)malloc(sizeof(SLList_t));
   List->size = 0;
   List->head = NULL;
   return List;
}

/* clear all nodes in the list then the list itself */
void destroyList(SLList_t* List) {
    while(List->size != 0) popback(List);
    free(List);
}


/* print all elements in the list */
void printlist(SLList_t* List) {
    if(List == NULL || List->size == 0 ) {
        printf("empty list\n");
        return;
    }

    
    if(List->size != 0) {
        for(Node_t* tmp = List->head; tmp != NULL; tmp = tmp->next) {
            printf("%s ", (char*)(tmp->data));
        }
    }
}

/* give an index N, traverse() traverse the list until N and return pointer to N-1 */
Node_t* traverse(SLList_t* List,  uint index) {
    // case 1: empty list
    if(List->size == 0) {
        printf("empty list!\n"); 
        return NULL;
    }
    // case 2: index is not present
    if(index < 0 || index >= List->size) {
        printf("Invalid index!\n");
        return NULL;
    }

    // case 3: index present
    Node_t* tmp = List->head; 
    for(uint i = 0; i < index; ++i) tmp = tmp->next; 

    return tmp;
}

/* insert a node with data at index */
void insert(SLList_t* List,  uint index, void* data) {
    Node_t* new_node = initNode(data);
    if(List->size == 0) { //case 1: empty list
        List->head = new_node;
        new_node->next = NULL;
    } 
    else 
    if(index == 0) { //case 2: front _insertion
        new_node->next = List->head;
        List->head = new_node;
    } 
    else { //case 3: Non empty list with _insert at middle
        Node_t* prev = traverse(List, index);
        new_node->next = prev->next;
        prev->next = new_node;
    }

    ++List->size;
}



/* insert a node with data at index */
void insort(SLList_t* List,  void* data) {
    Node_t* new_node = initNode(data);
    if(List->size == 0) { //case 1: empty list
        List->head = new_node;
        new_node->next = NULL;
    } 
    else if(strcmp((char*)List->head->data, (char*)data) > 0) { // inserting before the head
        new_node->next = List->head;
        List->head = new_node;
    }   
    else { // case 3: insert sort
        Node_t* prev = NULL;
        for(prev = List->head; strcmp((char*)prev->next->data, (char*)data) < 0; prev = prev->next);
        new_node->next = prev->next;
        prev->next = new_node;
    }

    ++List->size;
}

/* insert node at the front of the list */
void pushfront(SLList_t* List, void* data) {
   insert(List, 0, data);
} 


/* insert node at the end of the list */
void pushback(SLList_t* List, void* data) {
    if(List->size == 0) {
        Node_t* new_node = initNode(data);
        new_node->next = NULL;
        List->head = new_node;
        ++List->size;
    }
    else 
    if(List->size == 1) { //edge case for single node list
        Node_t* new_node = initNode(data);
        new_node->next = List->head->next;
        List->head->next = new_node;
        ++List->size;

    } else insert(List, List->size - 1, data);
}  


/* suppress() deletes node at index */ 
void* suppress(SLList_t* List, uint index) {
    //case 1: empty list
    if(List->size == 0) {
        printf("empty list!\n");
        return NULL;
    }
    //case 2: invalid index
    if(index < 0 || index >= List->size) {
        printf("invalid index");
        return NULL;
    }

    Node_t* to_del;
    void* tmp = NULL;
    
    if(index == 0) { //case 3: deleting head
       to_del = List->head;
       List->head = (List->head)->next;

    } 
    else {  //case 4: deleting any node that is not head
        Node_t* prev = traverse(List, index - 1);
        to_del = prev->next;
        prev->next = to_del->next;

    }

    tmp = to_del->data;
    free(to_del);
    --List->size;
    return tmp;
}

/* delete node at the back of the list */
void* popfront(SLList_t* List) {
    return suppress(List,0);
}  


/* delete node at the front of the list */
void* popback(SLList_t* List) {
    return suppress(List, List->size - 1);
}  


/* Parses the input line for relevant commands */ 
SLList_t* parseWords(char* line) {
    SLList_t* tmp = initList(); //command created with be used by then freed by parseCmd()
    char* word_token;
    char* delim = " \r\n";

    //get the first token 
    word_token = strtok(line, delim);

    //walk through other tokens 
    while(word_token != NULL) {
        //add new words to end of the list
        char* data = (char*)malloc(SIZE*sizeof(char));
        strcpy(data, word_token);
        pushback(tmp, data); 
        word_token = strtok(NULL, delim);
    }
    return tmp; //return pointer to list containing commands
} 


#endif //SLLIST_H
