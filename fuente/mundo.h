#ifndef __MUNDO_H
#define __MUNDO_H

#include "practica02.h"

typedef struct _mundo {
	bola *lista;
	puntos *ls;
	camara *c;
	jugador *j;
	pared *p;
	rayo *lr;
	marca *lw;
} mundo;

/*****************************************************************************/

mundo *crearMundo (void);
void destruirMundo (mundo *m);


#endif