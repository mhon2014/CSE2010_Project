#ifndef HANGMAN_H
#define HANGMAN_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "dllist.h"

#define MAX_LENGTH 64 // max string length for a word
#define ALPHABET_SIZE 26 // English alphabet size
#define CHAR_TO_INDEX(x) ((int)x - (int)'a')  // Hash function to convert letters to indices 
#define INDEX_TO_CHAR(x) (char)((int)x + (int)'a') // Hash function to convert indices to letters


typedef unsigned char byte_t; // 1 byte type
typedef struct
{
    byte_t freq; // number of occurences of the letter in the word
    uint positions; // encoded position of the letter in the word

} freq_t;

typedef struct 
{

    char* val; // word itself
    freq_t char_freq[ALPHABET_SIZE]; // occurences and position of every letter in word
    bool is_cand; // true if word is still candidate to guess

} Word_t;


/************************************************
 * PROTOTYPES 
 ***********************************************/ 

Word_t* initWord(char* val);
char highestFreqLetter(DLList_t  *words, uint *letter_freq, bool* guessLetters);
void encode(Word_t* to_encode);
byte_t charPresence(Word_t* w, char c);
void elimWords(DLList_t* wordList, bool flag, char bad_letter, byte_t inst, uint pos);
byte_t checkInWord(char* word, char letter);
void reset(DLList_t** words, byte_t size);
uint viableWords(DLList_t* wordlist); 
bool isMatchingPos(Word_t* w, char letter, uint pos);
uint getPositions(char* word, char letter);


/****************************************************************
 * IMPLEMENTATIONS
 * ****************************************************************/

/* initialize a word base on data received */
Word_t* initWord(char* word) {
    Word_t* new_node = (Word_t*)malloc(sizeof(Word_t));

    new_node->val = (char*)malloc((strlen(word) + 1)*sizeof(char));
    strcpy(new_node->val, word);
    new_node->is_cand = true;
    for(byte_t i = 0; i < ALPHABET_SIZE; ++i) {
        new_node->char_freq[i].freq = 0;
        new_node->char_freq[i].positions = 0x00000000;
    }
    encode(new_node);
    return new_node;
}


/* returns the encoded position of a letter in a word */
uint getPositions(char* word, char letter) {
    uint result = 0x00000000;
    for(byte_t char_i = 0; char_i < strlen(word); ++char_i) {
        if(word[char_i] == letter) {
            result |= (1 << (32 - char_i));
        }
    }
    return result;
}


/* determines if letter matches certain positions in a word */
bool isMatchingPos(Word_t* w, char letter, uint pos) {
    return (w->char_freq[CHAR_TO_INDEX(letter)].positions ^ pos) == 0x00000000;
}


/* return the number of words still candidates */
uint viableWords(DLList_t* wordlist) {
    uint counter = 0;
    for(Node_t* cursor = wordlist->head; cursor != NULL; cursor = cursor->next) {
        Word_t *w = (Word_t*)cursor->data;
        if(w->is_cand) ++counter;
    }
    return counter;
}

/* reset the list used for the previous guess */
void reset(DLList_t** words, byte_t size) {
    for(byte_t i = 0; i < size; ++i) {
        for(Node_t* cursor = words[i]->head; cursor != NULL; cursor = cursor->next) {
            Word_t *w = (Word_t*)cursor->data;
            w->is_cand = true;
        }
    }
}



/* check if a letter is present in word */
byte_t checkInWord(char* word, char letter){
    byte_t counter = 0;
    for(byte_t i = 0; i < strlen(word); ++i) {
        if(word[i] == letter) {
            ++counter;
        }
    }
    return counter;
}

/* encode frequency and position of every letter in word */
void encode(Word_t* word) {
    for(byte_t char_i = 0; char_i < strlen(word->val); ++char_i) {
        ++(word->char_freq[CHAR_TO_INDEX(word->val[char_i])].freq);
        word->char_freq[CHAR_TO_INDEX(word->val[char_i])].positions |= (1 << (32 - char_i));
    }
}

/* return the number of occurences of a letter in word */
byte_t charPresence(Word_t* w, char letter) {
    return w->char_freq[CHAR_TO_INDEX(letter)].freq;
}


/* 
update the frequency spectrum of every letter in the list of candidates
and return the most frequent 
*/
char highestFreqLetter(DLList_t  *wordlist, uint *letter_freq, bool* guessLetters) {

    byte_t max = CHAR_TO_INDEX('z');

    for(byte_t i = 0; i < ALPHABET_SIZE; ++i) letter_freq[i] = 0;

    for(Node_t* cursor = wordlist->head; cursor != NULL; cursor = cursor->next) {
        Word_t *w = (Word_t*)cursor->data;
        if(w->is_cand) {
            for(byte_t i = 0; i < ALPHABET_SIZE; ++i) {
                // char tmp = INDEX_TO_CHAR(i); // DEBUG
                if(charPresence(w, INDEX_TO_CHAR(i)) > 0) {
                    letter_freq[i] += charPresence(w, INDEX_TO_CHAR(i));
                    // ++letter_freq[i];
                }
            }
        } // else break;
    }

    for(byte_t i = 0; i < ALPHABET_SIZE; ++i) {
        if((letter_freq[i] >= letter_freq[max]) && !guessLetters[i]) {
            max = i;
        }
    }

    return INDEX_TO_CHAR(max);
}



/* remove the candature of words who does fit the updated guess string */
void elimWords(DLList_t* wordlist, bool is_good, char letter, byte_t inst, uint pos) {
    Node_t* next = NULL;
    for(Node_t* cursor = wordlist->head; cursor != NULL; cursor = next) {
        next = cursor->next;
        Word_t *w = (Word_t*)cursor->data;
        if(is_good) {
            // byte_t w_inst = charPresence(w, letter);
            if(charPresence(w, letter) != inst || !isMatchingPos(w, letter, pos)) {
                w->is_cand = false;
            }
            else { // is candidate
                pushfront(wordlist, w);
                remov(cursor);
            }
        }
        else { // is bad letter 
            byte_t w_inst = charPresence(w, letter);
            if(charPresence(w, letter) > 0) {
                w->is_cand = false;
            } 
            else { // is candidate 
                pushfront(wordlist, w);
                remov(cursor);
            }
        }
    }
}

#endif // HANGMAN_H