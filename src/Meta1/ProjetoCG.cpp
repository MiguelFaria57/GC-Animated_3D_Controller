/* ===================================================================================
	Departamento Eng. Informatica - FCTUC
	Computacao Grafica - 2021/22
	Miguel Faria, 2019216809
	Projeto
======================================================================================= */


// ---------------------------------------------------------------------- Includes
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL\glut.h>


// ---------------------------------------------------------------------- Defines
// Cores
#define BLUE       0.0, 0.0, 1.0, 1.0
#define RED		   1.0, 0.0, 0.0, 1.0
#define GREEN      0.0, 1.0, 0.0, 1.0
#define WHITE      1.0, 1.0, 1.0, 1.0
#define BLACK      0.0, 0.0, 0.0, 1.0
#define GREY       0.1, 0.1, 0.1, 1.0
#define YELLOW     1.0, 1.0, 0.0, 1.0
#define ORANGE     1.0, 0.2, 0.0, 1.0
#define LIGHT_GREY 0.3, 0.3, 0.3, 1.0
#define DARK_GREY  0.05, 0.05, 0.05, 1.0
#define LIGHT_BLUE 0.70, 0.80, 0.95, 1.0

// Pi
#define PI		 3.14159


// ---------------------------------------------------------------------- Variaveis e constantes
// Variaveis
GLfloat    centrox = 0, centroy = 0, centroz = 0;
GLfloat    rotacao = 0, rotacaoTampa = 0, rotacaoControlador = 0, translacaoBotaoLigar = 0, translacaoBotao = 0, incrementoT = 0.04, translacaoCirculo = 0, incrementoC = 0.07, saltarC = 0, incrementoSC = 0.08;
GLint mudarProjecao = 0, ligado = 0, pressionarBotao = 0, auxPressionarBotao = 0, auxCirculo = 0;

// Sistema Coordenadas + objectos
GLint	 wScreen = 1280, hScreen = 720;		// Janela
GLfloat	 xC = 10.0, yC = 10.0, zC = 10.0;	// Mundo

// Observador 
GLfloat  rVisao = 10, aVisao = 0.5 * PI, incVisao = 0.05;
GLfloat  obsP[] = { rVisao * (GLfloat)cos(aVisao), 3.0, rVisao * (GLfloat)sin(aVisao) };
GLfloat  angZoom = 60;

//Timer
GLint    msec = 10;


// ---------------------------------------------------------------------- Funcoes

// Desenhar eixos
void drawEixos() {
	glColor4f(RED);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(20, 0, 0);
	glEnd();
	glColor4f(GREEN);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 20, 0);
	glEnd();
	glColor4f(BLUE);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 0, 20);
	glEnd();
}

