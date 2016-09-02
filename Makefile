all: compila

compila: 
	g++ -std=c++11 -o trabalhocg teste.cpp tinyxml2.cpp -lGL -lglut -lGLU

clean:
	rm *.o