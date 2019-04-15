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
#include <ctype.h> //tolower case function
#include <unistd.h> //sleep???
#include "hangmanTrie.h"

Trie *trie;

// initialize data structures from the word file
void init_hangman_player(char* word_file)
{
  FILE *filename = fopen(word_file, "r");
  char *line = NULL;
  int counter = 0;
  size_t len_of_line = 0;

  trie = initTrie();

  if (filename == NULL){
    printf("Error file not found");
    exit(-1);
  }

  while(getline(&line, &len_of_line, filename) >= 0){
    // printf("before: %s", line);
    for(int i = 0; i < strlen(line)-2; i++){
      line[i] = tolower(line[i]);
    }
    // printf("after: %s", line);

    insert(trie, line);
    // sleep(1);
    counter++;
    // if(counter == 10){
    //   break;
    // }
  }  
  printf("%d\n", trie->nodeCount);
  //run peak memory
  fclose(filename);
  //make a trie
  //
}

// based on the current (partially filled or intitially blank) word, guess a letter
// current_word: current word -- may contain blanks, terminated by a null char as usual
// is_new_word: indicates a new hidden word (current_word has blanks)
// returns the guessed letter
// Assume all letters in a word are in lower case
char guess_hangman_player(char* current_word, bool is_new_word)
{ 
  static bool guessedLetters[ALPHABET_SIZE];
  if (is_new_word){
    for(int i = 0; i < ALPHABET_SIZE; i++){
      guessedLetters[i] = false;
    }
    //reset booleans
  }
  else {
    //prune the tree based on boolean array
    
  }
  char guess = ' ';
  
  // for(int i = 0; i < ALPHABET_SIZE; i++) printf("Bool: %d\n",guessedLetters[i]);

  int random = rand()%ALPHABET_SIZE;
  while(!guessedLetters[random]){
    if(!guessedLetters[random]){
      guessedLetters[random] = true;
      guess = INDEX_TO_CHAR(random);
      printf("random: %d ", random);
      printf("ascii: %d ", INDEX_TO_CHAR(random));
      printf("character: %c\n", guess);
      break;
    }
    random = rand()%ALPHABET_SIZE;
  }
  // for(int i = 0; i < ALPHABET_SIZE; i++){
  //   if(!guessedLetters[i]){
  //     printf("ascii: %d ", INDEX_TO_CHAR(i));
  //     guess = INDEX_TO_CHAR(i);
  //     guessedLetters[i] = true;
  //     printf("character: %c\n", guess);
  //     break;
  //   }
  // }
  // for(int i = 0; i < ALPHABET_SIZE; i++) printf("Bool: %d\n",guessedLetters[i]);
  
  printf("length of the hidden word: %lu\n",strlen(current_word));
  printf("word: %s\n", current_word);

  // sleep(1);
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
void feedback_hangman_player(bool is_correct_guess, char* current_word)
{ //if true we then narrow our search
  if(is_correct_guess){
    //narrow down tree based on the correct guesses
    //look at the current word and check which letters has been flagged
    //do some more stuff PLEASE
  }
  else {
    //cry
  }
}

