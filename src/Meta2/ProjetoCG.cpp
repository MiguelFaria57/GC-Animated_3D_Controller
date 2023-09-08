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

#include "RgbImage.h"


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
GLint	   mudarProjecao = 0, ligado = 0, pressionarBotao = 0, auxPressionarBotao = 0, auxCirculo = 0;

// Sistema Coordenadas + objectos
GLint	 wScreen = 1280, hScreen = 720;		// Janela
GLfloat	 xC = 10.0, yC = 10.0, zC = 10.0;	// Mundo

// Observador 
GLfloat  rVisao = 10, aVisao = 0.5 * PI, incVisao = 0.05;
GLfloat  obsP[] = { rVisao * (GLfloat)cos(aVisao), 3.0, rVisao * (GLfloat)sin(aVisao) };
GLfloat  angZoom = 60;

// Texto
char text[50];

//Timer
GLint    msec = 10;


// ---------------------------------------------------------------------- Texturas
RgbImage imag;

// Texturas (5 texturas)
GLuint texture[3];

// Inicializar texturas
void initTextures() {
	// Skybox (Esfera)
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	imag.LoadBmpFile("room.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

	//----------------------------------------- Fundo do Ecra
	glGenTextures(1, &texture[1]);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	imag.LoadBmpFile("basketball.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

	//----------------------------------------- Bola
	glGenTextures(1, &texture[2]);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	imag.LoadBmpFile("ball.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
}

// Skybox (Esfera)
void drawSphere() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	GLUquadricObj* esfera = gluNewQuadric();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glPushMatrix();
		//glTranslatef(2, 4, 2);
		glRotatef(-90, 1, 0, 0);
		gluQuadricDrawStyle(esfera, GLU_FILL);
		gluQuadricNormals(esfera, GLU_SMOOTH);
		gluQuadricTexture(esfera, GL_TRUE);
		gluSphere(esfera, 50.0, 100, 100);
		gluDeleteQuadric(esfera);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}


// ---------------------------------------------------------------------- Materiais

// Inicializar materiais
void initMaterials(int material);

// Mudar especularidade
GLfloat mudarEspec = 0;


// ---------------------------------------------------------------------- Luzes

// Grelha
GLfloat alpha = 0.1;

GLenum options[] ={
	GL_ZERO,
	GL_ONE,
	GL_SRC_COLOR,
	GL_ONE_MINUS_SRC_COLOR,
	GL_DST_COLOR,
	GL_ONE_MINUS_DST_COLOR,
	GL_SRC_ALPHA,
	GL_ONE_MINUS_SRC_ALPHA,
	GL_DST_ALPHA,
	GL_ONE_MINUS_DST_ALPHA
};

GLint src = 6;
GLint dst = 1;

GLint divisoesMalha = 32;

// Desenhar face dividida em malha
void drawMeshSide(GLfloat meshW, GLfloat meshH, GLfloat l1) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPushMatrix();

	glTranslatef(-meshW * 0.5, l1, meshH * 0.5);
	glRotatef(-90, 1, 0, 0);

	glNormal3f(0, 0, 1);

	glBegin(GL_QUADS);
	for (int i = 0; i < divisoesMalha - 1; i++) {
		for (int j = 0; j < divisoesMalha - 1; j++) {
			GLfloat x0 = meshW * i / GLfloat(divisoesMalha - 1);
			GLfloat y0 = meshH * j / GLfloat(divisoesMalha - 1);
			GLfloat u0 = i / GLfloat(divisoesMalha - 1);
			GLfloat v0 = j / GLfloat(divisoesMalha - 1);

			GLfloat x1 = meshW * (i + 1) / GLfloat(divisoesMalha - 1);
			GLfloat y1 = meshH * j / GLfloat(divisoesMalha - 1);
			GLfloat u1 = (i + 1) / GLfloat(divisoesMalha - 1);
			GLfloat v1 = j / GLfloat(divisoesMalha - 1);

			GLfloat x2 = meshW * (i + 1) / GLfloat(divisoesMalha - 1);
			GLfloat y2 = meshH * (j + 1) / GLfloat(divisoesMalha - 1);
			GLfloat u2 = (i + 1) / GLfloat(divisoesMalha - 1);
			GLfloat v2 = (j + 1) / GLfloat(divisoesMalha - 1);

			GLfloat x3 = meshW * i / GLfloat(divisoesMalha - 1);
			GLfloat y3 = meshH * (j + 1) / GLfloat(divisoesMalha - 1);
			GLfloat u3 = i / GLfloat(divisoesMalha - 1);
			GLfloat v3 = (j + 1) / GLfloat(divisoesMalha - 1);

			glVertex3f(x0, y0, 0.);
			glVertex3f(x1, y1, 0.);
			glVertex3f(x2, y2, 0.);
			glVertex3f(x3, y3, 0.);
		}
	}
	glEnd();
	glPopMatrix();
}

// Desenhar malha
void drawMesh(GLfloat meshW, GLfloat meshH, GLfloat l1) {
	glDisable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor4f(1.0, 1.0, 1.0, alpha);
	glPushMatrix();
	glTranslatef(-meshW*0.5, l1+0.01, meshH*0.5);
	glRotatef(-90, 1, 0, 0);
	glNormal3f(0, 0, 1);
	glBegin(GL_QUADS);
	for (int i = 0; i < divisoesMalha - 1; i++)
		for (int j = 0; j < divisoesMalha - 1; j++) {
			GLfloat x0 = meshW * i / float(divisoesMalha - 1);
			GLfloat y0 = meshH * j / float(divisoesMalha - 1);
			GLfloat u0 = i / float(divisoesMalha - 1);
			GLfloat v0 = j / float(divisoesMalha - 1);

			GLfloat x1 = meshW * (i + 1) / float(divisoesMalha - 1);
			GLfloat y1 = meshH * j / float(divisoesMalha - 1);
			GLfloat u1 = (i + 1) / float(divisoesMalha - 1);
			GLfloat v1 = j / float(divisoesMalha - 1);

			GLfloat x2 = meshW * (i + 1) / float(divisoesMalha - 1);
			GLfloat y2 = meshH * (j + 1) / float(divisoesMalha - 1);
			GLfloat u2 = (i + 1) / float(divisoesMalha - 1);
			GLfloat v2 = (j + 1) / float(divisoesMalha - 1);

			GLfloat x3 = meshW * i / float(divisoesMalha - 1);
			GLfloat y3 = meshH * (j + 1) / float(divisoesMalha - 1);
			GLfloat u3 = i / float(divisoesMalha - 1);
			GLfloat v3 = (j + 1) / float(divisoesMalha - 1);

			glColor4f(1.0, 1.0, 1.0, alpha);
			glVertex3f(x0, y0, 0.0);
			glVertex3f(x1, y1, 0.0);
			glVertex3f(x2, y2, 0.0);
			glVertex3f(x3, y3, 0.0);
		}
	glEnd();
	glPopMatrix();
	glEnable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


// Luz - ambiente
GLfloat intensidadeAmbiente = 1.0;
GLfloat corAmbiente[4] = {intensidadeAmbiente, intensidadeAmbiente, intensidadeAmbiente, 1.0};

// Luz - direcional
GLfloat intensidadeT = 0.3;
GLint   luzR = 1.0, luzG = 1.0, luzB = 1.0;
GLfloat posDirecional[4] = {0.0, 0.0, 1.0, 0.0}; // Z
GLfloat corAmbDirecional[4] = {0.0, 0.0, 0.0, 0.0};
GLfloat corDifDirecional[4] = {luzR*intensidadeT, luzG*intensidadeT, luzB*intensidadeT, 1.0};
GLfloat corEspDirecional[4] = {luzR*intensidadeT, luzG*intensidadeT, luzB*intensidadeT, 1.0};

// Luz - foco
GLfloat posFoco[] = {0.0, 5, 0.0, 1.0}; // Y
GLfloat corFoco[] = {1.0, 0.0, 0.0, 1.0};
GLfloat caFoco = 1.0;
GLfloat laFoco = 0.05;
GLfloat qaFoco = 0.0;
GLfloat	cFoco = 50.0; //abertura
GLfloat dFoco[] = {0.0, -1.0, 0.0, 0.0}; // -Y
GLfloat eFoco = 5.0; //distribuicao de intensidade
int mudarCorFoco = 0, apagarFoco = 0;


// Inicializar das luzes
void initLights(void) {
	// Luz - ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, corAmbiente);

	// Luz - direcional
	glLightfv(GL_LIGHT0, GL_POSITION, posDirecional);
	glLightfv(GL_LIGHT0, GL_AMBIENT, corAmbDirecional);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, corDifDirecional);
	glLightfv(GL_LIGHT0, GL_SPECULAR, corEspDirecional);

	// Luz - foco
	glLightfv(GL_LIGHT1, GL_POSITION, posFoco);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, corFoco);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, caFoco);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, laFoco);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, qaFoco);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, cFoco);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dFoco);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, eFoco);
}

