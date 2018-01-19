#include <GL/glut.h>
#include <stdio.h>
#include"model.h"
#include<string>
#include <math.h>
using namespace std;

int girax = 0;
int giray = 0;

float homerx = 0.5;
float homery = -0.15;
float homerz = 0;

float lastx = 0.0;
float lasty = 0.0;

int perspectiva = 0;
float zoom_perspectiva = 60;

//valores para la esfera
float dist = 0.0;
float radi_esfera = 0.0;

int ant_x = 0;
int ant_y = 0;

//booleans
int rotar = 0;
int puc_rotar_x = 0;
int puc_rotar_y = 0;
int homer_o_escena = 0;


void pintar_model(){
	
	Model mod;
	mod.load("homer.obj");

	//Calculem capsa mínima contenidora
	double xmax,xmin,ymax,ymin,zmax,zmin = 0;

	//calculem punts màxims i mínims dels vèrtexs
	for(int i = 0; i < mod.faces().size(); i++){
		
		if(mod.vertices()[mod.faces()[i].v[0]] > xmax) xmax = mod.vertices()[mod.faces()[i].v[0]];
		if(mod.vertices()[mod.faces()[i].v[0]] < xmin) xmin = mod.vertices()[mod.faces()[i].v[0]];

		if(mod.vertices()[mod.faces()[i].v[1]] > ymax) ymax = mod.vertices()[mod.faces()[i].v[1]];
		if(mod.vertices()[mod.faces()[i].v[1]] < ymin) ymin = mod.vertices()[mod.faces()[i].v[1]];

		if(mod.vertices()[mod.faces()[i].v[2]] > zmax) zmax = mod.vertices()[mod.faces()[i].v[2]];
		if(mod.vertices()[mod.faces()[i].v[2]] < zmin) zmin = mod.vertices()[mod.faces()[i].v[2]];

	}

	glPushMatrix();

	glTranslated(homerx, homery, homerz);
	//escalem la figura a 0.5 i posarem el centre de la figura a l'origen de coordenades. 
	//despres la mourem a sobre del terra, el qual està a -0.4. Com hem escalat a 0.5 i al punt 0,0,0
	//llavors el punt y minim es -0.25. Per tant, de -0.4 a -0.25 es -0.15. Per posar-la a la cantonada
	//movem l'eix x a 0.5
	glScaled(0.5,0.5,0.5);

	float dx = xmax - xmin;
	float dy = ymax - ymin;
	float dz = zmax - zmin;
	float escalat = std::max(std::max(dx, dy), dz);
	glScaled(1.0/escalat, 1.0/escalat, 1.0/escalat);

	dx = (xmin+(dx/2.0))*-1.0;
	dy = (ymin+(dy/2.0))*-1.0;
	dz = (zmin+(dz/2.0))*-1.0;
	glTranslatef(dx, dy, dz);	

	for(int i = 0; i<mod.faces().size();i++){
		glColor4fv(Materials[mod.faces()[i].mat].diffuse);
		glBegin(GL_TRIANGLES);
			glVertex3dv(&mod.vertices()[mod.faces()[i].v[0]]);
			glVertex3dv(&mod.vertices()[mod.faces()[i].v[1]]);
			glVertex3dv(&mod.vertices()[mod.faces()[i].v[2]]);
		glEnd();
	}
	glPopMatrix();

}


void pintar_terra(){

	glPushMatrix();

	glTranslatef(0.0,-0.4,0.0);	
	glColor3f(0.3f, 0.3f, 0.3f);
	glBegin(GL_QUADS);
		glVertex3f(0.75,0,0.75);
		glVertex3f(0.75,0,-0.75);
		glVertex3f(-0.75,0,-0.75);
		glVertex3f(-0.75,0,0.75);
	glEnd();

	glPopMatrix();

}


void pintar_ninot(){
	
	glPushMatrix();
	//com que el pinta a l'origen de coordenades (0,0,0), i el radi es 0,4 la figura compren de 0,4 a -0,4
		glColor3f(1,1,1);
		glTranslatef(0,0,0);
		glutSolidSphere(0.4,60,60);
	glPopMatrix();
	glPushMatrix();
		glColor3f(1,1,0);
		glTranslatef(0,0.6,0);
		glutSolidSphere(0.2,60,60);
	glPopMatrix();
	glPushMatrix();
	//he canviat les mesures del nas perque quedaven molt estranyes
		glColor3f(1,0,0);
		glTranslatef(0.0,0.6,0.075);
		glutSolidCone(0.15,0.2,60,60);
	glPopMatrix();

}

