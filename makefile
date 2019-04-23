termProject: final/evalHangmanPlayer.c final/hangmanPlayer.c
	gcc -o hangman final/evalHangmanPlayer.c final/hangmanPlayer.c -lm

debug: src/evalHangmanPlayer.c src/hangmanPlayer.c
	gcc -ggdb -o hangman final/evalHangmanPlayer.c final/hangmanPlayer.c -lm

remove: 
	rm ./hangman

run: 
	./hangman data/words.txt data/hiddenWords1.txt 

run2:
	./hangman data/words.txt data/hiddenWords2.txt 

output: 
	./hangman data/words.txt data/hiddenWords1.txt > ./test/output2.txt
