#include <GL/freeglut.h>
#include "Carro.h"
#include <math.h>
#include <list>

// QUANTIDADE DE MOVIMENTO EM X E EM Y
#define movX 1.0
#define movY 1.0

//NAMESPACES
using std::cout;
using std::vector;
using std::list;

// GLOBAL VARIABLES
int alturaJanela,larguraJanela;
Circle biggerCircle, smallerCircle;
Carro playerCar;
vector<Circle> enemies;
list<Bullet> bulletClub;
Rectangle finishLine;

bool keyStatus[256];


void display();
void init(void);
void keyUp(unsigned char,int,int);
void keyPress(unsigned char,int,int);
void idle();
void mouseMotion(int,int);
void mouseClick(int button,int state,int x,int y);
bool isOutsideScreen(Bullet&);




int main(int argc, char** argv) {
	std::string path = argv[1],
				filename = "config.xml";
	path = path + filename;

	processaConfig(path,biggerCircle,smallerCircle,playerCar,enemies,finishLine);

	larguraJanela = biggerCircle.radius * 2;
	alturaJanela = biggerCircle.radius * 2;

	playerCar.wheelAngle = 0;
	playerCar.carPartsAngle = 0;
	playerCar.cannonAngle = 0;
	playerCar.inMovement = false;

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(larguraJanela, alturaJanela);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("Trabalho Curto 2");

	init();

	glutMouseFunc(mouseClick);
	glutPassiveMotionFunc(mouseMotion);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyPress);
	glutKeyboardUpFunc(keyUp);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}

bool isOutsideScreen(Bullet& b){
	if(b.position.coordX < biggerCircle.center.coordX-biggerCircle.radius ||
		b.position.coordX > biggerCircle.center.coordX+biggerCircle.radius){
			return true;
		}else if(b.position.coordY > biggerCircle.center.coordY+biggerCircle.radius ||
				b.position.coordY < biggerCircle.center.coordY-biggerCircle.radius){
					return true;
				}
	return false;
}

void mouseMotion(int x,int y){
	static double prevPosX = playerCar.referenceCircle.center.coordX;
	if(x - prevPosX < 0){
		if(playerCar.cannonAngle > -45)
			playerCar.cannonAngle -= 1;
	}else if(x - prevPosX > 0){
		if(playerCar.cannonAngle < 45)
			playerCar.cannonAngle += 1;
	}
	prevPosX = x;

}

void mouseClick(int button,int state,int x,int y){
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		Bullet* newBullet = new Bullet(playerCar.bulletSpeed,playerCar.carPartsAngle,
										playerCar.cannonAngle,playerCar.referenceCircle.radius,
										playerCar.referenceCircle.center);
		newBullet->draw();
		bulletClub.push_back(*newBullet);
		delete newBullet;
	}
}


void idle(){
	static GLdouble previousTime = 0;
    GLdouble currentTime;
    GLdouble timeDiference;
	int dx = 0;

	currentTime = glutGet(GLUT_ELAPSED_TIME);
    timeDiference = currentTime - previousTime; // Elapsed time from the previous frame.
    previousTime = currentTime; //Update previous time
	playerCar.inMovement = false;

	if(keyStatus['d'] || keyStatus['D']){

		if(playerCar.wheelAngle < 45)
			playerCar.turn(0.5);
		dx = 1;

	}
	if(keyStatus['a'] || keyStatus['A']){

		if(playerCar.wheelAngle > -45)
			playerCar.turn(-0.5);
		dx = -1;

	}

	if(keyStatus['w'] || keyStatus['W']){
		double currentCarAngle = playerCar.carPartsAngle;
		double currentX = playerCar.referenceCircle.center.coordX,
				currentY = playerCar.referenceCircle.center.coordY;

		playerCar.moveAhead(-1,dx,timeDiference);

		if(playerCar.referenceCircle.colisaoInterna(biggerCircle) ||
			playerCar.referenceCircle.colisaoExterna(smallerCircle) ||
			playerCar.referenceCircle.colisaoEnemies(enemies)){
				playerCar.referenceCircle.center.coordX = currentX;
				playerCar.referenceCircle.center.coordY = currentY;
				playerCar.carPartsAngle = currentCarAngle;
				playerCar.inMovement = false;
		}
	}
	if(keyStatus['s'] || keyStatus['S']){
		double currentCarAngle = playerCar.carPartsAngle;
		double currentX = playerCar.referenceCircle.center.coordX,
				currentY = playerCar.referenceCircle.center.coordY;

		playerCar.moveAhead(1,dx,timeDiference);

		if(playerCar.referenceCircle.colisaoInterna(biggerCircle) ||
			playerCar.referenceCircle.colisaoExterna(smallerCircle) ||
			playerCar.referenceCircle.colisaoEnemies(enemies)){
				playerCar.referenceCircle.center.coordX = currentX;
				playerCar.referenceCircle.center.coordY = currentY;
				playerCar.carPartsAngle = currentCarAngle;
				playerCar.inMovement = false;
		}
	}

	bulletClub.remove_if(isOutsideScreen);
	for(list<Bullet>::iterator it = bulletClub.begin(); it != bulletClub.end(); ++it){
		it->updatePosition(timeDiference);
	}
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
	playerCar.draw();
	for(list<Bullet>::iterator it = bulletClub.begin(); it != bulletClub.end(); ++it){
		it->draw();
	}
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
