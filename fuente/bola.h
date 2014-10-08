#ifndef __BOLA_H
#define __BOLA_H

#include "practica02.h"

#define FACTORMASA	1.0

typedef struct _bola {
	GLfloat posicion[3];
	GLfloat orientacion[3];
	GLfloat radio;
	GLfloat radioSq;
	GLfloat color[3];
	struct _bola *siguiente;
} bola;

/*****************************************************************************/

bola *crearBola (GLfloat *posicion, GLfloat *orientacion, GLfloat radio, GLfloat *color);
void destruirBola (bola *b);
void moverBola (bola *b);
void dibujarBola (bola *b);
bola *insertarBola (bola **lista, bola *b);
bola *sacarBola (bola **lista, bola *b);


#endif