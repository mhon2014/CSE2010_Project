termProject: c/evalHangmanPlayer.c c/hangmanPlayer.c
	gcc -ggdb -o p c/evalHangmanPlayer.c c/hangmanPlayer.c -lm

remove: 
	rm ./p

run: 
	./p data/words.txt data/hiddenWords1.txt 

output: 
	./p data/words.txt data/hiddenWords1.txt > output2.txt
