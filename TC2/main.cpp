#include <GL/freeglut.h>
#include "tinyxml2.h"
#include "util.h"

//NAMESPACES
using tinyxml2::XMLDocument;
using tinyxml2::XMLNode;
using tinyxml2::XMLElement;
using std::cout;
using std::vector;

int alturaJanela,larguraJanela;
Circle biggerCircle, smallerCircle, player;
vector<Circle> enemies;
Rectangle finishLine;

bool keyStatus[256];


void processaConfig(std::string path);
void processaArquivoSVG(const XMLNode* node);
void display();
void init(void);
void keyUp(unsigned char,int,int);
void keyPress(unsigned char,int,int);
void idle();

int main(int argc, char** argv) {
	std::string path = argv[1],
				filename = "config.xml";
	path = path + filename;

	processaConfig(path);

	larguraJanela = biggerCircle.radius * 2;
	alturaJanela = biggerCircle.radius * 2;

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(larguraJanela, alturaJanela);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("Trabalho Curto 2");

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyPress);
	glutKeyboardUpFunc(keyUp);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}

void idle(){
	Point centroAtual = player.center;
	if(keyStatus['w'] || keyStatus['W']){
		player.center.coordY -= 1;
		if(colisaoCircMaior(biggerCircle,player) || colisao(smallerCircle,player) || colisaoEnemies(enemies,player)){
			player.center.coordY += 1;
		}
	}
	if(keyStatus['s'] || keyStatus['S']){
		player.center.coordY += 1;
		if(colisaoCircMaior(biggerCircle,player) || colisao(smallerCircle,player) || colisaoEnemies(enemies,player)){
			player.center.coordY -= 1;
		}
	}
	if(keyStatus['d'] || keyStatus['D']){
		player.center.coordX += 1;
		if(colisaoCircMaior(biggerCircle,player) || colisao(smallerCircle,player) || colisaoEnemies(enemies,player)){
			player.center.coordX -= 1;
		}
	}
	if(keyStatus['a'] || keyStatus['A']){
		player.center.coordX -= 1;
		if(colisaoCircMaior(biggerCircle,player) || colisao(smallerCircle,player) || colisaoEnemies(enemies,player)){
			player.center.coordX += 1;
		}
	}
	/*if(colisaoCircMaior(biggerCircle,player) || colisao(smallerCircle,player) || colisaoEnemies(enemies,player)){
		player.center = centroAtual;
	}*/
	glutPostRedisplay();
}

void keyUp(unsigned char key,int x,int y){
	keyStatus[key] = false;
}

void keyPress(unsigned char key,int x,int y){
	keyStatus[key] = true;
}

void display(){
	glClearColor(1,1,1,0);
	glClear(GL_COLOR_BUFFER_BIT);

	biggerCircle.drawCircle();
	smallerCircle.drawCircle();
	finishLine.drawRectangle();
	for(int i = 0; i < enemies.size(); i++){
		enemies.at(i).drawCircle();
	}
	player.drawCircle();
	glutSwapBuffers();
}

void init(void){
	glClearColor(0.0,0.0,0.0,0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(biggerCircle.center.coordX-biggerCircle.radius,biggerCircle.center.coordX+biggerCircle.radius,
			biggerCircle.center.coordY+biggerCircle.radius,biggerCircle.center.coordY-biggerCircle.radius,
			-1.0,1.0);
}

void processaArquivoSVG(const XMLNode* node){
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
			elem->QueryFloatAttribute("cx",&player.center.coordX);
			elem->QueryFloatAttribute("cy",&player.center.coordY);
			elem->QueryFloatAttribute("r",&player.radius);
			player.fill = elem->Attribute("fill");
		}
	}
}

void processaConfig(std::string path){
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

	processaArquivoSVG(rootSVG);

	delete doc;
	delete docSVG;
}