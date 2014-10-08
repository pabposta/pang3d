#include "practica02.h"


/*****************************************************************************/

extern mundo *m;
extern GLfloat vectorUp[3];
extern GLuint posicionCamara;


/*****************************************************************************/

camara *crearCamara (void)
{
	
	camara *c;
	GLuint i;

	
	if (!(c = (camara *) malloc (sizeof (camara))))
		return 0;


	for (i = 0; i < 3; i++)
	{
		
		c->posicion[i] = 0.0;
		c->orientacion[i] = vectorUp[i];

	}


	return c;

}

/*****************************************************************************/

void destruirCamara (camara *c)
{

	if (c)
		free (c);

}

/*****************************************************************************/

void moverCamara (camara *c)
{
	
	GLuint i;


	for (i = 0; i < 3; i++)
	{
		
		c->orientacion[i] = m->j->orientacionVista[i];
		
		switch (posicionCamara)
		{

		// la posicion por defecto es primera persona
		default:
		case 0:
			// primera persona
		case 1:
			// primera persona, pero dibujando al personaje
			c->posicion[i] = m->j->posicion[i] + vectorUp[i] * (m->j->radio / 3) +
							 (1.0 - vectorUp[i]) * m->j->orientacionVista[i] / 4;
			break;
		case 2:
			// persecucion desde cerca
			c->posicion[i] = m->j->posicion[i] + vectorUp[i] * m->j->radio - m->j->orientacionVista[i];
			break;
		case 3:
			// persecucion desde lejos
			c->posicion[i] = m->j->posicion[i] + vectorUp[i] * m->j->radio - m->j->orientacionVista[i] * 2.0;
			break;
		case 4:
			// desde el techo
			c->posicion[0] =  0.0;
			c->posicion[1] =  m->p->tamanyo / 2 * 1.5;
			c->posicion[2] =  m->p->tamanyo / 2;
			break;
				
		}

	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt (c->posicion[0], c->posicion[1], c->posicion[2],
	           c->posicion[0] + c->orientacion[0],
	           c->posicion[1] + c->orientacion[1],
			   c->posicion[2] + c->orientacion[2],
	           vectorUp[0], vectorUp[1], vectorUp[2]);

	
	// actualizar la posicion en openAL
	alListenerfv (AL_POSITION, c->posicion);
	alListenerfv (AL_ORIENTATION, c->orientacion);

}	

	