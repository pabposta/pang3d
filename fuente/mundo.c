#include "practica02.h"


/*****************************************************************************/

mundo *crearMundo (void)
{

	mundo *m;


	if (!(m = (mundo *) malloc (sizeof (mundo))))
		return 0;

	m->lista = 0;
	m->ls = 0;
	m->c = 0;
	m->j = 0;
	m->p = 0;
	m->lr = 0;
	m->lw = 0;
		

	return m;

}

/*****************************************************************************/

void destruirMundo (mundo *m)
{

	if (m)
		free (m);

}