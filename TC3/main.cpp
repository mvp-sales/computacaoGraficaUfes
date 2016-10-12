#include <GL/freeglut.h>
#include "util.h"

// QUANTIDADE DE MOVIMENTO EM X E EM Y
#define movX 1.0
#define movY 1.0

//NAMESPACES
using std::cout;
using std::vector;

// GLOBAL VARIABLES
int alturaJanela,larguraJanela;
Circle biggerCircle, smallerCircle, player;
Carro playerCar;
vector<Circle> enemies;
Rectangle finishLine;

bool keyStatus[256];


void display();
void init(void);
void keyUp(unsigned char,int,int);
void keyPress(unsigned char,int,int);
void idle();






int main(int argc, char** argv) {
	std::string path = argv[1],
				filename = "config.xml";
	path = path + filename;

	processaConfig(path,biggerCircle,smallerCircle,playerCar,enemies,finishLine);

	larguraJanela = biggerCircle.radius * 2;
	alturaJanela = biggerCircle.radius * 2;

	playerCar.wheelAngle = 0;
	playerCar.carPartsAngle = 0;
	playerCar.inMovement = false;

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
	if(keyStatus['d'] || keyStatus['D']){
		if(playerCar.wheelAngle < 45)
			playerCar.turn(1);
		//if(player.colisaoInterna(biggerCircle) || player.colisaoExterna(smallerCircle) || player.colisaoEnemies(enemies)){
		//	player.moveCenterX(-movX);
		//}
	}
	if(keyStatus['a'] || keyStatus['A']){
		if(playerCar.wheelAngle > -45)
			playerCar.turn(-1);
		//if(player.colisaoInterna(biggerCircle) || player.colisaoExterna(smallerCircle) || player.colisaoEnemies(enemies)){
		//	player.moveCenterX(movX);
		//}
	}
	if(keyStatus['w'] || keyStatus['W']){
		/*player.moveCenterY(-movY);
		if(player.colisaoInterna(biggerCircle) || player.colisaoExterna(smallerCircle) || player.colisaoEnemies(enemies)){
			player.moveCenterY(movY);
		}*/
		//playerCar.inMovement = true;
		playerCar.moveAhead(-1);
	}
	if(keyStatus['s'] || keyStatus['S']){
		/*player.moveCenterY(movY);
		if(player.colisaoInterna(biggerCircle) || player.colisaoExterna(smallerCircle) || player.colisaoEnemies(enemies)){
			player.moveCenterY(-movY);
		}*/
			//playerCar.inMovement = true;
			playerCar.moveAhead(1);
	}
	glutPostRedisplay();
}





void keyUp(unsigned char key,int x,int y){
	if(key == 'w' || key == 's'){
		playerCar.inMovement = false;
	}
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
	//player.drawCircle();
	//playerCar.referenceCircle.drawCircle();
	playerCar.draw();
	glutSwapBuffers();
}





void init(void){
	glClearColor(0.0,0.0,0.0,0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(biggerCircle.center.coordX-biggerCircle.radius,biggerCircle.center.coordX+biggerCircle.radius,
			biggerCircle.center.coordY+biggerCircle.radius,biggerCircle.center.coordY-biggerCircle.radius,
			-1.0,1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}



