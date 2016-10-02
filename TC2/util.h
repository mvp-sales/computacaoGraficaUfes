#include <iostream>
#include <vector>

struct Circle{
	float centerX,centerY,radius;
	std::string fill,id;
};

struct Rectangle{
	float bottomX,bottomY,width,length;
	std::string fill,id;
};

bool colisaoEnemies(std::vector<Circle>&,Circle);
bool colisaoCircMaior(Circle,Circle);
bool colisao(Circle,Circle);
void decideColor(std::string);
void drawCircle(Circle);
void drawRectangle(Rectangle);