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
char guess;

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
  guess = ' ';
  
  // reset guesses if new word
  if (is_new_word) {
    for(int i = 0; i < ALPHABET_SIZE; i++){
      guessedLetters[i] = false;
    }
  }


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
      /// for each character in word that matches guess



      //
    
  }

  else {

    char bad_letter = guess;
    eliminate_ancestors(bad_letter); // prune tree after bad guess
  
  }

} // end feedback

// initialize and return a new alpha node
AlphaNode* new_alpha_node() {

    AlphaNode* new_node = (AlphaNode*) malloc(sizeof(AlphaNode)); 

    // initialize parent and children pointers
    new_node->parent = NULL;
    for(int i = 0; i < ALPHABET_SIZE; i++){
        new_node->children[i] = NULL;
    } 

    // initialize other node fields
    new_node->depth = 0;
    new_node->is_candidate = true;
    new_node->end_of_word = false;

    return new_node;

}

//initialize and return a new trie
Trie *init_trie() { 

    Trie *newTrie = (Trie*)malloc(sizeof(Trie));

    newTrie->root = new_alpha_node();  // initialize root node
    newTrie->root->letter = '#';       // special character for root
    newTrie->nodeCount = 1;            // number of nodes in tree

    return newTrie;

} // end init_trie

// inserts given word into given trie
void insert(Trie *arg_trie, char *arg_word) {

    // variable declarations
    AlphaNode* new_node;                  // pointer for mallocing new nodes
    AlphaNode* current_node = arg_trie->root; // node pointer for traversal
    char current_letter;                  // stores letter as word is traversed

    // for each character in the word
    for (int i = 0; i < strlen(arg_word)-2; i++) {

        current_letter = arg_word[i];

        //if current node doesn't have this letter as child, initialize it
        if( (current_node->children[CHAR_TO_INDEX(current_letter)]) == NULL) {
            
            //initialise a new node
            new_node = new_alpha_node(); // initialize and allocate new node
            new_node->parent = current_node; // make it child of current node
            new_node->letter = current_letter; 
            new_node->depth = i+1;  // child level is one below current level
            current_node->children[CHAR_TO_INDEX(current_letter)] = new_node; // make parent point to new node
            arg_trie->nodeCount++; // increment size of trie
  
        } // end if

        current_node = current_node->children[CHAR_TO_INDEX(current_letter)]; // move to next level in trie
    
    } // end for loop

    current_node->end_of_word = true; // mark node of last letter as end_of_word
    
    return;

} // end insert function

// eliminate all paths that start with/ follow from the incorrect guess
void eliminate_ancestors(char bad_letter) {

  // recursively go through tree

  /// for every node equal to bad_letter (i.e. every ptr in "b" list)
  
      /// mark is_candidate false

      /// mark all ancestors with is_candidate false

  return;

} // close eliminate_ancestors