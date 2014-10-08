#ifndef __PARED_H
#define __PARED_H

#include "practica02.h"

typedef struct _pared {
	GLfloat tamanyo;
} pared;

/*****************************************************************************/

pared *crearPared (GLfloat tamanyo);
void destruirPared (pared *p);
void dibujarPared (pared *p);

#endif
