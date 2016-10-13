#include <iostream>
#include <vector>
#include "tinyxml2.h"


//ESTRUTURAS
struct Point{
	float coordX,coordY;

	double distPoints(Point);
};

struct Circle{
	Point center;
	float radius;
	std::string fill,id;

	void drawCircle();
	void moveCenterX(float);
	void moveCenterY(float);
	bool colisaoExterna(Circle);
	bool colisaoInterna(Circle);
	bool colisaoEnemies(std::vector<Circle>&);
};

struct Rectangle{
	Point bottomLeft;
	float width,length;
	std::string fill,id;

	void drawRectangle();
};

struct Carro{
	float wheelAngle,cannonAngle,carPartsAngle,carSpeed,bulletSpeed;
	bool inMovement;
	Circle referenceCircle;

	void draw();
	void moveAhead(float);
	void turn(float);
};

void drawWheel(float h,float w);
void drawRect(float h,float w,float R,float G,float B);
void decideColor(std::string);
void processaConfig(std::string,Circle&,Circle&,Carro&,std::vector<Circle>&,Rectangle&);
void processaArquivoSVG(const tinyxml2::XMLNode*,Circle&,Circle&,Carro&,std::vector<Circle>&,Rectangle&);
