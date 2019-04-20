#ifndef HANGMAN_H
#define HANGMAN_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "sllist.h"

#define ALPHABET_SIZE 26

#define CHAR_TO_INDEX(x) ((int)x - (int)'a') 
#define INDEX_TO_CHAR(x) (char)((int)x + (int)'a')


typedef unsigned char byte;

typedef struct
{
    byte freq;
    uint positions;
} freq_t;



typedef struct word {

    char* val;
    // freq_t char_freq[ALPHABET_SIZE];
    freq_t char_freq[ALPHABET_SIZE];
    bool is_cand;

} Word;



// char INDEX_TO_CHAR(byte x);
Word* initWord(char* val);
char highestFreqLetter(SLList  *words, uint *letter_freq, bool* guessLetters);
void encode(Word* to_encode);
byte charPresence(Word* w, char c);
void elimWords(SLList* wordList, bool flag, char bad_letter, byte inst, uint pos);
byte checkInWord(char* word, char letter);
void reset(SLList** words, byte size);
uint viableWords(SLList* wordlist); 
bool isMatchingPos(Word* w, char letter, uint pos);
uint getPositions(char* word, char letter);


// char INDEX_TO_CHAR(byte x) {
//     return (char)(x + 'a');
// }


uint getPositions(char* word, char letter) {
    uint result = 0x00000000;
    for(byte char_i = 0; char_i < strlen(word); ++char_i) {
        if(word[char_i] == letter) {
            result |= (1 << (32 - char_i));
        }
    }
    return result;
}


bool isMatchingPos(Word* w, char letter, uint pos) {
    // bool result = false;
    return (w->char_freq[CHAR_TO_INDEX(letter)].positions ^ pos) == 0x00000000;
}

uint viableWords(SLList* wordlist) {
    uint counter = 0;
    for(Node* cursor = wordlist->head; cursor != NULL; cursor = cursor->next) {
        Word *w = (Word*)cursor->data;
        if(w->is_cand) ++counter;
    }
    return counter;
}

void reset(SLList** words, byte size) {
    for(byte i = 0; i < size; ++i) {
        for(Node* cursor = words[i]->head; cursor != NULL; cursor = cursor->next) {
            Word *w = (Word*)cursor->data;
            w->is_cand = true;
        }
    }
}




byte checkInWord(char* word, char letter){
    byte counter = 0;
    for(byte i = 0; i < strlen(word); ++i) {
        if(word[i] == letter) {
            ++counter;
        }
    }
    return counter;
}


void encode(Word* to_encode) {
    for(byte char_i = 0; char_i < strlen(to_encode->val); ++char_i) {
        ++(to_encode->char_freq[CHAR_TO_INDEX(to_encode->val[char_i])].freq);
        to_encode->char_freq[CHAR_TO_INDEX(to_encode->val[char_i])].positions |= (1 << (32 - char_i));
    }
}

byte charPresence(Word* w, char c) {
    return w->char_freq[CHAR_TO_INDEX(c)].freq;
}

char highestFreqLetter(SLList  *wordlist, uint *letter_freq, bool* guessLetters) {
    // printf("Guessing... \n");
    // ANode* start = trie->root;
    byte max = 25;

    // for(byte i = 0; i < ALPHABET_SIZE; ++i) {
    //     preOrder(start->children[i], letter_freq, word_len);
    // }

    for(byte i = 0; i < ALPHABET_SIZE; ++i) {
        letter_freq[i] = 0;
    }

    for(Node* cursor = wordlist->head; cursor != NULL; cursor = cursor->next) {
        Word *w = (Word*)cursor->data;
        if(w->is_cand) {
            for(byte i = 0; i < ALPHABET_SIZE; ++i) {
                char tmp = INDEX_TO_CHAR(i); // DEBUG
                if(charPresence(w, INDEX_TO_CHAR(i)) > 0) {
                    ++letter_freq[i];
                }
            }
        }
    }

    // byte arr[26];
    // for(byte i = 0; i < ALPHABET_SIZE; ++i)
    //     arr[i] = letter_freq[i]; // DEBUG
    // return max freq letter
    for(byte i = 0; i < ALPHABET_SIZE; ++i) {
        if((letter_freq[i] >= letter_freq[max]) && !guessLetters[i]) {
            max = i;
        }
    }

    return INDEX_TO_CHAR(max);
}


Word* initWord(char* val) {
    Word* new_node = (Word*)malloc(sizeof(Word));

    new_node->val = (char*)malloc((strlen(val) + 1)*sizeof(char));
    strcpy(new_node->val, val);
    new_node->is_cand = true;
    for(byte i = 0; i < ALPHABET_SIZE; ++i) {
        new_node->char_freq[i].freq = 0;
        new_node->char_freq[i].positions = 0x00000000;
    }
    encode(new_node);
    return new_node;
}

void elimWords(SLList* wordlist, bool is_good, char letter, byte inst, uint pos) {
    for(Node* cursor = wordlist->head; cursor != NULL; cursor = cursor->next) {
        Word *w = (Word*)cursor->data;
        if(is_good) {
            byte w_inst = charPresence(w, letter);
            if(charPresence(w, letter) != inst || !isMatchingPos(w, letter, pos)) {
                w->is_cand = false;
                // printf(" ");
            }
        }
        else { // is bad letter 
            byte w_inst = charPresence(w, letter);
            if(charPresence(w, letter) > 0) {
                w->is_cand = false;
                // printf(" ");
            }
        }
    }
}

#endif // HANGMAN_H