#ifndef CARRO_H
#define CARRO_H

#include "util.h"

struct Carro{
	double wheelAngle,cannonAngle,carPartsAngle,carSpeed,bulletSpeed;
	Circle referenceCircle;

	//Carro();
	void draw();
	void moveAhead(int,int,GLdouble);
	void turn(double);
	void drawMovement();
	void setBasePos();

	private:
		double dLine1,dLine2,dLine3;
		double bPosLine1,bPosLine2,bPosLine3;
		void drawSuspensoes(double,double);
		void drawCarWheel(double,double,bool);
		void addMovementAnimation(int d);
};

struct Bullet{
	double speed;
	Point position;
	Circle referenceCircle;
	double angleCar,angleCannon,carRadius;

	//Bullet(double,double,double,double,Point);
	Bullet(Carro&);
	void draw();
	void updatePosition(GLdouble);
};

#endif
