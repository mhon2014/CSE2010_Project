/*****************************************
 * dllist.h 
 * Interface of linked list
 * Josias Moukpe
 ****************************************/


#ifndef DLLIST_H
#define DLLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SIZE 32 //size of data stored in each node

typedef unsigned int uint; //unsigned integer alias
typedef unsigned short ushort;
typedef struct node Node_t; // node alias
typedef struct dllist DLList_t; // list alias

struct node 
{
    void* data; // data carried by node
    struct node *next, // pointer to previous
                *prev; // pointer to next
};

struct dllist 
{
    uint size; // size of the list
    struct node *head, // pointer to head
                *tail; // pointer to tail
};


/****************************************
 * METHODS PROTOTYPES
 ***************************************/ 

// NODE METHODS
Node_t* initNode(void* data);
void destroyNode(Node_t* to_del); // clear memory of ode

//LIST METHODS

DLList_t* initList(); // initialize a list of TNodes
DLList_t* parseWords(char* line); //parse the input into DLList_t
Node_t* traverse(DLList_t* List,  uint index);  //return the pointer to the node previous to the node at index 
void destroyList(DLList_t* to_del); //clear memory of Taxonomic node
void insert(DLList_t* List, uint index, void* data); //_insert a node with payload data at position index
void insort(DLList_t* List, void* data); //_insert a node with payload data at position index
void pushfront(DLList_t* List, void* data); //_insert node at the front of the list
void pushback(DLList_t* List, void* data); //_insert node at the end of the list
void printlist(DLList_t* List); //print all elements in the list
void* suppress(DLList_t* List, uint index);  //deletes a node at position index
void* popfront(DLList_t* List);  //delete node at the back of the list
void* popback(DLList_t* List);  //delete node at the front of the list
void* getAt(DLList_t* List, uint index); //get the data at index 
void* remov(Node_t* to_del);
void* removFromList(DLList_t* List, Node_t* to_del);


/****************************************
 * Node_t Methods
 ***************************************/ 


/* Creates a new node for the tree */
Node_t* initNode(void* data) {
    Node_t* new_node = (Node_t*)malloc(sizeof(Node_t));
    // new_node->data = malloc(SIZE*sizeof(char));
    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = NULL;
    return new_node;
}

/* Destroy to_del node */ 
void destroyNode(Node_t* to_del) {
   free(to_del);
}




/*******************************************
 * DLList_t Methods
 ******************************************/


/* get data at index */
void* getAt(DLList_t* List,  uint index) {
    return (traverse(List, index)->data);
}


/* initialize size to 0 and head to NULL */
DLList_t* initList(void) {
   DLList_t* List = (DLList_t*)malloc(sizeof(DLList_t));
   List->size = 0;
   List->head = NULL;
   List->tail = List->head;
   return List;
}

/* clear all nodes in the list then the list itself */
void destroyList(DLList_t* List) {
    while(List->size != 0) popback(List);
    free(List);
}


/* print all elements in the list */
void printlist(DLList_t* List) {
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
Node_t* traverse(DLList_t* List,  uint index) {
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
    Node_t* tmp;
    if(index < (List->size / 2)) {
        tmp = List->head; 
        for(uint i = 0; i <= index; ++i) tmp = tmp->next; 
    }
    else {// index >= List->size / 2 
        tmp = List->tail; 
        for(uint i = List->size - 1; i >= index; --i) tmp = tmp->prev; 
    }
    return tmp;
}

/* insert a node with data at index */
void insert(DLList_t* List,  uint index, void* data) {
    Node_t* new_node = initNode(data);
    if(List->size == 0) { //case 1: empty list
        List->head = new_node;
        List->tail = new_node;
        new_node->next = NULL;
        new_node->prev = NULL;
    } 
    else 
    if(index == 0) { //case 2: front _insertion
        new_node->next = List->head;
        List->head->prev = new_node;
        List->head = new_node;
        new_node->prev = NULL;
    } 
    else 
    if(index >= List->size) {
        new_node->prev = List->tail;
        List->tail->next = new_node;
        List->tail = new_node;
        new_node->next = NULL;
    }
    else { //case 3: Non empty list with _insert at middle
        Node_t* after = traverse(List, index);
        new_node->next = after;
        new_node->prev = after->prev;
        after->prev->next = new_node;
        after->prev = new_node;
    }

    ++List->size;
}



/* insert a node with data at index */
void insort(DLList_t* List,  void* data) {
    Node_t* new_node = initNode(data);
    if(List->size == 0) { //case 1: empty list
        List->head = new_node;
        List->tail = new_node;
        new_node->next = NULL;
        new_node->next = NULL;
    } 
    else if(strcmp((char*)List->head->data, (char*)data) > 0) { // inserting before the head
        new_node->next = List->head;
        List->head->prev = new_node;
        List->head = new_node;
    }   
    else { // case 3: insert sort
        Node_t* prev = NULL;
        for(prev = List->head; strcmp((char*)prev->next->data, (char*)data) < 0; prev = prev->next);
        new_node->next = prev->next;
        new_node->prev = prev;
        prev->next->prev = new_node;
        prev->next = new_node;
    }

    ++List->size;
}

/* insert node at the front of the list */
void pushfront(DLList_t* List, void* data) {
   insert(List, 0, data);
} 


/* insert node at the end of the list */
void pushback(DLList_t* List, void* data) {
    if(List->size == 0) {
        Node_t* new_node = initNode(data);
        new_node->next = NULL;
        new_node->prev = NULL;
        List->head = new_node;
        List->tail = new_node;
        ++List->size;
    }
    else insert(List, List->size, data);
    // if(List->size == 1) { //edge case for single node list
    //     Node_t* new_node = initNode(data);
    //     new_node->next = List->head->next;
    //     List->head->next = new_node;
    //     ++List->size;

    // } else insert(List, List->size - 1, data);
    
}  


/* suppress() deletes node at index */ 
void* suppress(DLList_t* List, uint index) {
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
    else
    if(index == List->size - 1) {
        to_del = List->tail;
       List->tail = (List->tail)->prev;
    }
    else {  //case 4: deleting any node that is not head or tail
        to_del = traverse(List, index);
    }
    --List->size;
    return remov(to_del);
}



/* delete node at the back of the list */
void* popfront(DLList_t* List) {
    return suppress(List,0);
}  


/* delete node at the front of the list */
void* popback(DLList_t* List) {
    return suppress(List, List->size - 1);
}  


/* Parses the input line for relevant commands */ 
DLList_t* parseWords(char* line) {
    DLList_t* tmp = initList(); //command created with be used by then freed by parseCmd()
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

void* remov(Node_t* to_del) {
    void* ret = to_del->data;
    if(to_del->prev != NULL) to_del->prev->next = to_del->next;
    if(to_del->next != NULL) to_del->next->prev = to_del->prev;
    free(to_del);
    return ret;
}


void* removFromList(DLList_t* List, Node_t* to_del) {
    if(List->size == 0) {
        printf("empty list!\n");
        return NULL;
    }
    if(List->head == to_del) List->head = to_del->next;
    if(List->tail == to_del) List->tail = to_del->prev;
    return remov(to_del);
}
#endif //DLLIST_H
