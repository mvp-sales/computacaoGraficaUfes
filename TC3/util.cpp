#include <math.h>
#include <GL/freeglut.h>
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
	GLfloat twicePi = 2.0f * M_PI;
	glBegin(GL_TRIANGLE_FAN);
		decideColor(fill);
		glVertex3f(center.coordX,center.coordY,0.0);
		for(int i = 0; i <= triangleAmount; i++){
			glVertex3f(center.coordX + (radius * cos(i * twicePi/triangleAmount)),center.coordY + (radius * sin(i * twicePi/triangleAmount)),0.0);
		}
	glEnd();
}

void Circle::moveCenterX(float dx){
	center.coordX += dx;
}

void Circle::moveCenterY(float dy){
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
		glVertex3f(bottomLeft.coordX,bottomLeft.coordY,0.0); // bottom left
		glVertex3f(bottomLeft.coordX,bottomLeft.coordY + length,0.0); // top left
		glVertex3f(bottomLeft.coordX + width,bottomLeft.coordY + length,0.0); // top right
		glVertex3f(bottomLeft.coordX + width, bottomLeft.coordY,0.0); // bottom right
	glEnd();
}




void Carro::draw(){
	glPushMatrix();
		glTranslatef(referenceCircle.center.coordX,referenceCircle.center.coordY,0);

		glPushMatrix();
			if(inMovement) glRotatef(wheelAngle,0,0,1);
			drawRect((1.5)*referenceCircle.radius,(0.5)*referenceCircle.radius,0.604,0.804,0.196);

			glPushMatrix();
				glTranslatef(-(0.375)*referenceCircle.radius,-(1/3.0)*referenceCircle.radius,0);
				drawRect((0.125)*referenceCircle.radius,(0.25)*referenceCircle.radius,0.118,0.565,1.0);
			glPopMatrix();

			glPushMatrix();
				glTranslatef((0.375)*referenceCircle.radius,-(1/3.0)*referenceCircle.radius,0);
				drawRect((0.125)*referenceCircle.radius,(0.25)*referenceCircle.radius,0.118,0.565,1.0);
			glPopMatrix();

			glPushMatrix();
				glTranslatef((0.375)*referenceCircle.radius,(1/3.0)*referenceCircle.radius,0);
				drawRect((0.125)*referenceCircle.radius,(0.25)*referenceCircle.radius,0.118,0.565,1.0);
			glPopMatrix();

			glPushMatrix();
				glTranslatef(-(0.375)*referenceCircle.radius,(1/3.0)*referenceCircle.radius,0);
				drawRect((0.125)*referenceCircle.radius,(0.25)*referenceCircle.radius,0.118,0.565,1.0);
			glPopMatrix();

			glPushMatrix();
				glTranslatef(0,-(0.75)*referenceCircle.radius-0.15*referenceCircle.radius,0);
				drawRect((0.3)*referenceCircle.radius,(0.1)*referenceCircle.radius,34.0/255,139.0/255,34.0/255);
			glPopMatrix();
		glPopMatrix();

		glPushMatrix();
			glTranslatef((0.625)*referenceCircle.radius,-(1/3.0)*referenceCircle.radius,0);
			glRotatef(wheelAngle,0,0,1);
			drawWheel(referenceCircle.radius/4 + referenceCircle.radius/5,referenceCircle.radius/5);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-(0.625)*referenceCircle.radius,-(1/3.0)*referenceCircle.radius,0);
			glRotatef(wheelAngle,0,0,1);
			drawWheel(referenceCircle.radius/4 + referenceCircle.radius/5,referenceCircle.radius/5);
		glPopMatrix();

		glPushMatrix();
			glTranslatef((0.625)*referenceCircle.radius,(1/3.0)*referenceCircle.radius,0);
			glRotatef(wheelAngle,0,0,1);
			drawWheel(referenceCircle.radius/4 + referenceCircle.radius/5,referenceCircle.radius/5);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-(0.625)*referenceCircle.radius,(1/3.0)*referenceCircle.radius,0);
			glRotatef(wheelAngle,0,0,1);
			drawWheel(referenceCircle.radius/4 + referenceCircle.radius/5,referenceCircle.radius/5);
		glPopMatrix();
	glPopMatrix();


}

void Carro::moveAhead(float d){
	referenceCircle.center.coordY += d;
}

void Carro::turn(float angle){
	wheelAngle += angle;
}


