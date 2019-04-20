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
uint letter_freq[ALPHABET_SIZE]; // stores list of locations for each letter
bool guessedLetters[ALPHABET_SIZE]; // = { 0 };
char prev_str[MAX_LENGTH] = "\0";
uint counter = 1;


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
  FILE *output = fopen("test/output.txt", "w");
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
    Word* new_word = initWord(line);
    pushfront(words[strlen(line) - 1], new_word);
    strcpy(prev_str, line);

  }  

  // byte i = CHAR_TO_INDEX('z');
  // char c = INDEX_TO_CHAR(0);

  // char z = 'z';
  // for(byte i = 0; i < N_LIST; ++i)
  //   printf("Number of words in trie %d : %d\n", i, words[i]->size); // for testing


  // for(Node* cursor = words[13]->head; cursor != NULL; cursor = cursor->next) {
  //   Word *w = (Word*)cursor->data;
  //   fprintf(output, "%s\n", w->val); 
  //   printf("%s\n", w->val);
  // }

  fclose(output);
  fclose(file_ptr);

  // printf("size of DS %ld\n", sizeof(words));
  return;

} // end init_hangman_player


char guess_hangman_player(char* current_word, bool is_new_word) {

  // variable declarations
  byte curr_word_len = strlen(current_word);
  // reset guesses if new word
  if (is_new_word) {
    // if(counter == 1) scanf(" ");
    for(byte i = 0; i < ALPHABET_SIZE; i++) {
      guessedLetters[i] = false;
      letter_freq[i] = 0;
    }
  
    printf("new word #%d\n", counter);
    guess = ' ';
    reset(words, N_LIST);
    counter++;
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

  guessedLetters[CHAR_TO_INDEX(guess)] = true;
  printf("only %d words remain\n", viableWords(words[curr_word_len - 1]));

  return guess;
}


void feedback_hangman_player(bool is_correct_guess, char* current_word) {
  char letter = guess;
  byte curr_word_len = strlen(current_word);
  byte instances = checkInWord(current_word, letter);
  uint pos = getPositions(current_word, letter);
  
  // limit to paths with guessed letter in revealed position
  if (is_correct_guess) {

    elimWords(words[curr_word_len - 1], true, letter, instances, pos);

  } // end if "correct guess"

  else {
    elimWords(words[curr_word_len - 1], false, letter, instances, pos);
  }

} // end feedback
