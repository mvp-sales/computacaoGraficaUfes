#include "util.h"

struct Carro{
	double wheelAngle,cannonAngle,carPartsAngle,carSpeed,bulletSpeed;
	bool inMovement;
	Circle referenceCircle;

	//Carro();
	void draw();
	void moveAhead(int,int,GLdouble);
	void turn(double);
	void drawMovement(double,double);
	void setBasePos();

	private:
		double dLine1,dLine2,dLine3;
		double bPosLine1,bPosLine2,bPosLine3;
		void drawSuspensoes(double,double);
		void drawCarWheel(double,double,bool);
		void addMovementAnimation(double d);
};

struct Bullet{
	double speed;
	Point position;
	double angleCar,angleCannon,carRadius;

	Bullet(double,double,double,double,Point);
	void draw();
	void updatePosition(GLdouble);
};
