#include <math.h>
#include <GL/freeglut.h>
#include <string.h>
#include "util.h"

bool colisaoCircMaior(Circle biggerCircle,Circle player){
	double distCentros = sqrt(pow(biggerCircle.centerX - player.centerX,2) +
														pow(biggerCircle.centerY - player.centerY,2));
	if(distCentros < biggerCircle.radius - player.radius){
		return false;
	}
	return true;
}

bool colisao(Circle c,Circle player){
	double distCentros = sqrt(pow(c.centerX - player.centerX,2) +
														pow(c.centerY - player.centerY,2));
	if(distCentros > c.radius + player.radius){
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

void drawCircle(Circle c){

	GLfloat twicePi = 2.0f * M_PI;
	glBegin(GL_TRIANGLE_FAN);
		decideColor(c.fill);
		glVertex3f(c.centerX,c.centerY,0.0);
		for(int i = 0; i <= 360; i++){
			glVertex3f(c.centerX + (c.radius * cos(i * twicePi/360)),c.centerY + (c.radius * sin(i * twicePi/360)),0.0);
		}
	glEnd();
}

void drawRectangle(Rectangle r){
	glBegin(GL_QUADS);
		decideColor(r.fill);
		glVertex3f(r.bottomX,r.bottomY,0.0); // bottom left
		glVertex3f(r.bottomX,r.bottomY + r.length,0.0); // top left
		glVertex3f(r.bottomX + r.width,r.bottomY + r.length,0.0); // top right
		glVertex3f(r.bottomX + r.width, r.bottomY,0.0); // bottom right
	glEnd();
}