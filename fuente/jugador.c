#include "practica02.h"

#include <stdio.h>

/*****************************************************************************/

extern mundo *m;
extern GLfloat fps;
extern GLfloat vectorUp[3];
extern GLubyte dibujarEsfera;
extern sonidos *son;
extern GLubyte reproducir;


/*****************************************************************************/

void calcularOrientacion (jugador *j)
{
	
	GLfloat magnitud;
	GLuint i;


	// calcular la orientacion a partir de los angulos
	j->orientacion[0] = sin (j->angulo[0] * M_PI / 180.0) * j->avanzar +
	                    sin ((j->angulo[0] + 90.0) * M_PI / 180.0) * j->lateral;
	j->orientacion[1] = cos (j->angulo[0] * M_PI / 180.0) * j->avanzar +
	                    cos ((j->angulo[0] + 90.0) * M_PI / 180.0) * j->lateral;
	
	// hacer el vector unitario de la orientacion (pero sin tener en cuenta la
	// velocidad vertical que se calcula aparte)
	magnitud = sqrt (pow (j->orientacion[0], 2) + pow (j->orientacion[1], 2));

	if (magnitud > 0.0)
	{
		for (i = 0; i < 2; i++)
			j->orientacion[i] /= magnitud;

		// finalmente, multiplicar por la velocidad
		for (i = 0; i < 2; i++)
			j->orientacion[i] *= j->velocidad;

	}
		
}

/*****************************************************************************/

void calcularOrientacionVista (jugador *j)
{

	GLfloat magnitud;
	GLuint i;

	
	// calcular la orientacion a partir de los angulos
	j->orientacionVista[0] = sin (j->angulo[0] * M_PI / 180.0) *
	                         (cos ((j->angulo[1] + 45.0) * M_PI /180.0) +
	                         sin ((j->angulo[1] + 45.0) * M_PI /180.0));
	j->orientacionVista[1] = cos (j->angulo[0] * M_PI / 180.0) *
	                         (cos ((j->angulo[1] + 45.0) * M_PI /180.0) +
	                         sin ((j->angulo[1] + 45.0) * M_PI /180.0));
	j->orientacionVista[2] = sin (j->angulo[1] * M_PI / 180.0) -
	                         cos ((j->angulo[1] + 45.0) * M_PI /180.0);

	magnitud = sqrt (pow (j->orientacionVista[0], 2) +
	                 pow (j->orientacionVista[1], 2) +
	                 pow (j->orientacionVista[2], 2));

	for (i = 0; i < 3; i++)
		j->orientacionVista[i] /= magnitud;

}

/*****************************************************************************/

void colisionPared (jugador *j, pared *p)
{
	
	// colision con las paredes del eje x
	if (j->posicion[0] + j->radio > p->tamanyo / 2)
		j->posicion[0] = p->tamanyo / 2 - j->radio;
	else
	if (j->posicion[0] - j->radio < -p->tamanyo / 2)
		j->posicion[0] = - p->tamanyo / 2 + j->radio;

	// colision con las paredes del eje y
	if (j->posicion[1] + j->radio > p->tamanyo / 2)
		j->posicion[1] = p->tamanyo / 2 - j->radio;
	else
	if (j->posicion[1] - j->radio < -p->tamanyo / 2)
		j->posicion[1] = - p->tamanyo / 2 + j->radio;

	// colision con las paredes del eje z
	if (j->posicion[2] + j->radio > p->tamanyo / 2)
		j->posicion[2] = p->tamanyo / 2 - j->radio;
	else
	if (j->posicion[2] - j->radio < -p->tamanyo / 2)
	{
		
		j->posicion[2] = - p->tamanyo / 2 + j->radio;
		// poner orientacion de z a 0 si es < 0, para que la gravedad no la haga infinita
		if (j->orientacion[2] < 0)
			j->orientacion[2] = 0.0;

	}

}

/*****************************************************************************/