// Atualizar luzes
void updateLights() {
	corDifDirecional[0] = luzR * intensidadeT;
	corDifDirecional[1] = luzG * intensidadeT;
	corDifDirecional[2] = luzB * intensidadeT;
	corEspDirecional[0] = luzR * intensidadeT;
	corEspDirecional[1] = luzG * intensidadeT;
	corEspDirecional[2] = luzB * intensidadeT;
	glLightfv(GL_LIGHT0, GL_POSITION, posDirecional);
	glLightfv(GL_LIGHT0, GL_AMBIENT, corAmbDirecional);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, corDifDirecional);
	glLightfv(GL_LIGHT0, GL_SPECULAR, corEspDirecional);

	glLightfv(GL_LIGHT1, GL_POSITION, posFoco);
	glLightfv(GL_LIGHT1, GL_AMBIENT, corFoco);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, corFoco);
	glLightfv(GL_LIGHT1, GL_SPECULAR, corFoco);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, caFoco);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, laFoco);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, qaFoco);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, cFoco);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dFoco);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, eFoco);
}


// ---------------------------------------------------------------------- Funcoes Desenho

// Escrever texto
void writeText(char* string, GLfloat x, GLfloat y, GLfloat z) {
	glRasterPos3f(x, y, z);
	while (*string)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *string++);
}

