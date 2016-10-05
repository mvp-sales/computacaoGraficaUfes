#include <math.h>
#include <GL/freeglut.h>
#include <string.h>
#include "util.h"

double Point::distPoints(Point p){
	return sqrt(pow(coordX - p.coordX,2) + pow(coordY - p.coordY,2));
}

void Circle::drawCircle(){
	GLfloat twicePi = 2.0f * M_PI;
	glBegin(GL_TRIANGLE_FAN);
		decideColor(fill);
		glVertex3f(center.coordX,center.coordY,0.0);
		for(int i = 0; i <= 360; i++){
			glVertex3f(center.coordX + (radius * cos(i * twicePi/360)),center.coordY + (radius * sin(i * twicePi/360)),0.0);
		}
	glEnd();
}
void Rectangle::drawRectangle(){
	glBegin(GL_QUADS);
		decideColor(fill);
		glVertex3f(bottomLeft.coordX,bottomLeft.coordY,0.0); // bottom left
		glVertex3f(bottomLeft.coordX,bottomLeft.coordY + length,0.0); // top left
		glVertex3f(bottomLeft.coordX + width,bottomLeft.coordY + length,0.0); // top right
		glVertex3f(bottomLeft.coordX + width, bottomLeft.coordY,0.0); // bottom right
	glEnd();
}

bool colisaoCircMaior(Circle biggerCircle,Circle player){
	if(biggerCircle.center.distPoints(player.center) <= biggerCircle.radius - player.radius){
		return false;
	}
	return true;
}

bool colisao(Circle c,Circle player){
	if(c.center.distPoints(player.center) >= c.radius + player.radius){
		return false;
	}
	return true;
}

bool colisaoEnemies(std::vector<Circle>& enemies,Circle player){
	for(int i = 0; i < enemies.size(); ++i){
		if(colisao(enemies.at(i),player)){
			return true;
		}
	}
	return false;
}

void decideColor(std::string color){
	if(!strcmp(color.c_str(),"red")){
		glColor3f(1,0,0);
	}else if(!strcmp(color.c_str(),"blue")){
		glColor3f(0,0,1);
	}else if(!strcmp(color.c_str(),"green")){
		glColor3f(0,1,0);
	}else if(!strcmp(color.c_str(),"white")){
		glColor3f(1,1,1);
	}else{
		glColor3f(0,0,0);
	}
}