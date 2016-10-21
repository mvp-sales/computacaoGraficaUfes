all: tinyxml2.o util.o Carro.o
	g++ -o trabalhocg main.cpp tinyxml2.o util.o Carro.o -lGL -lglut -lGLU

util.o: util.cpp util.h
	g++ -c util.cpp

Carro.o: Carro.cpp Carro.h
	g++ -c Carro.cpp

tinyxml2.o: tinyxml2.cpp tinyxml2.h
	g++ -c tinyxml2.cpp

clean:
	rm trabalhocg *.o
