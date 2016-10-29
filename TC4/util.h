#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <vector>
#include <GL/freeglut.h>
#include "tinyxml2.h"


struct Carro;

struct Color{
	double red,green,blue;

	Color(double,double,double);
};

//ESTRUTURAS
struct Point{
	double coordX,coordY;

	double distPoints(Point);
};

struct Circle{
	Point center;
	double radius;
	std::string fill,id;
	Color* cor;

	double getCorR();
	double getCorG();
	double getCorB();
	void draw();
	void moveCenterX(double);
	void moveCenterY(double);
	bool colisaoExterna(Circle);
	bool colisaoInterna(Circle);
	//bool colisaoEnemies(std::vector<Circle>&);
};

struct Rectangle{
	Point bottomLeft;
	double width,length;
	std::string fill,id;
	Color* cor;

	void draw();
};

void drawPartWheel(double dx,double dy,double rotation,double h,double w);
void drawQuarterCircle(double dx,double dy,double x,double rotation);
void drawWheel(double h,double w);
void drawRect(double h,double w,double R,double G,double B);
void drawCirc(double r,double R,double G,double B);
Color* decideColor(std::string);
void processaConfig(std::string,Circle&,Circle&,Carro&,std::vector<Carro>&,Rectangle&);
void processaArquivoSVG(const tinyxml2::XMLNode*,Circle&,Circle&,Carro&,std::vector<Carro>&,Rectangle&);

#endif
