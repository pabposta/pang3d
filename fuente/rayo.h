#ifndef __RAYO_H
#define __RAYO_H

#include "practica02.h"

typedef struct _rayo {
	GLfloat origen[3];
	GLfloat destino[3];
	GLfloat color[3];
	GLfloat vida;
	struct _rayo *siguiente;
} rayo;

/*****************************************************************************/

rayo *dispararRayo (GLfloat *origen, GLfloat *destino);
void destruirRayo (rayo *r);
void dibujarRayo (rayo *r);
rayo *insertarRayo (rayo **lr, rayo *r);
rayo *sacarRayo (rayo **lr, rayo *r);


#endif