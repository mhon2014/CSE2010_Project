main: src/evalHangmanPlayer.c src/hangmanPlayer.c
	gcc -o bin/p src/evalHangmanPlayer.c src/hangmanPlayer.c -lm

remove: 
	rm ./bin/p

run: 
	./bin/p data/words.txt data/hiddenWords1.txt 

run2:
	./bin/p data/words.txt data/hiddenWords2.txt 

output: 
	./bin/p data/words.txt data/hiddenWords1.txt > ./test/output2.txt

debug: 
	gcc -ggdb -o bin/p src/evalHangmanPlayer.c src/hangmanPlayer.c -lm