// Desenhar caixa/paralelepipedo
void drawBox(GLfloat c, GLfloat l, GLfloat a, int mode) {
	if (mode==1 || mode==3)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	GLfloat c1 = c/2;
	GLfloat l1 = l/2;
	GLfloat a1 = a/2;

	GLfloat vertices[] = {
		// -------------------- Esquerda
		-c1,  -l1,  a1,	// 0 
		-c1,   l1,  a1,	// 1 
		-c1,   l1, -a1,	// 2 
		-c1,  -l1, -a1,	// 3 
		// -------------------- Direita
		 c1,  -l1,  a1,	// 4 
		 c1,   l1,  a1,	// 5 
		 c1,   l1, -a1,	// 6 
		 c1,  -l1, -a1,	// 7 
		// -------------------- Cima
		 c1,  l1,  a1,	// 8 
		-c1,  l1,  a1,	// 9 
		-c1,  l1, -a1,	// 10 
		 c1,  l1, -a1,	// 11
		// -------------------- Baixo
		 c1, -l1,  a1,	// 12 
		-c1, -l1,  a1,	// 13 
		-c1, -l1, -a1,	// 14 
		 c1, -l1, -a1,  // 15
		// -------------------- Frente
		-c1, -l1,  a1,	// 16 
		-c1,  l1,  a1,	// 17
		 c1,  l1,  a1,	// 18 
		 c1, -l1,  a1,	// 19
		// -------------------- Tras
		-c1, -l1, -a1,	// 20
		-c1,  l1, -a1,	// 21
		 c1,  l1, -a1,	// 22 
		 c1, -l1, -a1	// 23
	};

	GLfloat normais[] = {
		// -------------------- x=tam (Esquerda)
		-1.0,  0.0,  0.0,
		-1.0,  0.0,  0.0,
		-1.0,  0.0,  0.0,
		-1.0,  0.0,  0.0,
		// -------------------- x=tam (Direita)
		1.0,  0.0,  0.0,
		1.0,  0.0,  0.0,
		1.0,  0.0,  0.0,
		1.0,  0.0,  0.0,
		// -------------------- y=tam (Cima)
		0.0,  1.0,  0.0,
		0.0,  1.0,  0.0,
		0.0,  1.0,  0.0,
		0.0,  1.0,  0.0,
		// -------------------- y=-tam (Baixo)
		0.0,  -1.0,  0.0,
		0.0,  -1.0,  0.0,
		0.0,  -1.0,  0.0,
		0.0,  -1.0,  0.0,
		// -------------------- z=tam (Frente)
		0.0,  0.0,  1.0,
		0.0,  0.0,  1.0,
		0.0,  0.0,  1.0,
		0.0,  0.0,  1.0,
		// -------------------- z=-tam (Tras)
		0.0,  0.0,  -1.0,
		0.0,  0.0,  -1.0,
		0.0,  0.0,  -1.0,
		0.0,  0.0,  -1.0
	};

	/*GLfloat cor[] = {
		// -------------------- x=-tam (Esquerda) - Blue
		0.0, 0.0, 1.0,	// 0 
		0.0, 0.0, 1.0,	// 1 
		0.0, 0.0, 1.0,	// 2 
		0.0, 0.0, 1.0,	// 3 
		// -------------------- x=tam (Direita) - Green
		0.0, 1.0, 0.0,	// 4 
		0.0, 1.0, 0.0,	// 5 
		0.0, 1.0, 0.0,	// 6 
		0.0, 1.0, 0.0,	// 7 
		// -------------------- y=tam (Cima) - Red
		1.0, 0.0, 0.0,	// 8 
		1.0, 0.0, 0.0,	// 9 
		1.0, 0.0, 0.0,	// 10 
		1.0, 0.0, 0.0,	// 11
		// -------------------- y=-tam (Baixo) - Pink
		1.0, 0.0, 1.0,	// 12 
		1.0, 0.0, 1.0,	// 13
		1.0, 0.0, 1.0,	// 14 
		1.0, 0.0, 1.0,	// 15 
		// -------------------- y=tam (Cima) - Yellow
		1.0, 1.0, 0.0,	// 16 
		1.0, 1.0, 0.0,	// 17 
		1.0, 1.0, 0.0,	// 18 
		1.0, 1.0, 0.0,	// 19
		// -------------------- y=-tam (Baixo) - Orange
		1.0, 0.5, 0.1,	// 20 
		1.0, 0.5, 0.1,	// 21 
		1.0, 0.5, 0.1,	// 22 
		1.0, 0.5, 0.1,	// 23 
	};*/

	GLuint esquerda[] = { 0,  1,  2,  3 };
	GLuint  direita[] = { 4,  7,  6,  5 };
	GLuint     cima[] = { 8, 11, 10,  9 };
	GLuint    baixo[] = { 12, 13, 14, 15 };
	GLuint   frente[] = { 16, 19, 18, 17 };
	GLuint     tras[] = { 20, 21, 22, 23 };

	glVertexPointer(3, GL_FLOAT, 0, vertices); // Vertex arrays
	glNormalPointer(GL_FLOAT, 0, normais);	   // Normal arrays
	//glColorPointer(3, GL_FLOAT, 0, cor);       // Color arrays

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY);

	glPushMatrix();
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, esquerda);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, direita);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, cima);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, baixo);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, frente);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, tras);
	glPopMatrix();

	if (mode == 3) {
		glColor4f(BLACK);
		drawBox(c, l, a, 2);
	}
}


