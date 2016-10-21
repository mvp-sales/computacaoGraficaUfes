#include "Carro.h"
#include <math.h>

void Carro::setBasePos(){
	bPosLine1 = -referenceCircle.radius/8;
	bPosLine2 = 0;
	bPosLine3 = referenceCircle.radius/8;
	dLine1 = dLine2 = dLine3 = 0;
}

void Carro::drawSuspensoes(double dx,double dy){
	glPushMatrix();
		glTranslated(dx,dy,0);
		drawRect((0.125)*referenceCircle.radius,(0.25)*referenceCircle.radius,0.118,0.565,1.0);
	glPopMatrix();
}

void Carro::drawCarWheel(double dx,double dy,bool rotate){
	glPushMatrix();
		glTranslated(dx,dy,0);
		if(rotate)
			glRotated(wheelAngle,0,0,1);
		drawWheel(referenceCircle.radius/4 + referenceCircle.radius/5,referenceCircle.radius/5);
		drawMovement(-referenceCircle.radius/10,(3/8.0)*referenceCircle.radius);
		//drawMovement(referenceCircle.radius/10,(3/8.0)*referenceCircle.radius);*/
	glPopMatrix();
}

void Carro::draw(){
	glPushMatrix();
		glTranslated(referenceCircle.center.coordX,referenceCircle.center.coordY,0);
		glRotated(carPartsAngle,0,0,1);

		glPushMatrix();
			drawRect((1.5)*referenceCircle.radius,(0.5)*referenceCircle.radius,0.604,0.804,0.196);

			//MODULARIZAR
			drawSuspensoes(-(0.375)*referenceCircle.radius,-(1/2.0)*referenceCircle.radius);

			drawSuspensoes((0.375)*referenceCircle.radius,-(1/2.0)*referenceCircle.radius);

			drawSuspensoes((0.375)*referenceCircle.radius,(1/2.0)*referenceCircle.radius);

			drawSuspensoes(-(0.375)*referenceCircle.radius,(1/2.0)*referenceCircle.radius);
			//MODULARIZAR

			glPushMatrix();
				glTranslated(0,-(0.75)*referenceCircle.radius,0);
				glRotated(cannonAngle,0,0,1);
				glTranslated(0,-0.125*referenceCircle.radius,0);
				drawRect((0.25)*referenceCircle.radius,(0.1)*referenceCircle.radius,34.0/255,139.0/255,34.0/255);
			glPopMatrix();
		glPopMatrix();


		// MODULARIZAR
		this->drawCarWheel((0.625)*referenceCircle.radius,-(1/2.0)*referenceCircle.radius,true);

		this->drawCarWheel(-(0.625)*referenceCircle.radius,-(1/2.0)*referenceCircle.radius,true);

		this->drawCarWheel((0.625)*referenceCircle.radius,(1/2.0)*referenceCircle.radius,false);

		this->drawCarWheel(-(0.625)*referenceCircle.radius,(1/2.0)*referenceCircle.radius,false);

		//MODULARIZAR
	glPopMatrix();


}

void Carro::drawMovement(double dx,double dy){
	if(inMovement){
		/*glPushMatrix();
			glTranslated(dx,dy,0);
			glColor3d(231.0/255.0,233.0/255.0,2.0/3.0);
			//drawRect(referenceCircle.radius/4,referenceCircle.radius/12,0.5,0.5,0.5);
		glPopMatrix();*/
	}else{
		//glPushMatrix();
			glBegin(GL_LINES);
				glColor3d(0.0,0.0,0.0);
				glVertex2d(-referenceCircle.radius/8,bPosLine1 + dLine1);
				glVertex2d(referenceCircle.radius/8,bPosLine1 + dLine1);

				glVertex2d(-referenceCircle.radius/8,bPosLine2 + dLine2);
				glVertex2d(referenceCircle.radius/8,bPosLine2 + dLine2);

				glVertex2d(-referenceCircle.radius/8,bPosLine3 + dLine3);
				glVertex2d(referenceCircle.radius/8,bPosLine3 + dLine3);
			glEnd();
		//glPopMatrix();
	}
}

void Carro::addMovementAnimation(double d){
	/*dLine1 -=0.125;dLine2 -= 0.125;dLine3 -=0.125;

	if(bPosLine1 + dLine1 < -referenceCircle.radius/4){
		this->setBasePos();
	}*/
}

void Carro::moveAhead(int dy,int dx,GLdouble timediff){

	if(dy == 1 && (dx == 1 || dx == -1)){
		/*referenceCircle.center.coordX += carSpeed*timediff*sin((-carPartsAngle+wheelAngle)*M_PI/180);
		referenceCircle.center.coordY += timediff*carSpeed*dy*cos((-carPartsAngle+wheelAngle)*M_PI/180);*/
		referenceCircle.moveCenterX(carSpeed*timediff*sin((-carPartsAngle+wheelAngle)*M_PI/180));
		referenceCircle.moveCenterY(timediff*carSpeed*dy*cos((-carPartsAngle+wheelAngle)*M_PI/180));

		if(wheelAngle != 0){
			carPartsAngle += (-1)*(0.5)*wheelAngle/fabs(wheelAngle);
		}
	}/*else if(dy == 1 && dx == -1){
		referenceCircle.center.coordX += carSpeed*timediff*sin((-carPartsAngle+wheelAngle)*M_PI/180);
		referenceCircle.center.coordY += timediff*carSpeed*dy*cos((-carPartsAngle+wheelAngle)*M_PI/180);

		if(wheelAngle != 0){
			carPartsAngle += (-1)*(0.5)*wheelAngle/fabs(wheelAngle);
		}
	}*/else{
		/*referenceCircle.center.coordX += (-dy)*carSpeed*timediff*sin((carPartsAngle+wheelAngle)*M_PI/180);
		referenceCircle.center.coordY += timediff*carSpeed*dy*cos((carPartsAngle+wheelAngle)*M_PI/180);*/
		referenceCircle.moveCenterX((-dy)*carSpeed*timediff*sin((carPartsAngle+wheelAngle)*M_PI/180));
		referenceCircle.moveCenterY(timediff*carSpeed*dy*cos((carPartsAngle+wheelAngle)*M_PI/180));

		if(wheelAngle != 0){
			carPartsAngle += (0.5)*wheelAngle/fabs(wheelAngle);
		}
	}
	inMovement = true;

}

void Carro::turn(double angle){
	wheelAngle += angle;
}


Bullet::Bullet(double speed,double angleCar,double angleCannon,double carRadius,Point position){
	this->speed = speed;
	this->angleCar = angleCar;
	this->angleCannon = angleCannon;
	this->carRadius = carRadius;
	this->position = position;
}


void Bullet::updatePosition(GLdouble timeDiff){
	position.coordX += timeDiff*speed*sin((angleCar+angleCannon)*M_PI/180);
	position.coordY += (-1)*timeDiff*speed*cos((angleCar+angleCannon)*M_PI/180);
}

void Bullet::draw(){
	glPushMatrix();
		glTranslated(position.coordX,position.coordY,0);
		glRotated(angleCar,0,0,1);
		glTranslated(0,(-0.75)*carRadius,0);
		glRotated(angleCannon,0,0,1);
		glTranslated(0,(-0.3)*carRadius,0);
		drawCirc(0.1*carRadius,0.0,0.0,0.0);
	glPopMatrix();
}
