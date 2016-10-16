#include <iostream>
#include <vector>
#include <GL/freeglut.h>
#include "tinyxml2.h"


//ESTRUTURAS
struct Point{
	double coordX,coordY;

	double distPoints(Point);
};

struct Circle{
	Point center;
	double radius;
	std::string fill,id;

	void drawCircle();
	void moveCenterX(double);
	void moveCenterY(double);
	bool colisaoExterna(Circle);
	bool colisaoInterna(Circle);
	bool colisaoEnemies(std::vector<Circle>&);
};

struct Rectangle{
	Point bottomLeft;
	double width,length;
	std::string fill,id;

	void drawRectangle();
};

struct Carro{
	double wheelAngle,cannonAngle,carPartsAngle,carSpeed,bulletSpeed;
	Circle referenceCircle;

	void draw();
	void moveAhead(int,int,GLdouble);
	void turn(double);
};

struct Bullet{
	double speed;
	Point position;
	double angleCar,angleCannon,carRadius;

	Bullet(double,double,double,double,Point);
	void draw();
	void updatePosition(GLdouble);
};

void drawWheel(double h,double w);
void drawRect(double h,double w,double R,double G,double B);
void decideColor(std::string);
void processaConfig(std::string,Circle&,Circle&,Carro&,std::vector<Circle>&,Rectangle&);
void processaArquivoSVG(const tinyxml2::XMLNode*,Circle&,Circle&,Carro&,std::vector<Circle>&,Rectangle&);