void drawCylinder(GLfloat r, GLfloat a, int mode) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	GLfloat x = 0.0;
	GLfloat z = 0.0;
	GLfloat angle = 0.0;
	GLfloat angle_stepsize = 0.1;

	glBegin(GL_QUAD_STRIP);
	angle = 0.0;
	while (angle < 2 * PI) {
		x = r * cos(angle);
		z = r * sin(angle);
		glVertex3f(x, a/2, z);
		glVertex3f(x, -a/2, z);
		angle = angle + angle_stepsize;
	}
	glVertex3f(r, a/2, 0.0);
	glVertex3f(r, -a/2, 0.0);
	glEnd();	

	glBegin(GL_POLYGON);
	angle = 0.0;
	while (angle < 2 * PI) {
		x = r * cos(angle);
		z = r * sin(angle);
		glVertex3f(x, a/2, z);
		angle = angle + angle_stepsize;
	}
	glEnd();
	glBegin(GL_POLYGON);
	angle = 0.0;
	while (angle < 2 * PI) {
		x = r * cos(angle);
		z = r * sin(angle);
		glVertex3f(x, -a/2, z);
		angle = angle + angle_stepsize;
	}
	glEnd();


	if (mode == 2) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor4f(BLACK);
		glBegin(GL_POLYGON);
		angle = 0.0;
		while (angle < 2 * PI) {
			x = r * cos(angle);
			z = r * sin(angle);
			glVertex3f(x, a/2, z);
			angle = angle + angle_stepsize;
		}
		glEnd();
		glBegin(GL_POLYGON);
		angle = 0.0;
		while (angle < 2 * PI) {
			x = r * cos(angle);
			z = r * sin(angle);
			glVertex3f(x, -a/2, z);
			angle = angle + angle_stepsize;
		}
		glEnd();
	}
}

// Desenhar Tudo
void drawScene() {
	// Base	
	glPushMatrix();
		// Movimentos do Objeto
		glTranslatef(centrox, centroy, centroz);
		glRotatef(rotacao, 0.0, 1.0, 0.0);

		glPushMatrix();
			// Base
			glPushMatrix();
			glColor4f(GREY);
			drawBox(8, 0.7, 5, 3);
			glPopMatrix();

			// Botao ligar
			glPushMatrix();
			glColor4f(LIGHT_GREY);
			glTranslatef(0.005+4, 0, -(0.17 + 1.8));
			drawBox(0.01, 0.12, 0.54, 1);
				glPushMatrix();
				glColor4f(DARK_GREY);
				if (ligado == 1) {
					if (translacaoBotaoLigar < 0.12)
						translacaoBotaoLigar = translacaoBotaoLigar + incrementoT;
					glTranslatef(0, 0, translacaoBotaoLigar);
				}
				if (ligado == 0) {
					if (translacaoBotaoLigar > -0.12)
						translacaoBotaoLigar = translacaoBotaoLigar - incrementoT;
					glTranslatef(0, 0, translacaoBotaoLigar);
				}
				glTranslatef(0, 0, 0.005);
				drawBox(0.1, 0.1, 0.2, 3);
				glPopMatrix();
			glPopMatrix();

			// Led
			glPushMatrix();
			if (ligado == 0)
				glColor4f(RED);
			else if (ligado == 1)
				glColor4f(GREEN);
			glTranslatef(3.3, 0.16, 0.075+2.37);
			drawBox(0.05, 0.4, 0.15, 3);
			glPopMatrix();

			// Dobradicas
			glPushMatrix();
			glColor4f(BLUE);
			glTranslatef(-3, 0.35, -2.5);
			glRotatef(90, 0.0, 0.0, 1.0);
			drawCylinder(0.2, 1, 2);
			glPopMatrix();
			glPushMatrix();
			glColor4f(BLUE);
			glTranslatef(3, 0.35, -2.5);
			glRotatef(90, 0.0, 0.0, 1.0);
			drawCylinder(0.2, 1, 2);
			glPopMatrix();

			// Tampa
			glPushMatrix();
			glColor4f(LIGHT_GREY);
			glTranslatef(0, -0.25 + (0.25 + 0.35), -2.5);
			glRotatef(rotacaoTampa, 1.0, 0.0, 0.0);
			glTranslatef(0, 0.25, 2.5);
			drawBox(8, 0.5, 5, 3);
			// Ecra
				glPushMatrix();
				if (ligado == 0)
					glColor4f(DARK_GREY);
				else if (ligado == 1)
					glColor4f(LIGHT_BLUE);
				glTranslatef(0, -(0.005+0.25), 0.25);
				drawBox(7.5, 0.01, 4, 3);
					// Animacao
					if (ligado == 1) {
						glPushMatrix();
						glColor4f(RED);

						translacaoCirculo = translacaoCirculo + incrementoC;
						if (translacaoCirculo <= -3.55)
							incrementoC = -incrementoC;
						if (translacaoCirculo >= 3.55)
							incrementoC = -incrementoC;

						if (pressionarBotao == 1)
							auxCirculo = 1;
						
						if (auxCirculo == 1) {
							saltarC = saltarC + incrementoSC;
							if (saltarC >= 0.64)
								incrementoSC = -incrementoSC;
							if (saltarC <= 0) {
								incrementoSC = -incrementoSC;
								auxCirculo = 0;
							}
						}

						glTranslatef(translacaoCirculo, -(0.005 + 0.005), -0.5 + saltarC);
						drawCylinder(0.2, 0.01, 1);
						glPopMatrix();
					}
				glPopMatrix();
			glPopMatrix();

			// Botao
			glPushMatrix();
			glColor4f(YELLOW);
			if (pressionarBotao == 1) {
				translacaoBotao = translacaoBotao - incrementoT;
				if (translacaoBotao <= -0.2) 
					incrementoT = -incrementoT;
				if (translacaoBotao >= 0) {
					incrementoT = -incrementoT;
					pressionarBotao = 0;
				}
				glTranslatef(0, translacaoBotao, 0);
			}
			glTranslatef(-2, 0.35+0.15, 0);
			drawCylinder(0.5, 0.3, 2);
			glPopMatrix();

			// Controlador
			glPushMatrix();
			glColor4f(ORANGE);
			glTranslatef(2, 0.35+0.15, 0);
			glRotatef(rotacaoControlador, 0.0, 1.0, 0.0);
			drawCylinder(0.7, 0.3, 2);
				// Indicador do Controlador
				glPushMatrix();
				glColor4f(WHITE);
				glTranslatef(0, 0.15+0.025, -0.5);
				drawBox(0.1, 0.05, 0.3, 3);
				glPopMatrix();
			glPopMatrix();
	
		glPopMatrix();

	glPopMatrix();
}

