#ifndef HANGMAN_H
#define HANGMAN_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "dllist.h"

#define NONE 255                               // "null" index indicates a letter not in word, i.e. no associated struct
#define MAX_LENGTH 32                          // maximum string length for a word
#define ALPHABET_SIZE 26                       // English alphabet size
#define C2I(x) ((int)x - (int)'a')             // Hash function to map letters to indices 
#define I2C(x) (char)((int)x + (int)'a')       // Hash function to map indices to letters

// a type of size 1 byte
typedef unsigned char byte_t; 

// statistics of a particular letter in a word
typedef struct
{
    byte_t freq;  // number of occurences in word
    uint pos;     // binary encoding of positions of that letter

} letter_t;

// encodes a word as a struct of related statistics
typedef struct 

{

    bool is_cand;                          // true if word is still a candidate for the hidden word
    letter_t* distinct_letters;            // array of letter structs for each distinct letter in word 
    byte_t letter_indices[ALPHABET_SIZE];  // maps letters to index of a letter struct (NONE if no struct)

} word_t;


/************************************************
 * FUNCTION PROTOTYPES 
 ***********************************************/ 


word_t* initWord(char* val);
char highestFreqLetter(DLList_t  *words, ushort *letter_freq, bool* guessLetters);
void encode(word_t* to_encode, char* val);
byte_t charOccurrences(word_t* w, char c);
void elimWords(DLList_t* wordList, bool flag, char bad_letter, byte_t inst, uint pos);
byte_t checkInHiddenWord(char* word, char letter);
void reset(DLList_t** words, byte_t size);
uint viableWords(DLList_t* wordlist); 
bool isMatchingPos(word_t* w, char letter, uint pos);
uint getPositions(char* word, char letter);
void resetList(DLList_t* words);
byte_t numDistinctLetters(char* word, byte_t* letter_indices);

/****************************************************************
 * FUNCTION IMPLEMENTATIONS
 * ****************************************************************/

// determines number of distinct letter in words,
// assigns indices for corresponding letter structs
byte_t numDistinctLetters(char* word, byte_t* letter_indices) {

    byte_t counter = 0;      // counts the number of distinct letters
    byte_t freq[26] = { 0 };  // counts number of occurences of each letter
    
    // counts occurrences of letters in word

    for(byte_t c = 0; c < strlen(word); ++c) {
        ++freq[C2I(word[c])];
    }

    // for every letter 
    for(byte_t i = 0; i < ALPHABET_SIZE; ++i) {

        // if it occurs at least once, allocate new index
        if(freq[i] > 0) {
            letter_indices[i] = counter;
            ++counter;
        }

        // set associated index to "none" if not in word 
        else 
            letter_indices[i] = NONE;
    }

    return counter;

} 

// initialize a word based on data received 
word_t* initWord(char* word) {

    word_t* new_node = (word_t*) malloc(sizeof(word_t));                           // allocate memory for new word
    int num_letters = numDistinctLetters(word, new_node->letter_indices);          // number of distinct letters in word
    new_node->distinct_letters = (letter_t*) malloc(num_letters*sizeof(letter_t)); // allocate memory for letter structs
  
    new_node->is_cand = true; // intialize word as candidate for hidden word

    // for each letter in the word
    for(byte_t i = 0; i < ALPHABET_SIZE; ++i) {
        if(new_node->letter_indices[i] != NONE) {

            // set frequency and positions all to zero
            new_node->distinct_letters[new_node->letter_indices[i]].freq = 0;
            new_node->distinct_letters[new_node->letter_indices[i]].pos = 0x00000000;

        }
    }

    encode(new_node, word); // encode information for letters in word

    return new_node;

} 

// encode frequency and position of each distinct letter in word 
void encode(word_t* word, char* val) {

    // for each character in word
    for (byte_t char_i = 0; char_i < strlen(val); ++char_i) {

        byte_t j = C2I(val[char_i]); // get character
        
        // if letter has a corresponding struct (i.e. exists in word)
        if (word->letter_indices[j] != NONE) {
            (word->distinct_letters[word->letter_indices[j]].freq)++;                      // increment its frequency
            word->distinct_letters[word->letter_indices[j]].pos |= (1 << (32 - char_i));   // log its position
        }

    } // end for loop

    return;
}


// returns the encoded position of a given letter in a word
uint getPositions(char* word, char letter) {

    uint result = 0x00000000; // initialize positions to all zero

    // for each matching letter in word 
    for(byte_t char_i = 0; char_i < strlen(word); ++char_i) {
        if(word[char_i] == letter) {
            // adds position to result
            result |= (1 << (32 - char_i));
        }
    }

    return result;

}


