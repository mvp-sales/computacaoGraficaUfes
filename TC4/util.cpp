#include <math.h>
#include <string.h>
#include "util.h"
#include "Carro.h"

#define triangleAmount 360

using tinyxml2::XMLDocument;
using tinyxml2::XMLNode;
using tinyxml2::XMLElement;


Color::Color(double red,double green,double blue){
	this->red = red;
	this->green = green;
	this->blue = blue;
}


double Point::distPoints(Point p){
	return sqrt(pow(coordX - p.coordX,2) + pow(coordY - p.coordY,2));
}


double Circle::getCenterX(){
	return center.coordX;
}

double Circle::getCenterY(){
	return center.coordY;
}


double Circle::getCorR(){
	return cor->red;
}

double Circle::getCorG(){
	return cor->green;
}

double Circle::getCorB(){
	return cor->blue;
}


void Circle::draw(){
	GLdouble twicePi = 2.0f * M_PI;
	glPushMatrix();
		glTranslated(center.coordX,center.coordY,0);
		glBegin(GL_TRIANGLE_FAN);
			glColor3d(cor->red,cor->green,cor->blue);
			glVertex3d(0,0,0.0);
			for(int i = 0; i <= triangleAmount; i++){
				glVertex3d(radius * cos(i * twicePi/triangleAmount),radius * sin(i * twicePi/triangleAmount),0.0);
			}
		glEnd();
	glPopMatrix();
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







void Rectangle::draw(){
	glBegin(GL_QUADS);
		//decideColor(fill);
		glColor3d(cor->red,cor->green,cor->blue);
		glVertex3d(bottomLeft.coordX,bottomLeft.coordY,0.0); // bottom left
		glVertex3d(bottomLeft.coordX,bottomLeft.coordY + length,0.0); // top left
		glVertex3d(bottomLeft.coordX + width,bottomLeft.coordY + length,0.0); // top right
		glVertex3d(bottomLeft.coordX + width, bottomLeft.coordY,0.0); // bottom right
	glEnd();
}

void drawQuarterCircle(double dx,double dy,double x,double rotation){
	glPushMatrix();
		glTranslated(dx,dy,0);
		glRotated(rotation,0,0,1);
		glBegin    (GL_TRIANGLE_FAN);
		    glVertex2d (0,0);
		    for (int i = 0; i <= 100; i++) {
		        double angle = 0.5f*M_PI*i/100;
		        glVertex2d (cos(angle)*x,sin(angle)*x);

		    }
	    glEnd ();
	glPopMatrix();
}


void drawWheel(double h,double w){

	double x = fabs(h/2 - w);
	glPushMatrix();
		drawRect(h,w,0.7,0.7,0.7);


		drawQuarterCircle(-w/2,h/2,x,90);
		drawQuarterCircle(w/2,h/2,x,0);
		drawQuarterCircle(-w/2,-h/2,x,180);
		drawQuarterCircle(-w/2,-h/2,x,-90);


		drawPartWheel(-w/2-x/2,0,-90,x,h);
		drawPartWheel(w/2+x/2,0,90,x,h);
		drawPartWheel(0,-h/2-x/2,0,x,w);
		drawPartWheel(0,h/2+x/2,0,x,w);

	glPopMatrix();

}


void drawPartWheel(double dx,double dy,double rotation,double h,double w){
	glPushMatrix();
		glTranslated(dx,dy,0);
		glRotated(rotation,0,0,1);
		drawRect(h,w,0.2,0.2,0.2);
	glPopMatrix();
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

void drawCirc(double r,double R,double G,double B){
	double x,y;
	glColor3f(R,G,B);
	glBegin(GL_TRIANGLE_FAN);
		for(double i = 0; i <= 360.0; i+= 1.0){
			x = r * cos(M_PI*i/180);
			y = r * sin(M_PI*i/180);
			glVertex3d(x,y,0);
		}
	glEnd();

}


Color* decideColor(std::string color){
	if(!strcmp(color.c_str(),"red")){
		return new Color(1.0,0.0,0.0);
	}else if(!strcmp(color.c_str(),"blue")){
		return new Color(0.0,0.0,1.0);
	}else if(!strcmp(color.c_str(),"green")){
		return new Color(0.0,1.0,0.0);
	}else if(!strcmp(color.c_str(),"white")){
		return new Color(1.0,1.0,1.0);
	}else{
		return new Color(0.0,0.0,0.0);
	}
}





void processaConfig(std::string path,Circle& biggerCircle,Circle& smallerCircle,
						Carro& player,std::list<Carro>& enemies,Rectangle& finishLine,double& freqTiro){
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

	const XMLNode* carroAttr = svgLine->NextSibling();
	const XMLElement* carroAttrElem = carroAttr->ToElement();
	carroAttrElem->QueryDoubleAttribute("velTiro",&player.bulletSpeed);
	carroAttrElem->QueryDoubleAttribute("velCarro",&player.carSpeed);

	double velTiroEnemies,velCarroEnemies;
	const XMLElement* enemiesAttr = carroAttr->NextSibling()->ToElement();
	enemiesAttr->QueryDoubleAttribute("velTiro",&velTiroEnemies);
	enemiesAttr->QueryDoubleAttribute("velCarro",&velCarroEnemies);
	enemiesAttr->QueryDoubleAttribute("freqTiro",&freqTiro);

	XMLDocument* docSVG = new XMLDocument;
	if(docSVG->LoadFile(pathSVG.c_str())){
		printf("Erro na leitura do arquivo arena.svg\n");
		exit(1);
	}

	const XMLNode* rootSVG = docSVG->FirstChild();

	processaArquivoSVG(rootSVG,biggerCircle,smallerCircle,player,enemies,finishLine,velCarroEnemies,velTiroEnemies);


	delete doc;
	delete docSVG;
}






void processaArquivoSVG(const XMLNode* node, Circle& biggerCircle,Circle& smallerCircle,
						Carro& player,std::list<Carro>& enemies,Rectangle& finishLine,
						double velCarroEnemies,double velTiroEnemies){
	for(const XMLNode* sonNode = node->FirstChild(); sonNode != NULL; sonNode = sonNode->NextSibling()){
		const XMLElement* elem = sonNode->ToElement();
		const char* color = elem->Attribute("fill");
		Color* cor = decideColor(std::string(color));
		if(!strcmp(color,"blue")){
			elem->QueryDoubleAttribute("cx",&biggerCircle.center.coordX);
			elem->QueryDoubleAttribute("cy",&biggerCircle.center.coordY);
			elem->QueryDoubleAttribute("r",&biggerCircle.radius);
			biggerCircle.cor = cor;
		}else if(!strcmp(color,"white")){
				elem->QueryDoubleAttribute("cx",&smallerCircle.center.coordX);
				elem->QueryDoubleAttribute("cy",&smallerCircle.center.coordY);
				elem->QueryDoubleAttribute("r",&smallerCircle.radius);
				smallerCircle.cor = cor;
		}else if(!strcmp(color,"red")){
				Carro enemy = Carro(velTiroEnemies,velCarroEnemies);
				elem->QueryDoubleAttribute("cx",&enemy.referenceCircle.center.coordX);
				elem->QueryDoubleAttribute("cy",&enemy.referenceCircle.center.coordY);
				elem->QueryDoubleAttribute("r",&enemy.referenceCircle.radius);
				
				enemy.referenceCircle.cor = cor;
				enemy.setBasePos();

				enemy.dMove = rand() % 2 == 0? -1 : 1;
				enemy.dWheel = rand() % 2 == 0? 1 : -1;
				enemy.cannonAngle = rand() % 2 == 0? 1 : -1;

				enemy.rMove = rand() % 5000 + 1000;
				enemy.rWheel = rand() % 1001;

				enemies.push_back(enemy);
		}else if(!strcmp(color,"black")){
				elem->QueryDoubleAttribute("x",&finishLine.bottomLeft.coordX);
				elem->QueryDoubleAttribute("y",&finishLine.bottomLeft.coordY);
				elem->QueryDoubleAttribute("width",&finishLine.width);
				elem->QueryDoubleAttribute("height",&finishLine.length);
				finishLine.cor = cor;
		}else{
			elem->QueryDoubleAttribute("cx",&player.referenceCircle.center.coordX);
			elem->QueryDoubleAttribute("cy",&player.referenceCircle.center.coordY);
			elem->QueryDoubleAttribute("r",&player.referenceCircle.radius);
			player.referenceCircle.cor = cor;
			player.setBasePos();
		}
	}
}
