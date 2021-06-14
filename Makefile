all: compile link

compile:
	g++ -I src/include -c main.cpp -fno-diagnostics-color 
link:
	g++ main.o -o a -L src/lib -mwindows -l sfml-graphics -l sfml-window -l sfml-system