// Desenhar eixos
void drawEixos() {
	glDisable(GL_LIGHTING);

	glColor4f(RED);
	//initMaterials(15);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(20, 0, 0);
	glEnd();
	glColor4f(GREEN);
	//initMaterials(14);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 20, 0);
	glEnd();
	glColor4f(BLUE);
	//initMaterials(13);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 0, 20);
	glEnd();

	glEnable(GL_LIGHTING);
}

// Desenhar caixa/paralelepipedo
void drawBox(GLfloat c, GLfloat l, GLfloat a, int mode, int textura, int malha) {
	if (mode == 1 || mode == 3)
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
	//glColorPointer(3, GL_FLOAT, 0, cor);     // Color arrays

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY);

	glPushMatrix();
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, esquerda);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, direita);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, frente);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, tras);
	//glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, cima);

	// ----------------------------------------------- Malha
	if (malha == 1) {
		drawMeshSide(c, a, l1);

		glEnable(GL_BLEND);
		glBlendFunc(options[src], options[dst]);
		drawMesh(c, a, l1);
		glDisable(GL_BLEND);
	}
	//------------------------------------------------------
	else {
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, cima);
	}
	
	//------------------------------------------------ Textura
	if (textura >= 0) {
		GLfloat texturas[] = {
			0,0, 1,0, 1,1, 0,1,
			0,0, 1,0, 1,1, 0,1,
			0,0, 1,0, 1,1, 0,1,
			0,0, 1,0, 1,1, 0,1
		};
		glTexCoordPointer(2, GL_FLOAT, 0, texturas);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[textura]);
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, baixo);
		glDisable(GL_TEXTURE_2D);
	}
	//--------------------------------------------------------
	else {
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, baixo);
	}

	glPopMatrix();

	if (mode == 3) {
		//glColor4f(BLACK);
		initMaterials(12);
		drawBox(c, l, a, 2, textura, malha);
	}
}

