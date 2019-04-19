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
#include "utils/sllist.h"
// #include "utils/trie.h"
#include "utils/hangman.h"


#define MAX_LENGTH 128

/*******************************************************************************
 * global declarations
 ******************************************************************************/ 
// Trie *trie;
// Trie **tries;

SLList** words;
byte N_LIST;
char guess;
byte letter_freq[ALPHABET_SIZE]; // stores list of locations for each letter
static bool guessedLetters[ALPHABET_SIZE]; // = { 0 };
char prev_str[MAX_LENGTH] = "\0";


// byte max_len(char* word_file);

/******************************************************************************
 * METHODS
 *****************************************************************************/

byte max_len(char* word_file) {

  byte maxlen = 0;
  FILE *file_ptr = fopen(word_file, "r");  // file pointer for word file
  char line[MAX_LENGTH];      

  if (file_ptr == NULL) {
    printf("Error file not found");
    exit(-1);
  }

  while(fscanf(file_ptr, " %s", line) != EOF) {
    byte wordlen = strlen(line);
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
  words = (SLList**)malloc(N_LIST*sizeof(SLList*));

  for(byte i = 0; i < ALPHABET_SIZE; ++i) letter_freq[i] = 0;


  // variable declarations
  FILE *file_ptr = fopen(word_file, "r");  // file pointer for word file
  char line[MAX_LENGTH];                       // stores each from input file

  // verify file opened properly
  if (file_ptr == NULL) {
    printf("Error file not found");
    exit(-1);
  }

  // initialize the trie
  for(byte trie_i = 0; trie_i < N_LIST; ++trie_i) words[trie_i] = initList();

  // insert elements into trie
  while(fscanf(file_ptr, " %s", line) != EOF) {
    
    line[0] = tolower(line[0]);
    if(!strcmp(line, prev_str)) continue; // skip duplicates
    printf("inserting %s\n", line);
    Word* new_word = initWord(line);
    pushfront(words[strlen(line) - 1], new_word);
    strcpy(prev_str, line);
  }  

  // for(byte i = 0; i < N_TRIES; ++i)
  // printf("Number of words in trie %d : %d\n", i, tries[i]->nodeCount); // for testing
  fclose(file_ptr);

  // printf("size of DS %ld\n", sizeof(words));
  return;

} // end init_hangman_player










// based on the current (partially filled or intitially blank) word, guess a letter
// current_word: current word -- may contain blanks, terminated by a null char as usual
// is_new_word: indicates a new hidden word (current_word has blanks)
// returns the guessed letter
// Assume all letters in a word are in lower case
char guess_hangman_player(char* current_word, bool is_new_word) {

  // variable declarations
  byte curr_word_len = strlen(current_word);
  // reset guesses if new word
  if (is_new_word) {
    for(byte i = 0; i < ALPHABET_SIZE; i++) guessedLetters[i] = false;
    // resetPaths(tries[curr_word_len - 1]->root);
    printf("new word\n");
    /// call reset on the trie to be used
    reset(words, N_LIST);
  }

  // guessedLetters[CHAR_TO_INDEX('e')] = true;
  clock_t st,end;
  st = clock();
  guess = highestFreqLetter(words[curr_word_len - 1], letter_freq, guessedLetters);
  end = clock();

  printf("guessed %c at timing: %.4e\n", guess, ((double) (end - st)) / CLOCKS_PER_SEC);
  // for(byte i = 0; i < ALPHABET_SIZE; ++i) {
  //   printf("%c has %d freq \n", INDEX_TO_CHAR(i), letter_freq[i]);
  // }
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
  char letter = guess;
  byte curr_word_len = strlen(current_word);
  byte instances = checkInWord(current_word, letter);
  
  // limit to paths with guessed letter in revealed position
  if (is_correct_guess) {

      // find all positions in current word
      /// for each position that the letter occurs
          /// for each instance of letter in that position
                // go up to its parent
                // for each other child
                        // mark bad it and its ancestors

    elimWords(words[curr_word_len], true, letter, instances);

  } // end if "correct guess"

  else {
    elimWords(words[curr_word_len], false, letter, instances);
  }

} // end feedback

