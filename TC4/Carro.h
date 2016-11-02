#ifndef CARRO_H
#define CARRO_H

#include "util.h"
#include <list>

struct Bullet;

struct Carro{
	double wheelAngle,cannonAngle,carPartsAngle,carSpeed,bulletSpeed;
	int dMove,dWheel,dCannon;
	int rMove,rWheel;
	Circle referenceCircle;

	Carro();
	Carro(double,double);
	void draw();
	void moveAhead(int,GLdouble);
	void turn(double);
	void drawMovement();
	void setBasePos();
	void shoot(std::list<Bullet>&);
	bool colisaoEnemies(std::list<Carro>&);

	private:
		double dLine1,dLine2,dLine3;
		double bPosLine1,bPosLine2,bPosLine3;
		void drawSuspensoes(double,double);
		void drawCarWheel(double,double,bool);
		void addMovementAnimation(int d);
};

struct Bullet{
	double speed,angle;
	Point position;
	Circle referenceCircle;

	//Bullet(double,double,double,double,Point);
	Bullet(Carro&);
	void draw();
	void updatePosition(GLdouble);
	double getCoordX();
	double getCoordY();
};

#endif
