#include <GL/freeglut.h>
#include <cstdlib>
#include <iostream>
#include "tinyxml2.h"

//NAMESPACES
using tinyxml2::XMLDocument;
using tinyxml2::XMLNode;
using tinyxml2::XMLElement;
using std::cout;

struct Quadrado{
	int centroX,centroY;
	int corR,corG,corB;
	int tamanho;
};

// VARIÁVEIS GLOBAIS
int largura,altura,fundoR,fundoG,fundoB;
Quadrado quad;
const char* titulo;

bool estadoInsercao=true,estadoAlteracao=false;
bool left_button_pressed = false;
bool drag = false;

bool isSquareArea(int x,int y);
void drawSquare();
void mouseClick(int button,int state,int x,int y);
void dragAndDrop(int x,int y);
void display();
void init(void);
void processaJanela(const XMLNode* node);
void processaQuadrado(const XMLNode* node);

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {

	XMLDocument* doc = new XMLDocument;
	doc->LoadFile(argv[1]);

	const XMLNode* root = doc->FirstChild();
	const XMLNode* janela = root->FirstChild();
	const XMLNode* quadrado = root->LastChild();

	processaJanela(janela);
	processaQuadrado(quadrado);

	glutInitWindowPosition(0, 0); // Position the window's initial top-left corner
	glutInitWindowSize(largura, altura);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);                 // Initialize GLUT
	glutCreateWindow(titulo); // Create a window with the given title

	init();

	glutMotionFunc(dragAndDrop);
	glutMouseFunc(mouseClick);
	glutDisplayFunc(display); // Register display callback handler for window re-paint

	glutMainLoop();           // Enter the infinitely event-processing loop
	return 0;
}

bool isSquareArea(int x,int y){
	if((abs(quad.centroX - x) <= quad.tamanho/2.0) && (abs(quad.centroY - y) <= quad.tamanho/2.0)){
		return true;
	}
	return false;
}

void drawSquare(){

	float left = quad.centroX-quad.tamanho/2.0,
	 right = quad.centroX+quad.tamanho/2.0,
	 bottom = quad.centroY+quad.tamanho/2.0,
	 top = quad.centroY- quad.tamanho/2.0;


 glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
	 glColor3f(quad.corR,quad.corG,quad.corB);
	 glVertex3f(left, altura-top,0.0);    // x, y
	 glVertex3f( right, altura-top,0.0);
	 glVertex3f( right,  altura-bottom,0.0);
	 glVertex3f(left,altura-bottom,0.0);
 	glEnd();
}

void mouseClick(int button,int state,int x,int y){
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && estadoInsercao){

		quad.centroY = y;
		quad.centroX = x;

		drawSquare();
		glFlush();

		estadoInsercao = false;
		estadoAlteracao = true;
	}else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN && !estadoInsercao){
		if(isSquareArea(x,y)){
			glutPostRedisplay();
			estadoInsercao = true;
			estadoAlteracao = false;
		}
	}else if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && estadoAlteracao){
		left_button_pressed = true;
	}else{
		left_button_pressed = false;
	}
}

void dragAndDrop(int x,int y){
	if(left_button_pressed && isSquareArea(x,y)){
		quad.centroX = x;
		quad.centroY = y;
		drag = true;
		glutPostRedisplay();
	}
}

void display() {
   glClearColor(fundoR,fundoG,fundoB,0); // Set background color to black and opaque
   glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer
	 if(drag){
		drawSquare();
		drag = false;
	 }
   glFlush();  // Render now
}

void init(void){
	glClearColor(0.0,0.0,0.0,0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0,largura,0.0,altura,-1.0,1.0);
}

void processaJanela(const XMLNode* node){
	for(const XMLNode* sonNode = node->FirstChild(); sonNode != nullptr ; sonNode = sonNode->NextSibling()){
		const XMLElement* elem = sonNode->ToElement();
		if(!strcmp(sonNode->Value(),"largura")){
			largura = atoi(elem->GetText());
		}else if(!strcmp(sonNode->Value(),"altura")){
			altura = atoi(elem->GetText());
		}else if(!strcmp(sonNode->Value(),"fundo")){
			elem->QueryIntAttribute("corR",&fundoR);
			elem->QueryIntAttribute("corG",&fundoG);
			elem->QueryIntAttribute("corB",&fundoB);
		}else{
			titulo = elem->GetText();
		}
	}
}

void processaQuadrado(const XMLNode* node){
	const XMLElement* elem = node->ToElement();

	elem->QueryIntAttribute("tamanho",&quad.tamanho);
	elem->QueryIntAttribute("corR",&quad.corR);
	elem->QueryIntAttribute("corG",&quad.corG);
	elem->QueryIntAttribute("corB",&quad.corB);

}
