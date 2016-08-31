#ifdef _WIN32
#include <windows.h>
#else
#define APIENTRY
#endif

#include <GL/freeglut.h>
#include <cstdlib>
#include <iostream>
#include "tinyxml2.h"

//NAMESPACES
using tinyxml2::XMLDocument;
using tinyxml2::XMLNode;
using tinyxml2::XMLElement;
using std::cout;

// VARIÁVEIS GLOBAIS
int largura,altura,fundoR,fundoG,fundoB;
int squareSize,squareR,squareG,squareB;
const char* titulo;

void processaJanela(const XMLNode* node);
void processaQuadrado(const XMLNode* node);


void mouseClick(int button,int state,int x,int y){
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){

		float left = squareSize/(2.0*largura),
			right = left+squareSize/(2.0*largura),
			bottom = squareSize/(2.0*altura),
			top = bottom + squareSize/(2.0*altura);

		glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
		  glColor3f(squareR,squareG,squareB); // Red
		  glVertex2f(left, top);    // x, y
		  glVertex2f( right, top);
		  glVertex2f( right,  bottom);
		  glVertex2f(left,bottom);

		  /*glVertex2f(-squareSize/100.0, -squareSize/100.0);    // x, y
		  glVertex2f( squareSize/100.0, -squareSize/100.0);
		  glVertex2f( squareSize/100.0,  squareSize/100.0);
		  glVertex2f(-squareSize/100.0,  squareSize/100.0);*/

		  /*glVertex2i(-squareSize, -squareSize);    // x, y
		  glVertex2i( squareSize, -squareSize);
		  glVertex2i( squareSize,  squareSize);
		  glVertex2i(-squareSize,  squareSize);*/
		  cout<<"coord x:"<<x<<"\ncoord y:"<<y<<"\n";
		glEnd();
		glFlush();
	}
}

void display() {
   glClearColor(fundoR,fundoG,fundoB,0); // Set background color to black and opaque
   glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer
   glFlush();  // Render now
}

void init(void){
	glClearColor(0.0,0.0,0.0,0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0,0.2,0.0,0.2,-1.0,1.0);
}

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
	glutInit(&argc, argv);                 // Initialize GLUT
	glutCreateWindow(titulo); // Create a window with the given title
	//glutInitWindowSize(widthWindow, lengthWindow);   // Set the window's initial width & height

	glutMouseFunc(mouseClick);
	glutDisplayFunc(display); // Register display callback handler for window re-paint

	glutMainLoop();           // Enter the infinitely event-processing loop
	return 0;
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
	elem->QueryIntAttribute("tamanho",&squareSize);
	elem->QueryIntAttribute("corR",&squareR);
	elem->QueryIntAttribute("corG",&squareG);
	elem->QueryIntAttribute("corB",&squareB);
}
