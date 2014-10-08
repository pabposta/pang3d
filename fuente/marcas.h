#ifndef __MARCAS_H
#define __MARCAS_H

#include "practica02.h"

typedef struct _marca {
	GLfloat posicion[3];
	GLfloat angulo[3];
	GLfloat radio;
	GLfloat anchura;
	GLfloat color[3];
	GLfloat vida;
	struct _marca *siguiente;
} marca;

/*****************************************************************************/

marca *crearMarca (GLfloat *posicion, GLfloat *angulo, GLfloat *color);
void destruirMarca (marca *w);
void moverMarca (marca *w);
void dibujarMarca (marca *w);
marca *insertarMarca (marca **lw, marca *w);
marca *sacarMarca (marca **lw, marca *w);


#endif