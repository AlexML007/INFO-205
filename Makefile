sokoban: player.o  main.o 
	g++ player.o main.o -o sokoban -lfltk -lfltk_images
main.o: main.cpp
	g++ --std='c++20' -Wall -Wextra -pedantic -c main.cpp -o main.o -lfltk -lfltk_images
player.o: player.cpp 
	g++ --std='c++20' -Wall -Wextra -pedantic -c player.cpp -o player.o -lfltk -lfltk_images 
