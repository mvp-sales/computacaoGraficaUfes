#include <GL/freeglut.h>
#include <cstdlib>
#include <iostream>
#include "tinyxml2.h"

// NAMESPACES
using tinyxml2::XMLDocument;
using tinyxml2::XMLNode;
using tinyxml2::XMLElement;
using std::cout;

// STRUCT QUE REPRESENTA O QUADRADO
struct Quadrado{
	float centroX,centroY;
	float corR,corG,corB;
	float tamanho;
};

Quadrado quad;

// DISTANCIAS EM X E Y DO MOUSE NO MOMENTO ATUAL EM RELACAO
// AO CLIQUE DO ARRASTE
float distX,distY;

// VARIÁVEIS DA JANELA
float larguraJanela,alturaJanela;
float fundoR,fundoG,fundoB;
std::string titulo;

// FLAG QUE INDICA O ESTADO ATUAL
bool estadoInsercao=true;
// VERIFICA SE O BOTAO ESQUERDO ESTA PRESSIONADO
bool left_button_pressed = false;
// VERIFICA SE O ARRASTE ESTA ACONTECENDO
bool drag = false;

// VERIFICA SE O CLIQUE FOI NA AREA DO QUADRADO
bool isSquareArea(int x,int y);
// DESENHA O QUADRADO
void drawSquare();
// FUNCAO DE CALLBACK DO CLIQUE DO MOUSE
void mouseClick(int button,int state,int x,int y);
// FUNCAO DE CALLBACK DO MOVIMENTO DO MOUSE
void dragAndDrop(int x,int y);
// FUNCAO DO DISPLAY
void display();
void init(void);
// FUNCOES DE LEITURA DO ARQUIVO CONFIG.XML
void processaJanela(const XMLNode* node);
void processaQuadrado(const XMLNode* node);





int main(int argc, char** argv) {
	std::string path = argv[1],filename = "config.xml";
	path = path + filename;

	XMLDocument* doc = new XMLDocument;

	if(doc->LoadFile(path.c_str())){
		printf("Erro na leitura do arquivo\n");
		exit(1);
	}

	const XMLNode* aplicacao = doc->FirstChild();
	const XMLNode* janela = aplicacao->FirstChild();
	const XMLNode* quadrado = aplicacao->LastChild();

	processaJanela(janela);
	processaQuadrado(quadrado);

	delete doc;

	glutInitWindowPosition(0, 0); 
	glutInitWindowSize(larguraJanela, alturaJanela);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);                       
	glutCreateWindow(titulo.c_str()); 

	init();

	glutMotionFunc(dragAndDrop);
	glutMouseFunc(mouseClick);
	glutDisplayFunc(display); 

	glutMainLoop();           
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


	glBegin(GL_QUADS);              
		glColor3f(quad.corR,quad.corG,quad.corB);
		glVertex3f(left, alturaJanela-top,0.0);    
		glVertex3f( right, alturaJanela-top,0.0);
		glVertex3f( right,  alturaJanela-bottom,0.0);
		glVertex3f(left,alturaJanela-bottom,0.0);
	glEnd();
}





void mouseClick(int button,int state,int x,int y){
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && estadoInsercao){

		quad.centroY = y;
		quad.centroX = x;

		drawSquare();
		glFlush();

		estadoInsercao = false;
	}else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN && !estadoInsercao && isSquareArea(x,y)){
		glutPostRedisplay();
		estadoInsercao = true;
	}else if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !estadoInsercao && isSquareArea(x,y)){
		left_button_pressed = true;
		distX = (quad.centroX - x);
		distY = (quad.centroY - y);
	}else{
		left_button_pressed = false;
	}
}





void dragAndDrop(int x,int y){
	if(left_button_pressed){ 
		quad.centroX = x + distX;
		quad.centroY = y + distY;
		drag = true;
		glutPostRedisplay();
	}
}





void display() {
	glClearColor(fundoR,fundoG,fundoB,0); 
	glClear(GL_COLOR_BUFFER_BIT);         
	if(drag){
		drawSquare();
		drag = false;
	}
	glFlush();
}





void init(void){
	glClearColor(0.0,0.0,0.0,0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0,larguraJanela,0.0,alturaJanela,-1.0,1.0);
}





void processaJanela(const XMLNode* node){
	for(const XMLNode* sonNode = node->FirstChild(); sonNode != NULL ; sonNode = sonNode->NextSibling()){
		const XMLElement* elem = sonNode->ToElement();
		if(!strcmp(sonNode->Value(),"largura")){
			larguraJanela = atof(elem->GetText());
		}else if(!strcmp(sonNode->Value(),"altura")){
			alturaJanela = atof(elem->GetText());
		}else if(!strcmp(sonNode->Value(),"fundo")){
			elem->QueryFloatAttribute("corR",&fundoR);
			elem->QueryFloatAttribute("corG",&fundoG);
			elem->QueryFloatAttribute("corB",&fundoB);
		}else{
			titulo = elem->GetText();
		}
	}
}





void processaQuadrado(const XMLNode* node){
	const XMLElement* elem = node->ToElement();

	elem->QueryFloatAttribute("tamanho",&quad.tamanho);
	elem->QueryFloatAttribute("corR",&quad.corR);
	elem->QueryFloatAttribute("corG",&quad.corG);
	elem->QueryFloatAttribute("corB",&quad.corB);

}
