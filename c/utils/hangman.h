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


typedef struct word {

    char* val;
    byte char_freq[ALPHABET_SIZE];
    bool is_cand;

} Word;


Word* initWord(char* val);
char highestFreqLetter(SLList  *words, byte *letter_freq, bool* guessLetters);
void encode(Word* to_encode);
byte charPresence(Word* w, char c);
void elimWords(SLList* wordList, bool flag, char bad_letter, byte inst);
byte checkInWord(char* word, char letter);
void reset(SLList** words, byte size);

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
        ++to_encode->char_freq[CHAR_TO_INDEX(to_encode->val[char_i])];
    }
}

byte charPresence(Word* w, char c) {
    return w->char_freq[CHAR_TO_INDEX(c)];
}

char highestFreqLetter(SLList  *words, byte *letter_freq, bool* guessLetters) {
    // printf("Guessing... \n");
    // ANode* start = trie->root;
    byte max = 0;

    // for(byte i = 0; i < ALPHABET_SIZE; ++i) {
    //     preOrder(start->children[i], letter_freq, word_len);
    // }
    for(Node* cursor = words->head; cursor != NULL; cursor = cursor->next) {
        Word *w = (Word*)cursor->data;
        if(w->is_cand) {
            for(byte i = 0; i < ALPHABET_SIZE; ++i) {
                if(charPresence(w, INDEX_TO_CHAR(i)) > 0) {
                    ++letter_freq[i];
                }
            }
        }
    }

    // return max freq letter
    for(byte i = 0; i < ALPHABET_SIZE; ++i) {
        if(letter_freq[i] > letter_freq[max] && !guessLetters[i]) {
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
    for(byte i = 0; i < ALPHABET_SIZE; ++i) new_node->char_freq[i] = 0;
    encode(new_node);
    return new_node;
}

void elimWords(SLList* words, bool is_good, char letter, byte inst) {
    for(Node* cursor = words->head; cursor != NULL; cursor = cursor->next) {
        Word *w = (Word*)cursor->data;
        if(is_good)
            if(charPresence(w, letter) != inst) w->is_cand = false;
        else // is bad letter
            if(charPresence(w, letter) > 0) w->is_cand = false;
    }
}

#endif // HANGMAN_H