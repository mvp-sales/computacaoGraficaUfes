#include <math.h>
#include <string.h>
#include "util.h"

#define triangleAmount 360

using tinyxml2::XMLDocument;
using tinyxml2::XMLNode;
using tinyxml2::XMLElement;

double Point::distPoints(Point p){
	return sqrt(pow(coordX - p.coordX,2) + pow(coordY - p.coordY,2));
}





void Circle::drawCircle(){
	GLdouble twicePi = 2.0f * M_PI;
	glBegin(GL_TRIANGLE_FAN);
		decideColor(fill);
		glVertex3d(center.coordX,center.coordY,0.0);
		for(int i = 0; i <= triangleAmount; i++){
			glVertex3d(center.coordX + (radius * cos(i * twicePi/triangleAmount)),center.coordY + (radius * sin(i * twicePi/triangleAmount)),0.0);
		}
	glEnd();
}

void Circle::moveCenterX(double dx){
	center.coordX += dx;
}

void Circle::moveCenterY(double dy){
	center.coordY += dy;
}

bool Circle::colisaoExterna(Circle c){
	if(center.distPoints(c.center) >= c.radius + radius){
		return false;
	}
	return true;
}

bool Circle::colisaoInterna(Circle c){
	if(c.center.distPoints(center) <= c.radius - radius){
		return false;
	}
	return true;
}

bool Circle::colisaoEnemies(std::vector<Circle>& enemies){
	for(int i = 0; i < enemies.size(); ++i){
		if(this->colisaoExterna(enemies.at(i))){
			return true;
		}
	}
	return false;
}






void Rectangle::drawRectangle(){
	glBegin(GL_QUADS);
		decideColor(fill);
		glVertex3d(bottomLeft.coordX,bottomLeft.coordY,0.0); // bottom left
		glVertex3d(bottomLeft.coordX,bottomLeft.coordY + length,0.0); // top left
		glVertex3d(bottomLeft.coordX + width,bottomLeft.coordY + length,0.0); // top right
		glVertex3d(bottomLeft.coordX + width, bottomLeft.coordY,0.0); // bottom right
	glEnd();
}




void Carro::draw(){
	glPushMatrix();
		glTranslated(referenceCircle.center.coordX,referenceCircle.center.coordY,0);
		glRotated(carPartsAngle,0,0,1);

		glPushMatrix();
			drawRect((1.5)*referenceCircle.radius,(0.5)*referenceCircle.radius,0.604,0.804,0.196);

			glPushMatrix();
				glTranslated(-(0.375)*referenceCircle.radius,-(1/3.0)*referenceCircle.radius,0);
				drawRect((0.125)*referenceCircle.radius,(0.25)*referenceCircle.radius,0.118,0.565,1.0);
			glPopMatrix();

			glPushMatrix();
				glTranslated((0.375)*referenceCircle.radius,-(1/3.0)*referenceCircle.radius,0);
				drawRect((0.125)*referenceCircle.radius,(0.25)*referenceCircle.radius,0.118,0.565,1.0);
			glPopMatrix();

			glPushMatrix();
				glTranslated((0.375)*referenceCircle.radius,(1/3.0)*referenceCircle.radius,0);
				drawRect((0.125)*referenceCircle.radius,(0.25)*referenceCircle.radius,0.118,0.565,1.0);
			glPopMatrix();

			glPushMatrix();
				glTranslated(-(0.375)*referenceCircle.radius,(1/3.0)*referenceCircle.radius,0);
				drawRect((0.125)*referenceCircle.radius,(0.25)*referenceCircle.radius,0.118,0.565,1.0);
			glPopMatrix();

			glPushMatrix();
				glTranslated(0,-(0.75)*referenceCircle.radius,0);
				glRotated(cannonAngle,0,0,1);
				glTranslated(0,-0.125*referenceCircle.radius,0);
				drawRect((0.25)*referenceCircle.radius,(0.1)*referenceCircle.radius,34.0/255,139.0/255,34.0/255);
			glPopMatrix();
		glPopMatrix();

		glPushMatrix();
			glTranslated((0.625)*referenceCircle.radius,-(1/3.0)*referenceCircle.radius,0);
			glRotated(wheelAngle,0,0,1);
			drawWheel(referenceCircle.radius/4 + referenceCircle.radius/5,referenceCircle.radius/5);
		glPopMatrix();

		glPushMatrix();
			glTranslated(-(0.625)*referenceCircle.radius,-(1/3.0)*referenceCircle.radius,0);
			glRotated(wheelAngle,0,0,1);
			drawWheel(referenceCircle.radius/4 + referenceCircle.radius/5,referenceCircle.radius/5);
		glPopMatrix();

		glPushMatrix();
			glTranslated((0.625)*referenceCircle.radius,(1/3.0)*referenceCircle.radius,0);
			//glRotated(wheelAngle,0,0,1);
			drawWheel(referenceCircle.radius/4 + referenceCircle.radius/5,referenceCircle.radius/5);
		glPopMatrix();

		glPushMatrix();
			glTranslated(-(0.625)*referenceCircle.radius,(1/3.0)*referenceCircle.radius,0);
			//glRotated(wheelAngle,0,0,1);
			drawWheel(referenceCircle.radius/4 + referenceCircle.radius/5,referenceCircle.radius/5);
		glPopMatrix();
	glPopMatrix();


}

