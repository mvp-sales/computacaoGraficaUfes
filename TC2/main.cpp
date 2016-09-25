#include <GL/freeglut.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <cmath>
#include "tinyxml2.h"

#define _USE_MATH_DEFINES

//NAMESPACES
using tinyxml2::XMLDocument;
using tinyxml2::XMLNode;
using tinyxml2::XMLElement;
using std::cout;
using std::vector;

struct Circle{
	float centerX,centerY,radius;
	std::string fill,id;
};

struct Rectangle{
	float bottomX,bottomY,width,length;
	std::string fill,id;
};

int alturaJanela,larguraJanela;
Circle biggerCircle, smallerCircle, player;
vector<Circle> enemies;
Rectangle finishLine;

bool keyStatus[256];


void processaArquivoSVG(const XMLNode* node);
void display();
void init(void);
void decideColor(std::string color);
void drawCircle(Circle c);
void mouse(int,int,int,int);
void keyUp(unsigned char,int,int);
void keyPress(unsigned char,int,int);
void idle();
bool colisaoEnemies();
bool colisaoCircMaior();
bool colisao();

int main(int argc, char** argv) {
	std::string path = argv[1],
				filename = "config.xml";
	path = path + filename;

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
	if(pathSVG.at(0) == '~'){
		pathSVG = std::string(getenv("HOME")) + pathSVG.substr(1,pathSVG.length()-1);
	}
	pathSVG = pathSVG + name + dot+ ext;

	XMLDocument* docSVG = new XMLDocument;
	if(docSVG->LoadFile(pathSVG.c_str())){
		printf("Erro na leitura do arquivo arena.svg\n");
		exit(1);
	}

	const XMLNode* rootSVG = docSVG->FirstChild();

	processaArquivoSVG(rootSVG);

	larguraJanela = biggerCircle.radius * 2;
	alturaJanela = biggerCircle.radius * 2;

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(larguraJanela, alturaJanela);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("Tela");

	init();

	glutMouseFunc(mouse);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyPress);
	glutKeyboardUpFunc(keyUp);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}

bool colisaoCircMaior(){
	double distCentros = sqrt(pow(biggerCircle.centerX - player.centerX,2) +
														pow(biggerCircle.centerY - player.centerY,2));
	if(distCentros < biggerCircle.radius - player.radius){
		return false;
	}
	return true;
}

bool colisao(Circle c){
	double distCentros = sqrt(pow(c.centerX - player.centerX,2) +
														pow(c.centerY - player.centerY,2));
	if(distCentros > c.radius + player.radius){
		return false;
	}
	return true;
}

bool colisaoEnemies(){
	for(int i = 0; i < enemies.size(); ++i){
		if(colisao(enemies.at(i))){
			return true;
		}
	}
	return false;
}

void idle(){
	if(keyStatus['w'] || keyStatus['W']){
		player.centerY -= 2;
		if(colisaoCircMaior() || colisao(smallerCircle) || colisaoEnemies()){
			player.centerY += 2;
		}
	}
	if(keyStatus['s'] || keyStatus['S']){
		player.centerY += 2;
		if(colisaoCircMaior() || colisao(smallerCircle) || colisaoEnemies()){
			player.centerY -= 2;
		}
	}
	if(keyStatus['d'] || keyStatus['D']){
		player.centerX += 2;
		if(colisaoCircMaior() || colisao(smallerCircle) || colisaoEnemies()){
			player.centerX -= 2;
		}
	}
	if(keyStatus['a'] || keyStatus['A']){
		player.centerX -= 2;
		if(colisaoCircMaior() || colisao(smallerCircle) || colisaoEnemies()){
			player.centerX += 2;
		}
	}
	glutPostRedisplay();
}

void keyUp(unsigned char key,int x,int y){
	keyStatus[key] = false;
}

void keyPress(unsigned char key,int x,int y){
	keyStatus[key] = true;
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

void mouse(int button,int state,int x,int y){
	printf("%d e %d\n",x,alturaJanela-y);
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

void display(){
	glClearColor(1,1,1,0);
	glClear(GL_COLOR_BUFFER_BIT);

	drawCircle(biggerCircle);
	drawCircle(smallerCircle);
	drawRectangle(finishLine);
	for(int i = 0; i < enemies.size(); i++){
		drawCircle(enemies.at(i));
	}
	drawCircle(player);
	glutSwapBuffers();
}

void init(void){
	glClearColor(0.0,0.0,0.0,0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(biggerCircle.centerX-biggerCircle.radius,biggerCircle.centerX+biggerCircle.radius,
			biggerCircle.centerY+biggerCircle.radius,biggerCircle.centerY-biggerCircle.radius,
			-1.0,1.0);
}

void processaArquivoSVG(const XMLNode* node){
	for(const XMLNode* sonNode = node->FirstChild(); sonNode != nullptr; sonNode = sonNode->NextSibling()){
		const XMLElement* elem = sonNode->ToElement();
		const char* color = elem->Attribute("fill");
		if(!strcmp(color,"blue")){
			elem->QueryFloatAttribute("cx",&biggerCircle.centerX);
			elem->QueryFloatAttribute("cy",&biggerCircle.centerY);
			elem->QueryFloatAttribute("r",&biggerCircle.radius);
			biggerCircle.fill = color;
		}else if(!strcmp(color,"white")){
				elem->QueryFloatAttribute("cx",&smallerCircle.centerX);
				elem->QueryFloatAttribute("cy",&smallerCircle.centerY);
				elem->QueryFloatAttribute("r",&smallerCircle.radius);
				smallerCircle.fill = color;
		}else if(!strcmp(color,"red")){
				Circle enemy;
				elem->QueryFloatAttribute("cx",&enemy.centerX);
				elem->QueryFloatAttribute("cy",&enemy.centerY);
				elem->QueryFloatAttribute("r",&enemy.radius);
				enemy.fill = color;
				enemies.push_back(enemy);
		}else if(!strcmp(color,"black")){
				elem->QueryFloatAttribute("x",&finishLine.bottomX);
				elem->QueryFloatAttribute("y",&finishLine.bottomY);
				elem->QueryFloatAttribute("width",&finishLine.width);
				elem->QueryFloatAttribute("height",&finishLine.length);
				finishLine.fill = elem->Attribute("fill");
		}else{
			elem->QueryFloatAttribute("cx",&player.centerX);
			elem->QueryFloatAttribute("cy",&player.centerY);
			elem->QueryFloatAttribute("r",&player.radius);
			player.fill = elem->Attribute("fill");
		}
	}
}
