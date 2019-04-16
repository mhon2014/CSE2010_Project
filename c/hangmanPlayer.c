/*

  Authors (group members):
  Email addresses of group members:
  Group name:

  Course:
  Section:

  Description of the overall algorithm:


*/

#include "hangmanPlayer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hangmanTrie.h"

// global declarations
Trie *trie;
static bool guessedLetters[ALPHABET_SIZE];

// initialize data structures from the word file
void init_hangman_player(char* word_file) {

  // variable declarations
  FILE *file_ptr = fopen(word_file, "r");  // file pointer for word file
  char *line = NULL;                       // stores each from input file
  int counter = 0;                         // tracks number of words read
  size_t len_of_line = 0;				           //length of word read


  // verify file opened properly
  if (file_ptr == NULL) {
    printf("Error file not found");
    exit(-1);
  }

  // initialize the trie
  trie = init_trie();

  // insert elements into trie
  while(getline(&line, &len_of_line, file_ptr) >= 0) {

    // printf("%s", line); // for testing
    insert(trie, line); // insert word into trie
    counter++; // increment number of words

  }  

  printf("Number of words: %d\n", trie->nodeCount); // for testing
  fclose(file_ptr);

  return;

} // end init_hangman_player

// based on the current (partially filled or intitially blank) word, guess a letter
// current_word: current word -- may contain blanks, terminated by a null char as usual
// is_new_word: indicates a new hidden word (current_word has blanks)
// returns the guessed letter
// Assume all letters in a word are in lower case
char guess_hangman_player(char* current_word, bool is_new_word) {

  // variable declarations
  char guess = ' ';
  

  // reset guesses if new word
  if (is_new_word) {
    for(int i = 0; i < ALPHABET_SIZE; i++){
      guessedLetters[i] = false;
    }
  }


  // formulate next guess

  // update guess_letters

  return guess;
}

// feedback on the guessed letter
// is_correct_guess: true if the guessed letter is one of the letters in the hidden word
// current_word: partially filled or blank word
//   
// Case       is_correct_guess    current_word   
// a.         true                partial word with the guessed letter
//                                   or the whole word if the guessed letter was the
//                                   last letter needed
// b.         false               partial word without the guessed letter
void feedback_hangman_player(bool is_correct_guess, char* current_word) {

  if (is_correct_guess) {

    // limit to paths with guessed letter in revealed position
  }

  else {

    // eliminate all paths with guessed letter in ANY position

  }

} // end feedback