void Carro::moveAhead(int dy,int dx,GLdouble timediff){

	if(dy == 1 && dx == 1){
		referenceCircle.center.coordX += carSpeed*timediff*sin((-carPartsAngle+wheelAngle)*M_PI/180);
		referenceCircle.center.coordY += timediff*carSpeed*dy*cos((-carPartsAngle+wheelAngle)*M_PI/180);

		if(wheelAngle != 0){
			carPartsAngle += (-1)*(0.5)*wheelAngle/fabs(wheelAngle);
		}
	}else if(dy == 1 && dx == -1){
		referenceCircle.center.coordX += carSpeed*timediff*sin((-carPartsAngle+wheelAngle)*M_PI/180);
		referenceCircle.center.coordY += timediff*carSpeed*dy*cos((-carPartsAngle+wheelAngle)*M_PI/180);

		if(wheelAngle != 0){
			carPartsAngle += (-1)*(0.5)*wheelAngle/fabs(wheelAngle);
		}
	}else{
		referenceCircle.center.coordX += (-dy)*carSpeed*timediff*sin((carPartsAngle+wheelAngle)*M_PI/180);
		referenceCircle.center.coordY += timediff*carSpeed*dy*cos((carPartsAngle+wheelAngle)*M_PI/180);

		if(wheelAngle != 0){
			carPartsAngle += (0.5)*wheelAngle/fabs(wheelAngle);
		}
	}

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
		drawRect(0.1*carRadius,0.1*carRadius,0.0,0.0,0.0);
	glPopMatrix();
}



void drawWheel(double h,double w){

	double x = fabs(h/2 - w);
	glPushMatrix();
	drawRect(h,w,0.2,0.2,0.2);

	glPushMatrix();
		glTranslated(-w/2,h/2,0);
		glRotated(90,0,0,1);
		glBegin    (GL_TRIANGLE_FAN);
	    glVertex2d (0,0);
	    for (int i = 0; i <= 100; i++) {
	        double angle = 0.5f*M_PI*i/100;
	        glVertex2d (cos(angle)*x,sin(angle)*x);

	    }
	    glEnd ();
	glPopMatrix();

	glPushMatrix();
		glTranslated(w/2,h/2,0);
		glBegin    (GL_TRIANGLE_FAN);
	    glVertex2d (0,0);
	    for (int i = 0; i <= 100; i++) {
	        double angle = 0.5f*M_PI*i/100;
	        glVertex2d (cos(angle)*x,sin(angle)*x);

	    }
	    glEnd ();
	glPopMatrix();

	glPushMatrix();
		glTranslated(-w/2,-h/2,0);
		glRotated(180,0,0,1);
		glBegin    (GL_TRIANGLE_FAN);
	    glVertex2d (0,0);
	    for (int i = 0; i <= 100; i++) {
	        double angle = 0.5f*M_PI*i/100;
	        glVertex2d (cos(angle)*x,sin(angle)*x);

	    }
	    glEnd ();
	glPopMatrix();

	glPushMatrix();
		glTranslated(w/2,-h/2,0);
		glRotated(-90,0,0,1);
		glBegin    (GL_TRIANGLE_FAN);
	    glVertex2d (0,0);
	    for (int i = 0; i <= 100; i++) {
	        double angle = 0.5f*M_PI*i/100;
	        glVertex2d (cos(angle)*x,sin(angle)*x);

	    }
	    glEnd ();
	glPopMatrix();

	glPushMatrix();
		glTranslated(-w/2-x/2,0,0);
		glRotated(-90,0,0,1);
		drawRect(x,h,0.2,0.2,0.2);
	glPopMatrix();

	glPushMatrix();
		glTranslated(w/2+x/2,0,0);
		glRotated(90,0,0,1);
		drawRect(x,h,0.2,0.2,0.2);
	glPopMatrix();

	glPushMatrix();
		glTranslated(0,-h/2-x/2,0);
		drawRect(x,w,0.2,0.2,0.2);
	glPopMatrix();

	glPushMatrix();
		glTranslated(0,h/2+x/2,0);
		drawRect(x,w,0.2,0.2,0.2);
	glPopMatrix();

	glPopMatrix();

	//return h + 2*x;

}