// Desenhar botao/cilindro
void drawCylinder(GLfloat r, GLfloat a, int mode, int textura) {
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

	if (textura >= 0) {
		/*const int vezes = 8 * 63;
		GLfloat texturas[vezes];
		int aux = 0;
		while (aux < vezes) {
			texturas[aux++] = 0;
			texturas[aux++] = 0;
			texturas[aux++] = 1;
			texturas[aux++] = 0;
			texturas[aux++] = 1;
			texturas[aux++] = 1;
			texturas[aux++] = 0;
			texturas[aux++] = 1;
		}
		glTexCoordPointer(2, GL_FLOAT, 0, texturas);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);*/
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[textura]);
		glBegin(GL_POLYGON);
		angle = 0.0;
		//int pontos = 0;
		while (angle < 2 * PI) {
			//pontos++;
			x = r * cos(angle);
			z = r * sin(angle);
			if (angle <= PI)
				glTexCoord2f(x *4, z *4);
			else 
				glTexCoord2f(x *4, -z *4);
			glVertex3f(x, -a/2, z);
			angle = angle + angle_stepsize;
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
		//printf("%d\n", pontos);
	}
	else {
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

	/*glBegin(GL_POLYGON);
	angle = 0.0;
	while (angle < 2 * PI) {
		x = r * cos(angle);
		z = r * sin(angle);
		glVertex3f(x, -a/2, z);
		angle = angle + angle_stepsize;
	}
	glEnd();*/


	if (mode == 2) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glColor4f(BLACK);
		initMaterials(12);
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

void drawCylinderV2(GLfloat r, GLfloat a, int mode, int textura) {
	if (textura == -1) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		GLUquadricObj* lado = gluNewQuadric();
		GLUquadricObj* base = gluNewQuadric();
		GLUquadricObj* topo = gluNewQuadric();
		glPushMatrix();
		gluCylinder(lado, r, r, a, 100, 100);
		gluDisk(base, 0, r, 100, 100);
		glTranslatef(0, 0, a);
		gluDisk(topo, 0, r, 100, 100);
		gluDeleteQuadric(lado);
		gluDeleteQuadric(base);
		gluDeleteQuadric(topo);
		glPopMatrix();
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		GLUquadricObj* lado = gluNewQuadric();
		GLUquadricObj* base = gluNewQuadric();
		GLUquadricObj* topo = gluNewQuadric();

		glPushMatrix();
		gluCylinder(lado, r, r, a, 100, 100);
		gluDisk(base, 0, r, 100, 100);
		glTranslatef(0, 0, a);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[textura]);
		gluQuadricDrawStyle(topo, GLU_FILL);
		gluQuadricNormals(topo, GLU_SMOOTH);
		gluQuadricTexture(topo, GL_TRUE);
		gluDisk(topo, 0, r, 100, 100);
		gluDeleteQuadric(lado);
		gluDeleteQuadric(base);
		gluDeleteQuadric(topo);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	if (mode == 2) {
		initMaterials(12);
		GLUquadricObj* base = gluNewQuadric();
		GLUquadricObj* topo = gluNewQuadric();
		glPushMatrix();
		gluQuadricDrawStyle(base, GLU_SILHOUETTE);
		gluDisk(base, 0, r, 100, 100);
		glTranslatef(0, 0, a);
		gluQuadricDrawStyle(topo, GLU_SILHOUETTE);
		gluDisk(topo, 0, r, 100, 100);
		gluDeleteQuadric(base);
		gluDeleteQuadric(topo);
		glPopMatrix();
	}
}

// Desenhar cenario
void drawScene() {
	// Base	
	glPushMatrix();
		// Movimentos do Objeto
		glTranslatef(centrox, centroy, centroz);
		glRotatef(rotacao, 0.0, 1.0, 0.0);

		glPushMatrix();
			// Base
			glPushMatrix();
			//glColor4f(GREY);
			initMaterials(2);
			drawBox(8, 0.7, 5, 3, -1, 0);
			glPopMatrix();

			// Botao ligar
			glPushMatrix();
			//glColor4f(LIGHT_GREY);
			initMaterials(11);
			glTranslatef(0.005+4, 0, -(0.17 + 1.8));
			drawBox(0.01, 0.12, 0.54, 1, -1, 0);
				glPushMatrix();
				//glColor4f(DARK_GREY);
				initMaterials(12);
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
				drawBox(0.1, 0.1, 0.2, 3, -1, 0);
				glPopMatrix();
			glPopMatrix();

			// Led
			glPushMatrix();
			if (ligado == 0) {
				//glColor4f(RED);
				initMaterials(4);
			}
			else if (ligado == 1) {
				//glColor4f(GREEN);
				initMaterials(0);
			}
			glTranslatef(3.3, 0.16, 0.075+2.37);
			drawBox(0.05, 0.4, 0.15, 3, -1, 0);
			glPopMatrix();

			// Dobradicas
			/*glPushMatrix();
			//glColor4f(BLUE);
			initMaterials(8);
			glTranslatef(-3, 0.35, -2.5);
			glRotatef(90, 0.0, 0.0, 1.0);
			drawCylinder(0.2, 1, 2, -1);
			glPopMatrix();
			glPushMatrix();
			//glColor4f(BLUE);
			initMaterials(8);
			glTranslatef(3, 0.35, -2.5);
			glRotatef(90, 0.0, 0.0, 1.0);
			drawCylinder(0.2, 1, 2, -1);
			glPopMatrix();*/

			initMaterials(8);
			glPushMatrix();
			glTranslatef(-3, 0.35, -2.5);
			glRotatef(90, 0, 1, 0);
			drawCylinderV2(0.2, 1, 2, -1);
			glPopMatrix();
			
			initMaterials(8);
			glPushMatrix();
			glTranslatef(3, 0.35, -2.5);
			glRotatef(-90, 0, 1, 0);
			drawCylinderV2(0.2, 1, 2, -1);
			glPopMatrix();

			// Tampa
			glPushMatrix();
			//glColor4f(LIGHT_GREY);
			initMaterials(11);
			glTranslatef(0, -0.25 + (0.25 + 0.35), -2.5);
			glRotatef(rotacaoTampa, 1.0, 0.0, 0.0);
			glTranslatef(0, 0.25, 2.5);
			drawBox(8, 0.5, 5, 3, -1, 1);
			// Ecra
				glPushMatrix();
				//glColor4f(DARK_GREY);
				//initMaterials(12);
				/*if (ligado == 0)
					glColor4f(DARK_GREY);
				else if (ligado == 1)
					glColor4f(LIGHT_BLUE);*/
				glTranslatef(0, -(0.005+0.25), 0.25);
				glPushMatrix();
					glRotatef(180, 0, 1, 0);
					if (ligado == 0) {
						initMaterials(12);
						drawBox(7.5, 0.01, 4, 3, -1, 0);
					}
					else if (ligado == 1) {
						//initMaterials(16);
						initMaterials(11);
						drawBox(7.5, 0.01, 4, 3, 1, 0);
					}
				glPopMatrix();
					// Animacao
					if (ligado == 1) {
						glPushMatrix();
						//glColor4f(RED);
						initMaterials(9);

						translacaoCirculo = translacaoCirculo + incrementoC;
						if (translacaoCirculo <= -3.55)
							incrementoC = -incrementoC;
						if (translacaoCirculo >= 3.55)
							incrementoC = -incrementoC;

						if (pressionarBotao == 1)
							auxCirculo = 1;
						
						if (auxCirculo == 1) {
							saltarC = saltarC + incrementoSC;
							if (saltarC >= 1.16)
								incrementoSC = -incrementoSC;
							if (saltarC <= 0) {
								incrementoSC = -incrementoSC;
								auxCirculo = 0;
							}
						}

						//glTranslatef(translacaoCirculo, -(0.005 + 0.005), -0.5 + saltarC);
						//drawCylinder(0.2, 0.01, 1, 2);
						glTranslatef(translacaoCirculo, 0, -0.5 + saltarC);
						glPushMatrix();
						glRotatef(90, 1, 0, 0);
						drawCylinderV2(0.2, 0.01, 2, 2);
						glPopMatrix();

						glPopMatrix();
					}
				glPopMatrix();
			glPopMatrix();

			// Botao
			glPushMatrix();
			//glColor4f(YELLOW);
			initMaterials(10);
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
			/*glTranslatef(-2, 0.35+0.15, 0);
			drawCylinder(0.5, 0.3, 2, -1);*/
			glTranslatef(-2, 0.35, 0);
				glPushMatrix();
				glRotatef(-90, 1, 0, 0);
				drawCylinderV2(0.5, 0.3, 2, -1);
				glPopMatrix();
			glPopMatrix();

			// Controlador
			glPushMatrix();
			//glColor4f(ORANGE);
			initMaterials(9);
			/*glTranslatef(2, 0.35+0.15, 0);
			glRotatef(rotacaoControlador, 0.0, 1.0, 0.0);
			drawCylinder(0.7, 0.3, 2, -1);*/
			glTranslatef(2, 0.35, 0);
			glRotatef(rotacaoControlador, 0.0, 1.0, 0.0);
				glPushMatrix();
				glRotatef(-90, 1, 0, 0);
				drawCylinderV2(0.7, 0.3, 2, -1);
				glPopMatrix();
				// Indicador do Controlador
				glPushMatrix();
				//glColor4f(WHITE);
				initMaterials(3);
				/*initMaterials(16);
				//glTranslatef(0, 0.15+0.025, -0.5);*/
				glTranslatef(0, 0.15+0.15, -0.5);
				drawBox(0.1, 0.05, 0.3, 3, -1, 0);
				glPopMatrix();
			glPopMatrix();
	
		glPopMatrix();

	glPopMatrix();
}

// Desenhar observador (projecao ortogonal)
void drawObserver() {
	//glColor4f(GREEN);
	initMaterials(14);
	glPushMatrix();
	glTranslatef(obsP[0], obsP[1], obsP[2]);
	drawBox(1, 1, 1, 3, -1, 0);
	glPopMatrix();
}

// Escrever comandos
void drawCommands() {
	float numLinhas = 6;
	glColor4f(WHITE);
	//initMaterials(16);
	sprintf_s(text, "Comandos:");
	writeText(text, -8, numLinhas, 1);
	numLinhas -= 1.5;
	sprintf_s(text, "Observador-'Setas'");
	writeText(text, -8, numLinhas, 1);
	numLinhas -= 1;
	sprintf_s(text, "Mover-'w/a/s/d/e/q'");
	writeText(text, -8, numLinhas, 1);
	numLinhas -= 1;
	sprintf_s(text, "Rodar-'r/f'");
	writeText(text, -8, numLinhas, 1);
	numLinhas -= 1;
	sprintf_s(text, "Zoom-'z/x'");
	writeText(text, -8, numLinhas, 1);
	numLinhas -= 1;
	sprintf_s(text, "Alterar Projecao-'b'");
	writeText(text, -8, numLinhas, 1);
	numLinhas -= 1;
	sprintf_s(text, "Abrir/Fechar-'t/g'");
	writeText(text, -8, numLinhas, 1);
	numLinhas -= 1;
	sprintf_s(text, "Ligar-'c'");
	writeText(text, -8, numLinhas, 1);
	numLinhas -= 1;
	sprintf_s(text, "Pressionar Botao-'v'");
	writeText(text, -8, numLinhas, 1);
	numLinhas -= 1;
	sprintf_s(text, "Rodar Controlador-'y/h'");
	writeText(text, -8, numLinhas, 1);
	numLinhas -= 1;
	sprintf_s(text, "Aumentar intensidade Luz Direcional-'i'");
	writeText(text, -8, numLinhas, 1);
	numLinhas -= 1;
	sprintf_s(text, "Mudar Cor do Foco-'o'");
	writeText(text, -8, numLinhas, 1);
	numLinhas -= 1;
	sprintf_s(text, "Acender/Apagar Foco-'p'");
	writeText(text, -8, numLinhas, 1);
	numLinhas -= 1;
	sprintf_s(text, "Colocar/Tirar Malha-'j'");
	writeText(text, -8, numLinhas, 1);
	numLinhas -= 1;
	sprintf_s(text, "Alterar Transparencia da Malha-'k'");
	writeText(text, -8, numLinhas, 1);
	numLinhas -= 1;
	sprintf_s(text, "Alterar Especularidade do Material-'l'");
	writeText(text, -8, numLinhas, 1);
}

// Display
void display(void) {
	// Apaga
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Viewport 0 - Comandos
	glDisable(GL_LIGHTING);
	glViewport(0, 0, wScreen/6, hScreen/3);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-xC, xC, -xC, xC, -zC, zC);
	drawCommands();

	// Viewport 1 - Cenario
	glViewport(0, 0, wScreen, hScreen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	switch (mudarProjecao) {
	case 0:
		gluPerspective(angZoom, (float)wScreen/hScreen, 0.1, 30.0*zC);
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
	if (mudarProjecao == 0)
		drawSphere();
	drawEixos();
	drawScene();
	if (mudarProjecao == 1 || mudarProjecao == 2 || mudarProjecao == 3)
		drawObserver();

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

	// Intensidade da Luz - Direcional
	case 'i': 
	case 'I':
		intensidadeT = intensidadeT + 0.1;
		if (intensidadeT > 1.1) intensidadeT = 0;
		updateLights();
		glutPostRedisplay();
		break;

	// Mudar cor da Luz - Foco
	case 'o':
	case 'O':
		mudarCorFoco++;
		if (mudarCorFoco == 0) {
			corFoco[0] = 1;
			corFoco[1] = 0;
			corFoco[2] = 0;
		}
		else if (mudarCorFoco == 1) {
			corFoco[0] = 0;
			corFoco[1] = 1;
			corFoco[2] = 0;
		}
		else if (mudarCorFoco == 2) {
			corFoco[0] = 0;
			corFoco[1] = 0;
			corFoco[2] = 1;
			mudarCorFoco = -1;
		}
		updateLights();
		glutPostRedisplay();
		break;
	
	// Apagar Luz - Foco
	case 'p':
	case 'P':
		apagarFoco++;
		if (apagarFoco == 0) {
			glEnable(GL_LIGHT1);
		}
		else if (apagarFoco == 1) {
			glDisable(GL_LIGHT1);
			apagarFoco = -1;
		}
		glutPostRedisplay();
		break;

	// Colocar/Remover Malha
	case 'j':
	case 'J':
		divisoesMalha /= 2;
		if (divisoesMalha == 1) divisoesMalha = 32;
		glutPostRedisplay();
		break;

	// Mudar transparencia da grelha
	case 'k':
	case 'K':
		alpha = alpha + 0.1;
		if (alpha > 1.1) alpha = 0;
		glutPostRedisplay();
		break;

	// Mudar especularidade da base
	case 'l':
	case 'L':
		mudarEspec = mudarEspec + 0.05;
		if (mudarEspec > 1.05) mudarEspec = 0;
		glutPostRedisplay();
		break;


	// Escape
	case 27:
		exit(0);
		break;
	}
}

/*void keyboard_up(unsigned char key, int x, int y) {
	switch (key) {
	case 'v':
	case 'V':
		pressionarBotao = 1;
		glutPostRedisplay();
		break;
	}
}*/

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


// ---------------------------------------------------------------------- Inicializacao
void init(void) {
	glClearColor(BLACK);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	initTextures();
	initLights();
}

// ---------------------------------------------------------------------- Main
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(wScreen, hScreen);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Projeto - Miguel Faria");
	//glutCreateWindow("Projeto - Miguel Faria || Comandos: Observador-'Setas' ; Mover-'w/a/s/d/e/q' ; Rodar-'r/f' ; Zoom-'z/x' ; Alterar Projecao-'b' ; Abrir/Fechar-'t/g' ; Ligar-'c' ; Pressionar Botao-'v' ; Rodar Controlador-'y/h'");
	
	init();
	glutSpecialFunc(keyboardNotAscii);
	glutKeyboardFunc(keyboard);
	//glutKeyboardUpFunc(keyboard_up);
	glutDisplayFunc(display);
	glutTimerFunc(msec, Timer, 1);

	glutMainLoop();

	return 0;
}