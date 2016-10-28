#include "Carro.h"
#include <math.h>

void Carro::setBasePos(){
	bPosLine1 = -referenceCircle.radius/12;
	bPosLine3 = referenceCircle.radius/12;
	dLine1 = dLine3 = 0;
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
		this->drawMovement();
	glPopMatrix();
}

void Carro::draw(){
	glPushMatrix();
		glTranslated(referenceCircle.center.coordX,referenceCircle.center.coordY,0);
		glRotated(carPartsAngle,0,0,1);

		glPushMatrix();
			drawRect((1.5)*referenceCircle.radius,(0.5)*referenceCircle.radius,0.604,0.804,0.196);
			drawCirc(5,1.0,1.0,0.09);

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

void Carro::drawMovement(){
		glBegin(GL_LINES);
			glColor3d(0.0,0.0,0.0);
			glVertex2d(-referenceCircle.radius/8,bPosLine1 + dLine1);
			glVertex2d(referenceCircle.radius/8,bPosLine1 + dLine1);


			glVertex2d(-referenceCircle.radius/8,bPosLine3 + dLine3);
			glVertex2d(referenceCircle.radius/8,bPosLine3 + dLine3);
		glEnd();
}

void Carro::addMovementAnimation(int d){
	dLine1 += d*referenceCircle.radius/24;dLine3 += d*referenceCircle.radius/24;

	if(bPosLine1 + dLine1 < -referenceCircle.radius/4){
		this->setBasePos();
	}else if(bPosLine3 + dLine3 > referenceCircle.radius/4){
		this->setBasePos();
	}
}

void Carro::moveAhead(int dy,int dx,GLdouble timediff){

	if(dy == 1 && (dx == 1 || dx == -1)){

		carPartsAngle += (-1)*wheelAngle*carSpeed*timediff/(1.25*referenceCircle.radius);
		referenceCircle.moveCenterX(carSpeed*timediff*sin((-carPartsAngle+wheelAngle)*M_PI/180));
		referenceCircle.moveCenterY(timediff*carSpeed*dy*cos((-carPartsAngle+wheelAngle)*M_PI/180));

	}else{
		carPartsAngle += wheelAngle*carSpeed*timediff/(1.25*referenceCircle.radius);
		referenceCircle.moveCenterX((-dy)*carSpeed*timediff*sin((carPartsAngle+wheelAngle)*M_PI/180));
		referenceCircle.moveCenterY(timediff*carSpeed*dy*cos((carPartsAngle+wheelAngle)*M_PI/180));

	}
	this->addMovementAnimation(dy);

}

void Carro::turn(double angle){
	wheelAngle += angle;
	if(wheelAngle > 45){
		wheelAngle = 45;
	}else if(wheelAngle < -45){
		wheelAngle = -45;
	}
}


Bullet::Bullet(Carro& player){
	this->speed = player.bulletSpeed;
	this->angleCar = player.carPartsAngle;
	this->angleCannon = player.cannonAngle;
	//this->carRadius = player.referenceCircle.radius;
	this->position = player.referenceCircle.center;

	double originCannonMatrix[3][3]={{cos(player.carPartsAngle*M_PI/180),-sin(player.carPartsAngle*M_PI/180),player.referenceCircle.center.coordX},
								{sin(player.carPartsAngle*M_PI/180),cos(player.carPartsAngle*M_PI/180),player.referenceCircle.center.coordY},{0,0,1}};
	double baseCannonChassiVector[3]={0,-0.75*player.referenceCircle.radius,1};
	double baseCannonGlobal[3];
	for(int i = 0; i < 3; i++){
		baseCannonGlobal[i] = 0.0;
		for(int j = 0; j < 3; j++){
			baseCannonGlobal[i] += originCannonMatrix[i][j]*baseCannonChassiVector[j];
		}
	}



	double originBaseCannonMatrix[3][3]={{cos((player.carPartsAngle+player.cannonAngle)*M_PI/180),-sin((player.carPartsAngle+player.cannonAngle)*M_PI/180),baseCannonGlobal[0]},
										{sin((player.carPartsAngle+player.cannonAngle)*M_PI/180),cos((player.carPartsAngle+player.cannonAngle)*M_PI/180),baseCannonGlobal[1]},{0,0,1}};
	double topCannonVector[3]={0,-0.25*player.referenceCircle.radius,1};
	double topCannonGlobal[3];
	for(int i = 0; i < 3; i++){
		topCannonGlobal[i]=0;
		for(int j = 0; j < 3 ; j++){
			topCannonGlobal[i] += originBaseCannonMatrix[i][j]*topCannonVector[j];
		}
	}
	this->referenceCircle.radius=0.05*player.referenceCircle.radius;
	this->referenceCircle.fill = "black";
	this->referenceCircle.center.coordX = topCannonGlobal[0];
	this->referenceCircle.center.coordY = topCannonGlobal[1];

}


void Bullet::updatePosition(GLdouble timeDiff){
	position.coordX += timeDiff*speed*sin((angleCar+angleCannon)*M_PI/180);
	position.coordY += (-1)*timeDiff*speed*cos((angleCar+angleCannon)*M_PI/180);
	this->referenceCircle.center.coordX += timeDiff*speed*sin((angleCar+angleCannon)*M_PI/180);
	this->referenceCircle.center.coordY += (-1)*timeDiff*speed*cos((angleCar+angleCannon)*M_PI/180);
}

void Bullet::draw(){
	/*glPushMatrix();
		glTranslated(position.coordX,position.coordY,0);
		glRotated(angleCar,0,0,1);
		glTranslated(0,(-0.75)*carRadius,0);
		glRotated(angleCannon,0,0,1);
		glTranslated(0,(-0.3)*carRadius,0);
		drawCirc(0.05*carRadius,0.0,0.0,0.0);
	glPopMatrix();*/
	this->referenceCircle.drawCircle();
}