void drawRect(double h,double w,double R,double G,double B){
	glColor3d(R,G,B);

	glBegin(GL_QUADS);
		glVertex2d(-w/2.0,-h/2.0);
		glVertex2d(-w/2.0,h/2.0);
		glVertex2d(w/2.0,h/2.0);
		glVertex2d(w/2.0,-h/2.0);
	glEnd();
}



void decideColor(std::string color){
	if(!strcmp(color.c_str(),"red")){
		glColor3d(1,0,0);
	}else if(!strcmp(color.c_str(),"blue")){
		glColor3d(0,0,1);
	}else if(!strcmp(color.c_str(),"green")){
		glColor3d(0,1,0);
	}else if(!strcmp(color.c_str(),"white")){
		glColor3d(1,1,1);
	}else{
		glColor3d(0,0,0);
	}
}





void processaConfig(std::string path,Circle& biggerCircle,Circle& smallerCircle,
						Carro& player,std::vector<Circle>& enemies,Rectangle& finishLine){
	XMLDocument* doc = new XMLDocument;
	if(doc->LoadFile(path.c_str())){
		printf("Erro na leitura do arquivo config.xml");
		exit(1);
	}

	const XMLNode* root = doc->FirstChild();
	const XMLNode* svgLine = root->FirstChild();

	const XMLElement* elem = svgLine->ToElement();

	std::string name = elem->FindAttribute("nome")->Value(),
				ext = elem->FindAttribute("tipo")->Value(),
				pathSVG = elem->FindAttribute("caminho")->Value(),dot = ".";

	pathSVG = pathSVG + name + dot+ ext;

	XMLDocument* docSVG = new XMLDocument;
	if(docSVG->LoadFile(pathSVG.c_str())){
		printf("Erro na leitura do arquivo arena.svg\n");
		exit(1);
	}

	const XMLNode* rootSVG = docSVG->FirstChild();

	processaArquivoSVG(rootSVG,biggerCircle,smallerCircle,player,enemies,finishLine);

	const XMLNode* carroAttr = svgLine->NextSibling();
	const XMLElement* carroAttrElem = carroAttr->ToElement();
	carroAttrElem->QueryDoubleAttribute("velTiro",&player.bulletSpeed);
	carroAttrElem->QueryDoubleAttribute("velCarro",&player.carSpeed);

	delete doc;
	delete docSVG;
}






void processaArquivoSVG(const XMLNode* node, Circle& biggerCircle,Circle& smallerCircle,
						Carro& player,std::vector<Circle>& enemies,Rectangle& finishLine){
	for(const XMLNode* sonNode = node->FirstChild(); sonNode != NULL; sonNode = sonNode->NextSibling()){
		const XMLElement* elem = sonNode->ToElement();
		const char* color = elem->Attribute("fill");
		if(!strcmp(color,"blue")){
			elem->QueryDoubleAttribute("cx",&biggerCircle.center.coordX);
			elem->QueryDoubleAttribute("cy",&biggerCircle.center.coordY);
			elem->QueryDoubleAttribute("r",&biggerCircle.radius);
			biggerCircle.fill = color;
		}else if(!strcmp(color,"white")){
				elem->QueryDoubleAttribute("cx",&smallerCircle.center.coordX);
				elem->QueryDoubleAttribute("cy",&smallerCircle.center.coordY);
				elem->QueryDoubleAttribute("r",&smallerCircle.radius);
				smallerCircle.fill = color;
		}else if(!strcmp(color,"red")){
				Circle enemy;
				elem->QueryDoubleAttribute("cx",&enemy.center.coordX);
				elem->QueryDoubleAttribute("cy",&enemy.center.coordY);
				elem->QueryDoubleAttribute("r",&enemy.radius);
				enemy.fill = color;
				enemies.push_back(enemy);
		}else if(!strcmp(color,"black")){
				elem->QueryDoubleAttribute("x",&finishLine.bottomLeft.coordX);
				elem->QueryDoubleAttribute("y",&finishLine.bottomLeft.coordY);
				elem->QueryDoubleAttribute("width",&finishLine.width);
				elem->QueryDoubleAttribute("height",&finishLine.length);
				finishLine.fill = elem->Attribute("fill");
		}else{
			elem->QueryDoubleAttribute("cx",&player.referenceCircle.center.coordX);
			elem->QueryDoubleAttribute("cy",&player.referenceCircle.center.coordY);
			elem->QueryDoubleAttribute("r",&player.referenceCircle.radius);
			player.referenceCircle.fill = elem->Attribute("fill");
		}
	}
}
