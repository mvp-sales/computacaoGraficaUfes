all: tinyxml2.o
	g++ -o trabalhocg main.cpp tinyxml2.o -lGL -lglut -lGLU

tinyxml2.o: tinyxml2.cpp tinyxml2.h
	g++ -c tinyxml2.cpp 

clean:
	rm trabalhocg *.o
