termProject: c/evalHangmanPlayer.c c/hangmanPlayer.c
	gcc -o p c/evalHangmanPlayer.c c/hangmanPlayer.c -lm

remove: 
	rm ./p

run: 
	./p data/words.txt data/hiddenWords1.txt 

testbuild: c/hangmanPlayer.c
	gcc -o test c/testHangman.c c/hangmanPlayer.c -lm

testrun:
	./test data/words.txt data/hiddenWords1.txt