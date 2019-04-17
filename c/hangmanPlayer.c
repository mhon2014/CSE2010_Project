/*

  Authors (group members):
  Email addresses of group members:
  Group name:

  Course:
  Section:

  Description of the overall algorithm:


*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hangmanPlayer.h"
#include "hangmanTrie.h"
#include <ctype.h>
#include <unistd.h>
#include "utils/sllist.h"


#define MAX_LENGTH 32

// global declarations
Trie *trie;
static bool guessedLetters[ALPHABET_SIZE]; // = { 0 };
char guess = 'e';
SLList* letter_ptrs[ALPHABET_SIZE]; // stores list of locations for each letter

// initialize data structures from the word file
void init_hangman_player(char* word_file) {

  for(byte i = 0; i < ALPHABET_SIZE; ++i) {
    letter_ptrs[i] = initList();
  }

  // variable declarations
  FILE *file_ptr = fopen(word_file, "r");  // file pointer for word file
  char line[MAX_LENGTH];                       // stores each from input file
  // int counter = 0;                         // tracks number of words read
  // size_t len_of_line = 0;				           //length of word read

  // verify file opened properly
  if (file_ptr == NULL) {
    printf("Error file not found");
    exit(-1);
  }

  // initialize the trie
  trie = init_trie();

  // insert elements into trie
  while(fscanf(file_ptr, " %s", line) != EOF) {
    line[0] = tolower(line[0]);
    // printf("%s\n", line); // for testing
    // sleep(1);
    insertTrie(trie, line); // insert word into trie
    // counter++; // increment number of words

  }  

  // printf("Number of words: %d\n", trie->nodeCount); // for testing
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
  
  // reset guesses if new word
  if (is_new_word) {
    for(int i = 0; i < ALPHABET_SIZE; i++) guessedLetters[i] = false;
  }


  guess = ((AlphaNode*)highestFreqLetter(trie, letter_ptrs, guessedLetters)->head->data)->letter;
  printf("guessed %c\n", guess);
  // scanf(" ");
  guessedLetters[CHAR_TO_INDEX(guess)] = true;

  /// go through all possible paths

    /// if you find a !(is_candidate), that path is bad so stop searching it 
    /// if you get to the end of path and it's all good
            /// go all the way up that path and increment a 
            /// "words with that letter in them" counter for every letter you find

  /// quess = most frequent letter

  // update guess_letters array with the letter we're going to guess

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

  // limit to paths with guessed letter in revealed position
  if (is_correct_guess) {

      // find all positions in current word
      /// for each position that the letter occurs
          /// for each instance of letter in that position
                // go up to its parent
                // for each other child
                        // mark bad it and its ancestors

  } // end if "correct guess"

  else {
    char bad_letter = guess;
    eliminate_paths(trie, letter_ptrs, bad_letter); // prune tree after bad guess
  }

} // end feedback

