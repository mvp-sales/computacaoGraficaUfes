#include <GL/freeglut.h>
#include "Carro.h"
#include <math.h>
#include <time.h>
#include <list>


//NAMESPACES
using std::cout;
using std::vector;
using std::list;

// GLOBAL VARIABLES
int alturaJanela,larguraJanela;
double shootFrequency;
Circle biggerCircle, smallerCircle;
Carro playerCar = Carro();
list<Carro> enemies;
list<Bullet> enemyBulletClub,playerBulletClub;
Rectangle finishLine;

bool keyStatus[256],startGame=false,endGame=false;
bool check1=false,check2=false,check3=false,winGame=false;
// Text variable
static char str[2000];
void * font = GLUT_BITMAP_9_BY_15;


void display();
void init(void);
void keyUp(unsigned char,int,int);
void keyPress(unsigned char,int,int);
void idle();
void mouseMotion(int,int);
void mouseClick(int button,int state,int x,int y);
bool isOutsideScreen(Bullet&);
void PrintScore(GLfloat x, GLfloat y);
void moveEnemy(Carro& enemy,GLdouble timeDiff,GLdouble currentTime);
void verifyCheckpoints();
bool colisaoBetweenEnemies(Carro& enemy);




int main(int argc, char** argv) {
	std::string path = argv[1],
				filename = "config.xml";
	path = path + filename;

	processaConfig(path,biggerCircle,smallerCircle,playerCar,enemies,finishLine,shootFrequency);


	larguraJanela = biggerCircle.radius * 2;
	alturaJanela = biggerCircle.radius * 2;

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(larguraJanela, alturaJanela);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("Trabalho Curto 4");

	init();

	glutMouseFunc(mouseClick);
	glutPassiveMotionFunc(mouseMotion);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyPress);
	glutKeyboardUpFunc(keyUp);
	glutIdleFunc(idle);
	srand (time(NULL));
	glutMainLoop();
	return 0;
}

bool isOutsideScreen(Bullet& b){
	if(b.getCoordX() < biggerCircle.center.coordX-biggerCircle.radius ||
		b.getCoordX() > biggerCircle.center.coordX+biggerCircle.radius){
			return true;
		}else if(b.getCoordY() > biggerCircle.center.coordY+biggerCircle.radius ||
				b.getCoordY() < biggerCircle.center.coordY-biggerCircle.radius){
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
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && startGame){
		playerCar.shoot(playerBulletClub);
	}
}


