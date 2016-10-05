#include <iostream>
#include <vector>

struct Point{
	float coordX,coordY;

	double distPoints(Point p);
};

struct Circle{
	Point center;
	float radius;
	std::string fill,id;

	void drawCircle();
};

struct Rectangle{
	Point bottomLeft;
	float width,length;
	std::string fill,id;

	void drawRectangle();
};

bool colisaoEnemies(std::vector<Circle>&,Circle);
bool colisaoCircMaior(Circle,Circle);
bool colisao(Circle,Circle);
void decideColor(std::string);