#ifndef HANGMAN_H
#define HANGMAN_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "dllist.h"

#define NONE 255
#define MAX_LENGTH 32 // max string length for a word
#define ALPHABET_SIZE 26 // English alphabet size
#define C2I(x) ((int)x - (int)'a')  // Hash function to convert letters to indices 
#define I2C(x) (char)((int)x + (int)'a') // Hash function to convert indices to letters


typedef unsigned char byte_t; // 1 byte type
typedef struct
{
    byte_t freq; // number of occurences of the letter in the word
    uint pos; // encoded position of the letter in the word

} freq_t;

typedef struct 
{

    // char* val; // word itself
    byte_t freqs_indices[ALPHABET_SIZE]; // occurences and position of every letter in word
    freq_t* letter_freqs;
    bool is_cand; // true if word is still candidate to guess

} Word_t;


/************************************************
 * PROTOTYPES 
 ***********************************************/ 

Word_t* initWord(char* val);
char highestFreqLetter(DLList_t  *words, ushort *letter_freq, bool* guessLetters);
void encode(Word_t* to_encode, char* val);
byte_t charPresence(Word_t* w, char c);
void elimWords(DLList_t* wordList, bool flag, char bad_letter, byte_t inst, uint pos);
byte_t checkInWord(char* word, char letter);
void reset(DLList_t** words, byte_t size);
uint viableWords(DLList_t* wordlist); 
bool isMatchingPos(Word_t* w, char letter, uint pos);
uint getPositions(char* word, char letter);
void resetList(DLList_t* words);
byte_t numDistinctLetters(char* word, byte_t* freq_indices);

/****************************************************************
 * IMPLEMENTATIONS
 * ****************************************************************/

byte_t numDistinctLetters(char* word, byte_t* freq_indices) {
    byte_t counter = 0;
    byte_t tmp[26] = { 0 };
    // byte_t pos = 0;
    
    for(byte_t c = 0; c < strlen(word); ++c) {
        ++tmp[C2I(word[c])];
    }

    for(byte_t i = 0; i < ALPHABET_SIZE; ++i) {
        if(tmp[i] > 0) {
            freq_indices[i] = counter;
            ++counter;
        }
        else 
            freq_indices[i] = NONE;
    }
    return counter;
}


/* initialize a word base on data received */
Word_t* initWord(char* word) {
    Word_t* new_node = (Word_t*)malloc(sizeof(Word_t));
    
    new_node->is_cand = true;
    new_node->letter_freqs = (freq_t*)malloc(numDistinctLetters(word, new_node->freqs_indices)*sizeof(freq_t));

    for(byte_t i = 0; i < ALPHABET_SIZE; ++i) {
        if(new_node->freqs_indices[i] != NONE) {
            // new_node->char_freq[i].freq = 0;
             // new_node->char_freq[i].pos = 0x00000000;
            new_node->letter_freqs[new_node->freqs_indices[i]].freq = 0;
            new_node->letter_freqs[new_node->freqs_indices[i]].pos = 0x00000000;
        }
    }

    encode(new_node, word);
    return new_node;
}

/* encode frequency and position of every letter in word */
void encode(Word_t* word, char* val) {
    for(byte_t char_i = 0; char_i < strlen(val); ++char_i) {
        // ++(word->char_freq[C2I(val[char_i])].freq);
        // word->char_freq[C2I(val[char_i])].pos |= (1 << (32 - char_i));
        byte_t j = C2I(val[char_i]);
        if(word->freqs_indices[j] != NONE) {
            ++(word->letter_freqs[word->freqs_indices[j]].freq);
            word->letter_freqs[word->freqs_indices[j]].pos |= (1 << (32 - char_i));
        }
    }
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


/* determines if letter matches certain pos in a word */
bool isMatchingPos(Word_t* w, char letter, uint pos) {
    return ((w->letter_freqs[w->freqs_indices[C2I(letter)]].pos ^ pos) == 0x00000000);
    // return (w->char_freq[C2I(letter)].pos ^ pos) == 0x00000000;
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

/* reset all lists */
void reset(DLList_t** words, byte_t size) {
    for(byte_t i = 0; i < size; ++i) {
        for(Node_t* cursor = words[i]->head; cursor != NULL; cursor = cursor->next) {
            Word_t *w = (Word_t*)cursor->data;
            w->is_cand = true;
        }
    }
}

/* reset the list used for the previous guess */
void resetList(DLList_t* words) {
    for(Node_t* cursor = words->tail; cursor != NULL; cursor = cursor->prev) {
        Word_t *w = (Word_t*)cursor->data;
        if(w->is_cand) break;
        else w->is_cand = true;
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

/* return the number of occurences of a letter in word */
byte_t charPresence(Word_t* w, char letter) {
    byte_t i = C2I(letter);
    if(w->freqs_indices[i] != NONE) 
        return w->letter_freqs[w->freqs_indices[i]].freq;
    else 
        return 0;
    // return w->char_freq[C2I(letter)].freq;
}

/* 
update the frequency spectrum of every letter in the list of candidates
and return the most frequent 
*/
char highestFreqLetter(DLList_t  *wordlist, ushort *letter_freq, bool* guessLetters) {

    byte_t max = C2I('z');

    for(byte_t i = 0; i < ALPHABET_SIZE; ++i) letter_freq[i] = 0;

    for(Node_t* cursor = wordlist->head; cursor != NULL; cursor = cursor->next) {
        Word_t *w = (Word_t*)cursor->data;
        // printf("candidature : %s\n", w->is_cand?"cand":"not cand");
        if(w->is_cand) {
            for(byte_t i = 0; i < ALPHABET_SIZE; ++i) {
                // char tmp = I2C(i); // DEBUG
                byte_t p = charPresence(w, I2C(i));
                if( p > 0) {
                    letter_freq[i] += p;
                    // ++letter_freq[i];
                }
            }
        } else break;
    }

    for(byte_t i = 0; i < ALPHABET_SIZE; ++i) {
        if((letter_freq[i] >= letter_freq[max]) && !guessLetters[i]) {
            max = i;
        }
    }

    return I2C(max);
}



/* remove the candature of words who does fit the updated guess string */
void elimWords(DLList_t* wordlist, bool is_good, char letter, byte_t inst, uint pos) {
    Node_t* next = NULL;
    for(Node_t* cursor = wordlist->head; cursor != NULL; cursor = next) {
        next = cursor->next;
        Word_t *w = (Word_t*)cursor->data;
        if(!w->is_cand) break;
        if(is_good) {
            // byte_t w_inst = charPresence(w, letter);
            if(charPresence(w, letter) != inst || !isMatchingPos(w, letter, pos)) {
                w->is_cand = false;
            }
            else { // is candidate
                // w->is_cand = true;
                if(w->is_cand) {
                    pushfront(wordlist, w);
                    removFromList(wordlist, cursor);
                }
            }
        }
        else { // is bad letter 
            byte_t w_inst = charPresence(w, letter);
            if(charPresence(w, letter) > 0) {
                w->is_cand = false;
            } 
            else { // is candidate 
                // w->is_cand = true;
                if(w->is_cand) {
                    pushfront(wordlist, w);
                    removFromList(wordlist, cursor);
                }
            }
        }
    }
}

#endif // HANGMAN_H