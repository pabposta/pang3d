#include "practica02.h"


/*****************************************************************************/

extern mundo *m;
extern GLfloat fps;


/*****************************************************************************/

pared *crearPared (GLfloat tamanyo)
{

	pared *p;


	if (!(p = (pared *) malloc (sizeof (pared))))
		return 0;

	p->tamanyo = tamanyo;

	return p;

}

/*****************************************************************************/

void destruirPared (pared *p)
{

	if (p)
		free (p);

}


/*****************************************************************************/

void dibujarPared (pared *p)
{

	glColor3f (1.0, 1.0, 1.0);
	glutWireCube (m->p->tamanyo);

}