void idle(){
	if(!endGame && !winGame){
		static GLdouble previousTime = 0,timeToShoot = 0;
	    GLdouble currentTime;
	    GLdouble timeDiference;

		currentTime = glutGet(GLUT_ELAPSED_TIME);
	    timeDiference = currentTime - previousTime; // Elapsed time from the previous frame.
	    previousTime = currentTime; //Update previous time

		if(keyStatus['d'] || keyStatus['D']){

			playerCar.turn(0.05*timeDiference);

		}
		if(keyStatus['a'] || keyStatus['A']){

			playerCar.turn(-0.05*timeDiference);

		}

		if(keyStatus['w'] || keyStatus['W']){
			if(!startGame) startGame = true;
			double currentCarAngle = playerCar.carPartsAngle;
			Point currentPosition = playerCar.referenceCircle.center;
			playerCar.moveAhead(-1,timeDiference);

			if(playerCar.referenceCircle.colisaoInterna(biggerCircle) ||
				playerCar.referenceCircle.colisaoExterna(smallerCircle) ||
				playerCar.colisaoEnemies(enemies)){

					playerCar.referenceCircle.center = currentPosition;
					playerCar.carPartsAngle = currentCarAngle;
			}
		}
		if((keyStatus['s'] || keyStatus['S']) && startGame){
			double currentCarAngle = playerCar.carPartsAngle;
			Point currentPosition = playerCar.referenceCircle.center;

			playerCar.moveAhead(1,timeDiference);

			if(playerCar.referenceCircle.colisaoInterna(biggerCircle) ||
				playerCar.referenceCircle.colisaoExterna(smallerCircle) ||
				playerCar.colisaoEnemies(enemies)){

					playerCar.referenceCircle.center = currentPosition;
					playerCar.carPartsAngle = currentCarAngle;
			}
		}

		if(startGame){
			for(list<Carro>::iterator it = enemies.begin(); it != enemies.end(); ++it){
				moveEnemy(*it,timeDiference,currentTime);
			}
		}


		for(list<Bullet>::iterator it = playerBulletClub.begin(); it != playerBulletClub.end(); ++it){
			it->updatePosition(timeDiference);
			for(list<Carro>::iterator it2 = enemies.begin(); it2 != enemies.end(); ++it2){
				if(it2->referenceCircle.colisaoExterna(it->referenceCircle)){
					it2 = enemies.erase(it2);
				}
			}
		}
		playerBulletClub.remove_if(isOutsideScreen);

		for(list<Bullet>::iterator it = enemyBulletClub.begin(); it != enemyBulletClub.end(); ++it){
			it->updatePosition(timeDiference);
			if(it->referenceCircle.colisaoExterna(playerCar.referenceCircle)){
				endGame = true;
			}
		}
		enemyBulletClub.remove_if(isOutsideScreen);

		if(startGame){
			if(currentTime - timeToShoot > 1.0/shootFrequency){
				for(list<Carro>::iterator it = enemies.begin(); it != enemies.end(); ++it){
					it->shoot(enemyBulletClub);
				}
				timeToShoot = currentTime;
			}
		}else timeToShoot = currentTime;

		verifyCheckpoints();
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

	biggerCircle.draw();
	smallerCircle.draw();
	finishLine.draw();

	for(list<Carro>::iterator it = enemies.begin(); it != enemies.end(); ++it){
		it->draw();
	}
	playerCar.draw();
	for(list<Bullet>::iterator it = playerBulletClub.begin(); it != playerBulletClub.end(); ++it){
		it->draw();
	}
	for(list<Bullet>::iterator it = enemyBulletClub.begin(); it != enemyBulletClub.end(); ++it){
		it->draw();
	}

	PrintScore(biggerCircle.getCenterX() + (1.0/2.0)*biggerCircle.radius,
				biggerCircle.getCenterY() - (9.0/10.0)*biggerCircle.radius);
	if(winGame){
		char *tmpStr;
		sprintf(str,"YOU WIN");
		glRasterPos2f(biggerCircle.getCenterX(),biggerCircle.getCenterY());
		tmpStr = str;
		while( *tmpStr ){
	        glutBitmapCharacter(font, *tmpStr);
	        tmpStr++;
	    }
	}else if(endGame){
			char *tmpStr;
			sprintf(str,"YOU LOSE");
			glRasterPos2f(biggerCircle.getCenterX(),biggerCircle.getCenterY());
			tmpStr = str;
			while( *tmpStr ){
		        glutBitmapCharacter(font, *tmpStr);
		        tmpStr++;
		    }
	}
	//}
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

void PrintScore(GLfloat x, GLfloat y){
	static GLdouble initialTime = glutGet(GLUT_ELAPSED_TIME),
					actualTime;
    char *tmpStr;
	glColor3d(0.0,0.0,0.0);
	if(winGame || endGame){
		sprintf(str, "Tempo: %.4lf s",actualTime/1000.0);
	}else if(startGame){
		actualTime = glutGet(GLUT_ELAPSED_TIME) - initialTime;
		sprintf(str, "Tempo: %.4lf s",actualTime/1000.0);
	}else{
		sprintf(str, "Tempo: 0.0000 s");
		initialTime = glutGet(GLUT_ELAPSED_TIME);
	}
    //Define the position to start printing
    glRasterPos2f(x, y);
    //Print  the first Char with a certain font
    //glutBitmapLength(font,(unsigned char*)str);
    tmpStr = str;
    //Print each of the other Char at time
    while( *tmpStr ){
        glutBitmapCharacter(font, *tmpStr);
        tmpStr++;
    }
}


void moveEnemy(Carro& enemy,GLdouble timeDiff,GLdouble currentTime){

	static GLdouble timeNow2 = currentTime;
	if(currentTime - timeNow2 > enemy.rWheel){
		timeNow2 = currentTime;
		enemy.dWheel *= -1;
		enemy.rWheel = rand() % 2001 + 500;
	}
	enemy.turn(enemy.dWheel*0.05*timeDiff);


	static GLdouble timeNow = currentTime;
	if(currentTime - timeNow > enemy.rMove){
		timeNow = currentTime;
		enemy.dMove *= -1;
		enemy.rMove = rand() % 1000 + 5001;
	}
	double currentCarAngle = enemy.carPartsAngle;
	Point currentPosition = enemy.referenceCircle.center;
	enemy.moveAhead(enemy.dMove,timeDiff);

	if(enemy.referenceCircle.colisaoInterna(biggerCircle) ||
		enemy.referenceCircle.colisaoExterna(smallerCircle) ||
		 enemy.referenceCircle.colisaoExterna(playerCar.referenceCircle) ||
			colisaoBetweenEnemies(enemy)){

			enemy.referenceCircle.center = currentPosition;
			enemy.carPartsAngle = currentCarAngle;
			enemy.dMove *= -1;
	}

	enemy.cannonAngle += enemy.dCannon;
	if(enemy.cannonAngle > 45){
		enemy.cannonAngle = 45;
		enemy.dCannon *= -1;
	}else if(enemy.cannonAngle < -45){
		enemy.cannonAngle = -45;
		enemy.dCannon *= -1;
	}
}

void verifyCheckpoints(){
	if(playerCar.referenceCircle.getCenterX() > biggerCircle.getCenterX() &&
		playerCar.referenceCircle.getCenterY() < biggerCircle.getCenterY() &&
		check1 && check2 && check3){
			winGame = true;
		}
	if(playerCar.referenceCircle.getCenterX() < biggerCircle.getCenterX() &&
		playerCar.referenceCircle.getCenterY() < biggerCircle.getCenterY()){
			check1 = true;
		}
	if(playerCar.referenceCircle.getCenterX() < biggerCircle.getCenterX() &&
		playerCar.referenceCircle.getCenterY() > biggerCircle.getCenterY() &&
		check1){
			check2 = true;
		}
	if(playerCar.referenceCircle.getCenterX() > biggerCircle.getCenterX() &&
		playerCar.referenceCircle.getCenterY() > biggerCircle.getCenterY() &&
		check1 && check2){
			check3 = true;
		}
}

bool colisaoBetweenEnemies(Carro& enemy){
	for(list<Carro>::iterator it = enemies.begin();
							it != enemies.end(); ++it){
		if(it->referenceCircle.getCenterX() == enemy.referenceCircle.getCenterX() &&
			it->referenceCircle.getCenterY() == enemy.referenceCircle.getCenterY()){

			continue;
		}else if(enemy.referenceCircle.colisaoExterna(it->referenceCircle)){
				return true;
			}
	}
	return false;
}

/*int turnWheelProb = rand() % 10;

static GLdouble timeNow2 = currentTime;
if(currentTime - timeNow2 > enemy.rWheel){
	timeNow2 = currentTime;
	enemy.dWheel *= -1;
	enemy.rWheel = rand() % 2001 + 500;
}
if(turnWheelProb < 7){
	enemy.turn(enemy.dWheel*0.05*timeDiff);
}

int moveProb = rand() % 10;

static GLdouble timeNow = currentTime;
if(currentTime - timeNow > enemy.rMove){
	timeNow = currentTime;
	enemy.dMove *= -1;
	enemy.rMove = rand() % 1000 + 5001;
}
if(moveProb < 7){
	double currentCarAngle = enemy.carPartsAngle;
	Point currentPosition = enemy.referenceCircle.center;
	enemy.moveAhead(enemy.dMove,timeDiff);

	if(enemy.referenceCircle.colisaoInterna(biggerCircle) ||
		enemy.referenceCircle.colisaoExterna(smallerCircle) ||
		 enemy.referenceCircle.colisaoExterna(playerCar.referenceCircle) ||
			colisaoBetweenEnemies(enemy)){

			enemy.referenceCircle.center = currentPosition;
			enemy.carPartsAngle = currentCarAngle;
			enemy.dMove *= -1;
	}
}

int turnCannonProb = rand() % 10;

if(turnCannonProb < 8){
	enemy.cannonAngle += enemy.dCannon;
	if(enemy.cannonAngle > 45){
		enemy.cannonAngle = 45;
	}else if(enemy.cannonAngle < -45){
		enemy.cannonAngle = -45;
	}
}*/
