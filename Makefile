all: compile link

compile:
	g++ -I src/include -c main.cpp
link:
	g++ main.o -o a -L src/lib -l sfml-graphics -l sfml-window -l sfml-system