void onMotion(int x, int y){

	if(homer_o_escena == 0){
		if(puc_rotar_x == 1){
			 if(x < girax) girax -= 1.5;
			 else girax += 1.5;
		}
		
		if(puc_rotar_y == 1){
			 if(y < giray) giray -= 1.5;
			 else giray += 1.5;
		}
	}
	else if (homer_o_escena == 1){
		if (x < lastx) homerx -= 0.02;
		else if (x >= lastx) homerx += 0.02;
		if (y < lasty) homerz -= 0.02;
		else if (y >= lasty) homerz += 0.02;

	}	
	lastx = x;
	lasty = y;
	glutPostRedisplay();
}

void reset(){}

void keyboard(unsigned char c, int x, int y){

	if(c == 'r'){
		if (rotar == 0) rotar = 1;
		else if (rotar == 1) rotar = 0;
		puc_rotar_x = 0;
		puc_rotar_y = 0;	
	}

	else if(c == 'x') puc_rotar_x = 1;
	
	else if(c == 'y') puc_rotar_y = 1;
	
	else if(c == 'c'){
		if(homer_o_escena == 0) homer_o_escena = 1;
		else if(homer_o_escena == 1) homer_o_escena = 0;
		printf("S'ha canviat el mode de moure a: %d \n",homer_o_escena);	
	}
	else if(c == 'h'){
		printf("Ajuda\n");
		printf("Si es vol rotar l'eix x, prem el botó x.\n");
		printf("Si es vol rotar l'eix y, prem el botó y.\n");
		printf("Si es premen els botons x i y es podrà moure tota la figura.\n");
		printf("Si es vol que no es mogui, premeu r i es reiniciarà.\n");
		printf("Si es vol moure el Homer, premeu el botó c.\n");
	}
	else if(c == 'r') reset();
	else if(c == 'p'){
		if (perspectiva == 0) perspectiva = 1;
		else perspectiva = 0;
		glutPostRedisplay();
	}
	else if(c == 27){	
		exit(0);
	}

}


void camera(){
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float aspect_radio = (float)ant_x/(float)ant_y;
	if (perspectiva == 1){
		gluPerspective(45,aspect_radio,1,10);
	}
	else{
	//Redimensionament respecte l'aspect radio
		if (aspect_radio >= 1.0){
			glOrtho(-radi_esfera*aspect_radio,radi_esfera*aspect_radio, -radi_esfera, radi_esfera, 1,10);
		}
		else{
			glOrtho(-radi_esfera, radi_esfera, -radi_esfera/aspect_radio, radi_esfera/aspect_radio, 1, 10);
		}
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void reshape(int x, int y) {

	ant_x = x;
	ant_y = y;
	if (y == 0){
		y = 1;	
	}
	glViewport(0,0,x,y);
	camera();
	
}


void calcular_esfera(){

	float x_max = 1;
	float x_min = -1;
	float y_max = 1;
	float y_min = -1;
	float z_max = 1;
	float z_min = -1;

	radi_esfera = sqrt(pow((x_max-x_min),2) + pow((y_max-y_min),2) + pow((z_max-z_min),2));
	radi_esfera /= 2;

	printf("Radi esfera %f\n",radi_esfera);
	
}


void display(void) {

	glClearColor(0.4,0.4,1.0,1.0); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	camera();

	//Movem el punt observador
	glTranslatef(0,0,-radi_esfera*2);
	glRotated(girax,1,0,0);
	glRotated(giray,0,1,0);
	glTranslatef(0,0,0);

	pintar_terra();
	pintar_ninot();
	pintar_model();

	glutSwapBuffers();
}


void initGL(int argc, char **argv){

	glutInit(&argc, argv);
	glutInitWindowSize(600,600);
	glutCreateWindow("INDI practiques OpenGL");
	glEnable(GL_DEPTH_TEST);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMotionFunc(onMotion);
	glutKeyboardFunc(keyboard);

	calcular_esfera();

	glutMainLoop();

}

int main(int argc, char **argv) {
	
	initGL(argc, argv);
	return 0;
}