// Desenhar observador (projecao ortogonal)
void drawObserver() {
	glColor4f(GREEN);
	glPushMatrix();
	glTranslatef(obsP[0], obsP[1], obsP[2]);
	drawBox(1, 1, 1, 3);
	glPopMatrix();
}

// Display
void display(void) {
	// Apaga 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, wScreen, hScreen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	switch (mudarProjecao) {
	case 0:
		gluPerspective(angZoom, (float)wScreen / hScreen, 0.1, 3.0 * zC);
		gluLookAt(obsP[0], obsP[1], obsP[2], 0, 0, 0, 0, 1, 0);
		break;
	case 1:
		glOrtho(-20, 20, -20, 20, -50, 50);
		gluLookAt(0, 0, 0, -90, 0, 0, 0, 1, 0);
		break;
	case 2:
		glOrtho(-20, 20, -20, 20, -50, 50);
		gluLookAt(0, 0, 0, 0, -90, 0, 0, 0, -1);
		break;
	case 3:
		glOrtho(-20, 20, -20, 20, -50, 50);
		gluLookAt(0, 0, 0, 0, 0, -90, 0, 1, 0);
		break;
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Objectos
	drawEixos();
	if (mudarProjecao == 1 || mudarProjecao == 2 || mudarProjecao == 3)
		drawObserver();
	drawScene();

	// Actualizacao
	glutSwapBuffers();
}

// Timer
void Timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(msec, Timer, 1);
}