void drawWheel(float h,float w){

	//float x = fabs((h/2.0-2.0*w)/3.0);
	float x = fabs(h/2 - w);
	//cout<<x<<std::endl;
	glPushMatrix();
	drawRect(h,w,0.2,0.2,0.2);

	glPushMatrix();
		glTranslatef(-w/2,h/2,0);
		glRotatef(90,0,0,1);
		glBegin    (GL_TRIANGLE_FAN);
	    glVertex2f (0,0);
	    for (int i = 0; i <= 100; i++) {
	        double angle = 0.5f*M_PI*i/100;
	        glVertex2f (cos(angle)*x,sin(angle)*x);

	    }
	    glEnd ();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(w/2,h/2,0);
		glBegin    (GL_TRIANGLE_FAN);
	    glVertex2f (0,0);
	    for (int i = 0; i <= 100; i++) {
	        double angle = 0.5f*M_PI*i/100;
	        glVertex2f (cos(angle)*x,sin(angle)*x);

	    }
	    glEnd ();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-w/2,-h/2,0);
		glRotatef(180,0,0,1);
		glBegin    (GL_TRIANGLE_FAN);
	    glVertex2f (0,0);
	    for (int i = 0; i <= 100; i++) {
	        double angle = 0.5f*M_PI*i/100;
	        glVertex2f (cos(angle)*x,sin(angle)*x);

	    }
	    glEnd ();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(w/2,-h/2,0);
		glRotatef(-90,0,0,1);
		glBegin    (GL_TRIANGLE_FAN);
	    glVertex2f (0,0);
	    for (int i = 0; i <= 100; i++) {
	        double angle = 0.5f*M_PI*i/100;
	        glVertex2f (cos(angle)*x,sin(angle)*x);

	    }
	    glEnd ();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-w/2-x/2,0,0);
		glRotatef(-90,0,0,1);
		drawRect(x,h,0.2,0.2,0.2);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(w/2+x/2,0,0);
		glRotatef(90,0,0,1);
		drawRect(x,h,0.2,0.2,0.2);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0,-h/2-x/2,0);
		drawRect(x,w,0.2,0.2,0.2);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0,h/2+x/2,0);
		drawRect(x,w,0.2,0.2,0.2);
	glPopMatrix();

	glPopMatrix();

	//return h + 2*x;

}



void drawRect(float h,float w,float R,float G,float B){
	glColor3f(R,G,B);

	glBegin(GL_QUADS);
		glVertex2f(-w/2.0,-h/2.0);
		glVertex2f(-w/2.0,h/2.0);
		glVertex2f(w/2.0,h/2.0);
		glVertex2f(w/2.0,-h/2.0);
	glEnd();
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

	delete doc;
	delete docSVG;
}






void processaArquivoSVG(const XMLNode* node, Circle& biggerCircle,Circle& smallerCircle,
						Carro& player,std::vector<Circle>& enemies,Rectangle& finishLine){
	for(const XMLNode* sonNode = node->FirstChild(); sonNode != NULL; sonNode = sonNode->NextSibling()){
		const XMLElement* elem = sonNode->ToElement();
		const char* color = elem->Attribute("fill");
		if(!strcmp(color,"blue")){
			elem->QueryFloatAttribute("cx",&biggerCircle.center.coordX);
			elem->QueryFloatAttribute("cy",&biggerCircle.center.coordY);
			elem->QueryFloatAttribute("r",&biggerCircle.radius);
			biggerCircle.fill = color;
		}else if(!strcmp(color,"white")){
				elem->QueryFloatAttribute("cx",&smallerCircle.center.coordX);
				elem->QueryFloatAttribute("cy",&smallerCircle.center.coordY);
				elem->QueryFloatAttribute("r",&smallerCircle.radius);
				smallerCircle.fill = color;
		}else if(!strcmp(color,"red")){
				Circle enemy;
				elem->QueryFloatAttribute("cx",&enemy.center.coordX);
				elem->QueryFloatAttribute("cy",&enemy.center.coordY);
				elem->QueryFloatAttribute("r",&enemy.radius);
				enemy.fill = color;
				enemies.push_back(enemy);
		}else if(!strcmp(color,"black")){
				elem->QueryFloatAttribute("x",&finishLine.bottomLeft.coordX);
				elem->QueryFloatAttribute("y",&finishLine.bottomLeft.coordY);
				elem->QueryFloatAttribute("width",&finishLine.width);
				elem->QueryFloatAttribute("height",&finishLine.length);
				finishLine.fill = elem->Attribute("fill");
		}else{
			elem->QueryFloatAttribute("cx",&player.referenceCircle.center.coordX);
			elem->QueryFloatAttribute("cy",&player.referenceCircle.center.coordY);
			elem->QueryFloatAttribute("r",&player.referenceCircle.radius);
			player.referenceCircle.fill = elem->Attribute("fill");
		}
	}
}