void colisionBola (jugador *j, bola *b)
{
	
	GLfloat g[3];
	GLfloat magnitud;

	
	g[0] = j->posicion[0] - b->posicion[0];
	g[1] = j->posicion[1] - b->posicion[1];
	g[2] = j->posicion[2] - b->posicion[2];

	magnitud = sqrt (pow (g[0], 2) + pow (g[1], 2) + pow (g[2], 2));
	if (magnitud <= j->radio + b->radio)
	{	
		
		j->velocidad = 0.0;
		j->vivo = FALSE;

		// reproducir sonido de muerte
		if (reproducir)
		{
		
			alSourcefv (son->fuentePierdes, AL_POSITION, j->posicion);
			alSourcePlay (son->fuentePierdes);

		}

	}

}

/*****************************************************************************/

void aplicarGravedad (jugador *j)
{

	GLfloat gravedad[3] = { 0.0, 0.0, -9.81 };
	

	j->orientacion[0] += gravedad[0] / fps;
	j->orientacion[1] += gravedad[1] / fps;
	j->orientacion[2] += gravedad[2] / fps;

}

/*****************************************************************************/

jugador *crearJugador (GLfloat *posicion, GLfloat *angulo)
{

	GLfloat radio = 0.25;
	GLfloat velocidad = 6.0;
	GLfloat aux[3] = { 0.0, 0.0, 0.0 };
	GLuint i;
	jugador *j;


	if (!(j = (jugador *) malloc (sizeof (jugador))))
		return 0;

	for (i = 0; i < 3; i++)
	{

		j->posicion[i] = posicion[i] + vectorUp[i] * radio;
		j->angulo[i] = angulo[i];
		
	}	
	
	j->posicion[2] += radio;
	j->velocidad = velocidad;
	j->avanzar = 0.0;
	j->lateral = 0.0;
	j->orientacion[2] = 0.0;
	calcularOrientacion (j);
	calcularOrientacionVista (j);
	j->radio = radio;
	j->vivo = TRUE;

	
	return j;

}

/*****************************************************************************/

void destruirJugador (jugador *j)
{

	if (j)
		free (j);

}

/*****************************************************************************/

void moverJugador (jugador *j)
{

	bola *aux = m->lista;
	
	
	if (j->vivo)
	{
	
		// gravedad
		aplicarGravedad (j);

		// calcular las orientaciones a partir de los angulos
		calcularOrientacion (j);
		calcularOrientacionVista (j);

		// mover al jugador
		j->posicion[0] += j->orientacion[0] / fps;
		j->posicion[1] += j->orientacion[1] / fps;
		j->posicion[2] += j->orientacion[2] / fps;

		//comprobar colisiones
		colisionPared (j, m->p);

		while (aux)
		{

			colisionBola (j, aux);
			aux = aux->siguiente;

		}

	}
	else
		// calcular la orientacion de la vista para poder mirar libremente
		// despues de haber muerto
		calcularOrientacionVista (j);

}

/*****************************************************************************/

void avanzarJugador (jugador *j)
{

	j->avanzar = 1.0;

}

/*****************************************************************************/

void retrocederJugador (jugador *j)
{

	j->avanzar = -1.0;

}

/*****************************************************************************/

void izquierdaJugador (jugador *j)
{

	j->lateral = -1.0;

}

/*****************************************************************************/

void derechaJugador (jugador *j)
{

	j->lateral = 1.0;

}

/*****************************************************************************/

void pararAvanzarJugador (jugador *j)
{

	j->avanzar = 0.0;

}

/*****************************************************************************/

void pararRetrocederJugador (jugador *j)
{

	j->avanzar = 0.0;

}

/*****************************************************************************/

void pararIzquierdaJugador (jugador *j)
{

	j->lateral = 0.0;

}

/*****************************************************************************/

void pararDerechaJugador (jugador *j)
{

	j->lateral = 0.0;

}

/*****************************************************************************/

void saltarJugador (jugador *j)
{

	GLfloat altura = 3.0;

	
	if (j->vivo)
		if (!saltandoJugador (j))
			j->orientacion[2] = altura;

}

/*****************************************************************************/

void dispararJugador (jugador *j)
{

	GLfloat aux[3];
	GLuint i;

	
	for (i = 0; i < 3; i++)
		aux[i] = m->c->posicion[i] + m->c->orientacion[i];
	
	m->lr = insertarRayo (&m->lr, dispararRayo (m->c->posicion, aux));

}

