/*

  Authors (group members): 
  Josias Moukpe 
  Email addresses of group members:
  jmoukpe2016@my.fit.edu
  Group name: Fantastic For(int i = 0; i < 4; ++i)
  Course: cse2010
  Section: 14b

  Description of the overall algorithm:
  builds multiple tries for base on the length of the words
  all tries are stored in BST with key the word length
  Guess base on the  most frequent letter in the appropriate trie
  correct guesses would prune the trie of all the path not containing the guessed letters
  incorrect guesses would prune the trie of the path containing the guessed letter

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include "hangmanPlayer.h"
#include "utils/dllist.h"
#include "utils/hangman.h"



/*******************************************************************************
 * global declarations
 ******************************************************************************/ 

DLList_t** words;
byte_t N_LIST;
char guess;
ushort letter_freq[ALPHABET_SIZE]; // stores list of locations for each letter
bool guessedLetters[ALPHABET_SIZE]; // = { 0 };
char prev_str[MAX_LENGTH] = "\0"; // keys track of the previous string to remove duplicates
uint counter = 1;
byte_t previous_len = 1;
// bool done = true;

/******************************************************************************
 * METHODS
 *****************************************************************************/

/* return the lentgh of the longest word in the words file */
byte_t max_len(char* word_file) {

  byte_t maxlen = 0;
  FILE *file_ptr = fopen(word_file, "r");  // file pointer for word file
  char line[MAX_LENGTH];      

  if (file_ptr == NULL) {
    printf("Error file not found");
    exit(-1);
  }

  while(fscanf(file_ptr, " %s", line) != EOF) {
    byte_t wordlen = strlen(line);
    // printf("strlen = %d\n", wordlen);
    if(wordlen > maxlen) maxlen = wordlen;
  }  

  // printf("Number of words: %d\n", trie->nodeCount); // for testing
  fclose(file_ptr);
  return maxlen;

}

// initialize data structures from the word file
void init_hangman_player(char* word_file) {
  N_LIST = max_len(word_file);
  words = (DLList_t**)malloc(N_LIST*sizeof(DLList_t*));

  for(byte_t i = 0; i < ALPHABET_SIZE; ++i) letter_freq[i] = 0;


  // variable declarations
  FILE *file_ptr = fopen(word_file, "r");  // file pointer for word file
  FILE *output = fopen("test/output.txt", "w"); // DEBUG
  char line[MAX_LENGTH];                       // stores each from input file

  // verify file opened properly
  if (file_ptr == NULL) {
    printf("Error file not found");
    exit(-1);
  }

  // initialize the trie
  for(byte_t trie_i = 0; trie_i < N_LIST; ++trie_i) words[trie_i] = initList();

  // insert elements into trie
  while(fscanf(file_ptr, " %s", line) != EOF) {
    line[0] = tolower(line[0]);
    if(!strcmp(line, prev_str)) continue; // skip duplicates
    Word_t* new_word = initWord(line);
    pushfront(words[strlen(line) - 1], new_word);
    strcpy(prev_str, line);
  }  

  reset(words, N_LIST);
  fclose(output); // DEBUG
  fclose(file_ptr);
  return;

} // end init_hangman_player



/* guess the most likely letter in the word given the current selection of words */
char guess_hangman_player(char* current_word, bool is_new_word) {
  byte_t curr_word_len = strlen(current_word);

  if (is_new_word) {
    // if(counter == 5) scanf(" ");
    for(byte_t i = 0; i < ALPHABET_SIZE; i++) {
      guessedLetters[i] = false;
      letter_freq[i] = 0;
    }
  
    printf("new word #%d\n", counter); // DEBUG
    guess = ' ';
    resetList(words[previous_len - 1]);
    counter++; // DEBUG
  }

  previous_len = curr_word_len;

  guess = highestFreqLetter(words[curr_word_len - 1], letter_freq, guessedLetters);
  guessedLetters[CHAR_TO_INDEX(guess)] = true;
  printf("guessed %c \n", guess);

  // uint words_left = viableWords(words[curr_word_len - 1]);
  // printf("only %d words remain\n", viableWords(words[curr_word_len - 1]));

  return guess;
}

/* update the selection of word based on the guess */
void feedback_hangman_player(bool is_correct_guess, char* current_word) {
  char letter = guess;
  byte_t curr_word_len = strlen(current_word);
  byte_t instances = checkInWord(current_word, letter);
  uint pos = getPositions(current_word, letter);
  
  if (is_correct_guess) {
    elimWords(words[curr_word_len - 1], true, letter, instances, pos);
  } // end if "correct guess"
  else {
    elimWords(words[curr_word_len - 1], false, letter, instances, pos);
  }
} 

