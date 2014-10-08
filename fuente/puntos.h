#ifndef __PUNTOS_H
#define __PUNTOS_H

#include "practica02.h"

typedef struct _puntos {
	GLfloat posicion[3];
	GLfloat orientacion[3];
	GLfloat angulo;
	GLfloat velocidad;
	GLint puntuacion;
	GLfloat vida;
	GLfloat color[3];
	struct _puntos *siguiente;
} puntos;

/*****************************************************************************/

puntos *crearPuntos (GLfloat *posicion, GLfloat *orientacion, GLint puntuacion);
void destruirPuntos (puntos *s);
void moverPuntos (puntos *s);
void dibujarPuntos (puntos *s);
puntos *insertarPuntos (puntos **ls, puntos *s);
puntos *sacarPuntos (puntos **ls, puntos *s);


#endif