/*****************************************************************************/

void dibujarJugador (jugador *j)
{

	static GLfloat achatamiento = 0.0; // el achatamiento para animar a la bola
	GLfloat incremento = 270.0;
	static GLfloat cosAchatamiento = 1.0; 


	// animar el movimiento
	if (!saltandoJugador (j) && (j->avanzar != 0.0 || j->lateral != 0.0))
		cosAchatamiento = fabs (cos (achatamiento * M_PI / 180.0)) * 0.15 + 0.85;
	else
		// si no se mueve, volver suavemente a la posicion normal
		if (cosAchatamiento < 1.0)
			cosAchatamiento += (fabs ((cos (achatamiento * M_PI / 180.0)) -
			                    cos ((achatamiento - incremento / fps) * M_PI / 180.0))) *
			                    0.15;
		else
		{

			cosAchatamiento = 1.0;
			achatamiento = 0.0;

		}

	
	glColor3f (1.0, 0.0, 1.0);
		
	glPushMatrix();
		// trasladar a posicion en el mundo
		glTranslatef (j->posicion[0], j->posicion[1], j->posicion[2] - j->radio * 2 * 0.33 * (1.0 - cosAchatamiento));
		
		// orientar al jugador en la direccion de vista, pero solo si este esta
		// vivo, asi se consigue un efecto de que la camara gire alrededor suyo cuando
		// este haya muerto
		if (j->vivo)
				glRotatef (j->angulo[0], vectorUp[0], vectorUp[1], -vectorUp[2]);

		// dibujar la bola de abajo
		glPushMatrix();
			glTranslatef (0.0, 0.0, -j->radio * 0.33);
			
			// achatarla si el jugador se esta moviendo
			glScalef (1.0 + (1.0 - cosAchatamiento) * 0.3,
			          1.0 + (1.0 - cosAchatamiento) * 0.3, cosAchatamiento);
									
			
			// cambiar el sistema de coordenadas
			glRotatef (90.0, vectorUp[2], vectorUp[0], -vectorUp[1]);

			glutWireSphere (j->radio * 0.67, 10, 10);
		glPopMatrix();

		// dibujar la bola de arriba
		glPushMatrix();
			glTranslatef (0.0, 0.0, -j->radio * 0.5 * (1.0 - cosAchatamiento));

			// cambiar la forma
			glScalef (1.0, 1.0, 2.0 * cosAchatamiento);
			
			// cambiar el sistema de coordenadas
			glRotatef (90.0, vectorUp[2], vectorUp[0], -vectorUp[1]);

			glutWireSphere (j->radio * 0.5, 10, 10);
		glPopMatrix();

		// dibujar el sombrero
		glPushMatrix();
			glTranslatef (0.0, 0.0, j->radio * cosAchatamiento * 0.5);

			// cambiar la forma
			glScalef (1.0, 1.0, 0.0);
			
			// cambiar el sistema de coordenadas
			glRotatef (90.0, vectorUp[2], vectorUp[0], -vectorUp[1]);

			glutWireSphere (0.75 * j->radio, 10, 10);

		glPopMatrix();

	glPopMatrix();

	// dibujar la esfera de colision
	if (dibujarEsfera)
	{
		
		glPushMatrix();
			glTranslatef (j->posicion[0], j->posicion[1], j->posicion[2]);

			// cambiar el sistema de coordenadas
			glRotatef (90.0, vectorUp[2], vectorUp[0], -vectorUp[1]);

			glColor3f (0.5, 1.0, 0.5);
			glutWireSphere (j->radio, 10, 10);
		glPopMatrix();

	}

	// incrementar el achatamiento
	achatamiento += incremento / fps;

	if (achatamiento > 180.0)
		achatamiento -= 360.0;

}

/*****************************************************************************/

GLubyte saltandoJugador (jugador *j)
{
	
	if (j->posicion[2] <= -m->p->tamanyo / 2 + j->radio)
		return FALSE;
	else
		return TRUE;

}