// determines if letter matches certain position in a word 
bool isMatchingPos(word_t* w, char letter, uint pos) {

    return ((w->distinct_letters[w->letter_indices[C2I(letter)]].pos ^ pos) == 0x00000000);

}


// return the number of words still marked candidates
uint viableWords(DLList_t* wordlist) {

    uint counter = 0; // initialize number of words counter to zero

    // traverse list of words
    for (Node_t* current = wordlist->head; current != NULL; current = current->next) {

        // get data for current word
        word_t *temp = (word_t*) current->data;

        // increment if word is a candidate
        if (temp->is_cand) 
            counter++;
    
    }

    return counter;
}

// resets information in all lists to prepare for new word
void reset(DLList_t** words, byte_t size) {

    // for each word in each list
    for(byte_t i = 0; i < size; ++i) {
        for(Node_t* current = words[i]->head; current != NULL; current = current->next) {

            // mark as candidate
            word_t *w = (word_t*)current->data;
            w->is_cand = true;
     
        }
    }

    return;

}

// reset the list used for the previous guess
void resetList(DLList_t* words) {

    // for each node in list
    for(Node_t* current = words->tail; current != NULL; current = current->prev) {

        word_t *temp = (word_t*)current->data;  // get data

        // stop resetting once candidate words are reached
        if(temp->is_cand) 
            break;

        // mark non-candidates as candidates again
        else 
            temp->is_cand = true;

    }

    return;

}


// check for given letter in revealed part of hidden word
byte_t checkInHiddenWord(char* word, char letter) {

    byte_t num_occurrences = 0; // initialize counter for number of instances

    // for each occurrence of letter in word
    for(byte_t i = 0; i < strlen(word); ++i) {
        if(word[i] == letter) {

            // increment counter 
            num_occurrences++; 

        }
    }

    return num_occurrences;

}

// return the number of occurences of a letter in dictionary word
byte_t charOccurrences(word_t* w, char letter) {

    // get index of target letter
    byte_t i = C2I(letter);

    // return frequency if letter is present
    if(w->letter_indices[i] != NONE) 
        return w->distinct_letters[w->letter_indices[i]].freq;

    // return 0 if letter not present
    else 
        return 0;

}


// update the frequency info for letters in list of candidates, return the most frequent 
char highestFreqLetter(DLList_t  *wordlist, ushort *letter_freq, bool* guessLetters) {

    byte_t max = C2I('z'); // highest possible index

    // initialize frequencies to zero
    for (byte_t i = 0; i < ALPHABET_SIZE; ++i) 
        letter_freq[i] = 0;

    // for every word in list
    for (Node_t* current = wordlist->head; current != NULL; current = current->next) {

        word_t *temp = (word_t*) current->data; // get data of word

        // if word is candidate
        if (temp->is_cand) {
            // for each letter
            for (byte_t i = 0; i < ALPHABET_SIZE; ++i) {

                // check for presence
                byte_t p = charOccurrences(temp, I2C(i));
                // increment frequency if present
                if( p > 0) 
                    letter_freq[i] += p;

            } // end loop over letters

        // if word is not candidate
        } else 
            break;

    } // end loop over words

    // update max when more frequent letter is found
    for (byte_t i = 0; i < ALPHABET_SIZE; ++i) {
        if ((letter_freq[i] >= letter_freq[max]) && !guessLetters[i]) {
            max = i;
        }
    }

    char max_char = I2C(max); // convert max from index back to char 

    return max_char; 

}



// eliminate words that no longer fit updated hidden word info
void elimWords(DLList_t* wordlist, bool is_good, char letter, byte_t inst, uint pos) {

    Node_t* next = NULL;

    // for each node in word list
    for(Node_t* current = wordlist->head; current != NULL; current = next) {

        next = current->next; // saves next field of current node

        word_t *temp = (word_t*)current->data; // get data of current word

        // stop searching when bad candidates are reached
        if(!(temp->is_cand)) 
            break;

        // if is good letter
        if(is_good) {

            // if word doesn't have correct frequency of letter, or not in matching positions
            if(charOccurrences(temp, letter) != inst || !isMatchingPos(temp, letter, pos)) {
                // mark as not a candidate
                temp->is_cand = false; 
            }

            // move words that are still candidates to front of list
            else if(temp->is_cand) {
                    pushfront(wordlist, temp); 
                    removFromList(wordlist, current);
                }

        } 

        // if is bad letter
        else { 

            // get occurrences of target letter
            byte_t temp_inst = charOccurrences(temp, letter); 

            // mark bad if "bad letter" occurs in word
            if (charOccurrences(temp, letter) > 0) {
                temp->is_cand = false;
            } 

            // push candidate to front
            else if (temp->is_cand) {
                    pushfront(wordlist, temp);
                    removFromList(wordlist, current);
            }
        }
    }
}

#endif // HANGMAN_H
