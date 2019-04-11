termProject: c/evalHangmanPlayer.c c/hangmanPlayer.c
	gcc -o p c/evalHangmanPlayer.c c/hangmanPlayer.c -lm

remove: 
	rm ./p

run: 
	./p data/words.txt data/hiddenWords1.txt 