// ---------------------------------------------------------------------- Eventos
// Letras
void keyboard(unsigned char key, int x, int y) {
	// Mudar posicao do objeto
	switch (key) {
	case 'w':
	case 'W':
		centroz = centroz + 0.25;
		glutPostRedisplay();
		break;
	case 's':
	case 'S':
		centroz = centroz - 0.25;
		glutPostRedisplay();
		break;
	case 'd':
	case 'D':
		centrox = centrox + 0.25;
		glutPostRedisplay();
		break;
	case 'A':
	case 'a':
		centrox = centrox - 0.25;
		glutPostRedisplay();
		break;
	case 'e':
	case 'E':
		centroy = centroy + 0.25;
		glutPostRedisplay();
		break;
	case 'q':
	case 'Q':
		centroy = centroy - 0.25;
		glutPostRedisplay();
		break;

	// Mudar posicao do observador
	case 'r':
	case 'R':
		rotacao = rotacao + 2.5;
		glutPostRedisplay();
		break;
	case 'f':
	case 'F':
		rotacao = rotacao - 2.5;
		glutPostRedisplay();
		break;

	case 'x':
	case 'X':
		rVisao = rVisao++;
		obsP[0] = rVisao * cos(aVisao);
		obsP[2] = rVisao * sin(aVisao);
		glutPostRedisplay();
		break;
	case 'z':
	case 'Z':
		rVisao = rVisao--;
		obsP[0] = rVisao * cos(aVisao);
		obsP[2] = rVisao * sin(aVisao);
		glutPostRedisplay();
		break;

	// Mudar projecao
	case 'b':
	case 'B':
		mudarProjecao = (mudarProjecao + 1) % 4;
		glutPostRedisplay();
		break;

	// Abrir/Fechar Tampa
	case 't':
	case 'T':
		if (rotacaoTampa > -110)
			rotacaoTampa = rotacaoTampa - 2.5;
		glutPostRedisplay();
		break;
	case 'g':
	case 'G':
		if (rotacaoTampa < 0)
			rotacaoTampa = rotacaoTampa + 2.5;
		glutPostRedisplay();
		break;

	// Carregar Botao Ligar
	case 'c':
	case 'C':
		translacaoBotaoLigar = 0;
		ligado = !ligado;
		glutPostRedisplay();
		break;

	// Carregar Botao
	case 'v':
	case 'V':
		pressionarBotao = 1;
		glutPostRedisplay();
		break;

	// Rodar Controlador para Direita/Esquerda
	case 'y':
	case 'Y':
		if (rotacaoControlador < 150) {
			rotacaoControlador = rotacaoControlador + 5;
			if (incrementoC < 0)
				incrementoC = incrementoC + 0.002;
			else
				incrementoC = incrementoC - 0.002;
		}
		glutPostRedisplay();
		break;
	case 'h':
	case 'H':
		if (rotacaoControlador > -150) {
			rotacaoControlador = rotacaoControlador - 5;
			if (incrementoC < 0)
				incrementoC = incrementoC - 0.002;
			else
				incrementoC = incrementoC + 0.002;
		}
		glutPostRedisplay();
		break;

	// Escape
	case 27:
		exit(0);
		break;
	}
}

// Setas
void keyboardNotAscii(int key, int x, int y) {
	if (key == GLUT_KEY_UP)
		obsP[1] = (obsP[1] + 0.1);
	if (key == GLUT_KEY_DOWN)
		obsP[1] = (obsP[1] - 0.1);

	if (obsP[1] > yC)
		obsP[1] = yC;
	if (obsP[1] < -yC)
		obsP[1] = -yC;

	if (key == GLUT_KEY_LEFT)
		aVisao = (aVisao + 0.1);
	if (key == GLUT_KEY_RIGHT)
		aVisao = (aVisao - 0.1);

	obsP[0] = rVisao * cos(aVisao);
	obsP[2] = rVisao * sin(aVisao);

	glutPostRedisplay();
}



// ---------------------------------------------------------------------- Main
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(wScreen, hScreen);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Projeto - Miguel Faria || Comandos: Observador-'Setas' ; Mover-'w/a/s/d/e/q' ; Rodar-'r/f' ; Zoom-'z/x' ; Alterar Projecao-'b' ; Abrir/Fechar-'t/g' ; Ligar-'c' ; Pressionar Botao-'v' ; Rodar Controlador-'y/h'");

	glClearColor(BLACK);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	glutSpecialFunc(keyboardNotAscii);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutTimerFunc(msec, Timer, 1);

	glutMainLoop();

	return 0;
}