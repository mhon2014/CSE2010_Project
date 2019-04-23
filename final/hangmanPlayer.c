/*

  Authors (group members): 
  Sarah Arends, Alejandra Escobar, Michael Hon, Josias Moukpe 
  Email addresses of group members:
  sarends2015@my.fit.edu, aescobarsant2017@my.fit.edu, mhon2014@my.fit.edu, jmoukpe2016@my.fit.edu
  Group name: Fantastic For(int i = 0; i < 4; ++i)
  Course: cse2010
  Section: 14

  Description of the overall algorithm:
  Words are seperated into lists by size and encoded as a struct, 
  with frequency and position information for each letter. Candidate 
  words are kept at the front of each list. If they fail to match the
  position encoding of a correct guess, or include an incorrect guess,
  they are marked "not candidate" and moved to the end of the list.

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include "hangmanPlayer.h"
#include "dllist.h"
#include "hangman.h"



/*******************************************************************************
 * GLOBAL VARIABLE DECLARATIONS
 ******************************************************************************/ 

DLList_t** words;
byte_t num_lists;                     // number of lists, each of different length words
char guess;                           // stores letter to be guessed
ushort letter_freq[ALPHABET_SIZE];    // stores list of locations for each letter
bool guessedLetters[ALPHABET_SIZE];   // = { 0 };
char prev_str[MAX_LENGTH] = "\0";     // keys track of the previous string to remove duplicates
byte_t previous_len = 1;              // stores previous length for resetting purposes

/****************************************************************************** 
 * METHODS
 *****************************************************************************/

// return the length of the longest word in the words file
byte_t max_len(char* word_file) {

  byte_t maxlen = 0;                       // maximum length of a word
  FILE *file_ptr = fopen(word_file, "r");  // file pointer for word file
  char line[MAX_LENGTH];                   // stores line from file

  // verify file opened properly
  if (file_ptr == NULL) {
    printf("Error file not found");
    exit(-1);
  }

  // loop over words and update maxlen
  while(fscanf(file_ptr, " %s", line) != EOF) {
    byte_t wordlen = strlen(line);
    if(wordlen > maxlen) maxlen = wordlen;
  }  

  fclose(file_ptr); // close file

  return maxlen;

}

// initialize data structures from the word file
void init_hangman_player(char* word_file) {

  num_lists = max_len(word_file);                          // number of lists, each containing words of same length
  words = (DLList_t**)malloc(num_lists*sizeof(DLList_t*)); // alloocate space for lists

  // initialize list of letter frequencies
  for(byte_t i = 0; i < ALPHABET_SIZE; ++i) 
  	letter_freq[i] = 0;

  // variable declarations
  FILE *file_ptr = fopen(word_file, "r");        // file pointer for word file
  char line[MAX_LENGTH];                         // stores each from input file

  // verify file opened properly
  if (file_ptr == NULL) {

    printf("Error file not found");
    exit(-1);

  } // end if

  // initialize the lists
  for(byte_t i = 0; i < num_lists; ++i) words[i] = initList();

  // insert elements into lists
  while(fscanf(file_ptr, " %s", line) != EOF) {

    line[0] = tolower(line[0]);

    // skip duplicates
    if (!strcmp(line, prev_str)) 
    	continue; 

    word_t* new_word = initWord(line);            // initialize word structure
    pushback(words[strlen(line) - 1], new_word);  // insert at end of list
    strcpy(prev_str, line);

  } 

  fclose(file_ptr); // close file

  return;

} // end init_hangman_player

// guess the most likely letter in the word given the current selection of words
char guess_hangman_player(char* current_word, bool is_new_word) {

  byte_t curr_word_len = strlen(current_word); // get length of current word
  
  if (is_new_word) {

 	// initialize stats for new word
    for(byte_t i = 0; i < ALPHABET_SIZE; i++) {
      guessedLetters[i] = false;
      letter_freq[i] = 0;
   
    }
  
  	// initialize guess and list
    guess = ' ';
    resetList(words[previous_len - 1]);
  
  } // end if

  // determine next guess, mark as guessed
  previous_len = curr_word_len;  
  guess = highestFreqLetter(words[curr_word_len - 1], letter_freq, guessedLetters);
  guessedLetters[C2I(guess)] = true;

  return guess;
}

// update the selection of words based on the guess
void feedback_hangman_player(bool is_correct_guess, char* current_word) {

  // variable declarations
  char letter = guess;                                        // letter that was guessed
  byte_t curr_word_len = strlen(current_word);                // get length of current word
  byte_t num_instances = 0;                                   // instances of letter in word, initially zero
  uint pos = getPositions(current_word, letter);              //  get encoded positions of letter in word
  
  // eliminate words that don't match the revealed letters
  if (is_correct_guess) {
    num_instances = checkInHiddenWord(current_word, letter); // update num_instances if in word
    elimWords(words[curr_word_len - 1], true, letter, num_instances, pos);
  } 

  // eliminate words that contain the incorrect letter
  else {
    elimWords(words[curr_word_len - 1], false, letter, num_instances, pos);
  }

  return;

} 

