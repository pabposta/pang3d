#include "practica02.h"

#include <stdio.h>

/*****************************************************************************/

extern mundo *m;
extern GLfloat fps;
extern sonidos *son;
extern GLubyte reproducir;


/*****************************************************************************/

void colisionPared (bola *b, pared *p)
{
	
	GLfloat roce = 0.995;	// entre 0 y 1, determina el porcentaje de velocidad que pierde
							// la bola al colisionar con una pared (1.0 no pierde nada)
	GLubyte colision = FALSE; // flag para determinar si se reproduce sonido o no

	
	// comprobar colision con las seis paredes del cubo
	// eje x
	if (b->posicion[0] - b->radio <= -p->tamanyo / 2)
	{	
		
		b->posicion[0] = -p->tamanyo / 2 + b->radio;
		b->orientacion[0] *= -roce;
		colision = TRUE;

	}

	if (b->posicion[0] + b->radio >= p->tamanyo / 2)
	{	
		
		b->posicion[0] = p->tamanyo / 2 - b->radio;
		b->orientacion[0] *= -roce;
		colision = TRUE;

	}

	// eje y
	if (b->posicion[1] - b->radio <= -p->tamanyo / 2)
	{	
		
		b->posicion[1] = -p->tamanyo / 2 + b->radio;
		b->orientacion[1] *= -roce;
		colision = TRUE;

	}

	if (b->posicion[1] + b->radio >= p->tamanyo / 2)
	{	
		
		b->posicion[1] = p->tamanyo / 2 - b->radio;
		b->orientacion[1] *= -roce;
		colision = TRUE;

	}

	// eje z
	if (b->posicion[2] - b->radio <= -p->tamanyo / 2)
	{	
		
		b->posicion[2] = -p->tamanyo / 2 + b->radio;
		b->orientacion[2] *= -roce;
		colision = TRUE;

	}


	if (b->posicion[2] + b->radio >= p->tamanyo / 2)
	{	
		
		b->posicion[2] = p->tamanyo / 2 - b->radio;
		b->orientacion[2] *= -roce;
		colision = TRUE;

	}

	// reproducir sonido
	if (colision && reproducir)
	{
		
		alSourcefv (son->fuenteRebote1, AL_POSITION, b->posicion);
		alSourcePlay (son->fuenteRebote1);

	}

}

/*****************************************************************************/

void colisionBola (bola *b1, bola *b2)
{
	
	// las formulas son de la pagina http://user.cs.tu-berlin.de/~timo/nkp/	
	GLfloat g[3];
	GLfloat v0sp, v1sp;
	GLfloat u0sp, u1sp;
	GLfloat u0ss[3], u1ss[3];
	GLfloat m0, m1;
	GLfloat q0;			// m0 / (m0 + m1)
	GLfloat q1;			// m1 / (m0 + m1)
	GLfloat magnitud;
	GLfloat escalaMasa = 0.1;

	
	g[0] = b1->posicion[0] - b2->posicion[0];
	g[1] = b1->posicion[1] - b2->posicion[1];
	g[2] = b1->posicion[2] - b2->posicion[2];

	magnitud = sqrt (pow (g[0], 2) + pow (g[1], 2) + pow (g[2], 2));
	if (magnitud > b1->radio + b2->radio)
		return;

	// han colisionado
	
	m0 = 4 / 3 * M_PI * pow (b1->radio, 3) * FACTORMASA;
	m1 = 4 / 3 * M_PI * pow (b2->radio, 3) * FACTORMASA;
	q0 = m0 / (m0 + m1) - (m0 / (m0 + m1) - m1 / (m0 + m1)) * escalaMasa;
	q1 = m1 / (m0 + m1) + (m0 / (m0 + m1) - m1 / (m0 + m1)) * escalaMasa;

	g[0] /= magnitud;
	g[1] /= magnitud;
	g[2] /= magnitud;

	v0sp = b1->orientacion[0] * g[0] + b1->orientacion[1] * g[1] + b1->orientacion[2] * g[2];
	v1sp = b2->orientacion[0] * g[0] + b2->orientacion[1] * g[1] + b2->orientacion[2] * g[2];
	
	u0sp = 2 * (q0 * v0sp + q1 * v1sp) - v0sp;
	u1sp = 2 * (q0 * v0sp + q1 * v1sp) - v1sp;

	u0ss[0] = b1->orientacion[0] - g[0] * v0sp;
	u0ss[1] = b1->orientacion[1] - g[1] * v0sp;
	u0ss[2] = b1->orientacion[2] - g[2] * v0sp;
	
	u1ss[0] = b2->orientacion[0] - g[0] * v1sp;
	u1ss[1] = b2->orientacion[1] - g[1] * v1sp;
	u1ss[2] = b2->orientacion[2] - g[2] * v1sp;

	b1->orientacion[0] = u0ss[0] + g[0] * u0sp;
	b1->orientacion[1] = u0ss[1] + g[1] * u0sp;
	b1->orientacion[2] = u0ss[2] + g[2] * u0sp;

	b2->orientacion[0] = u1ss[0] + g[0] * u1sp;
	b2->orientacion[1] = u1ss[1] + g[1] * u1sp;
	b2->orientacion[2] = u1ss[2] + g[2] * u1sp;

	// reproducir sonido
	if (reproducir)
	{
		
		alSourcefv (son->fuenteRebote1, AL_POSITION, b1->posicion);
		alSourcePlay (son->fuenteRebote2);

	}

}

/*****************************************************************************/

void aplicarGravedad (bola *b)
{

	GLfloat gravedad[3] = { 0.0, 0.0, -9.81 };


	b->orientacion[0] += gravedad[0] / fps;
	b->orientacion[1] += gravedad[1] / fps;
	b->orientacion[2] += gravedad[2] / fps;

}

/*****************************************************************************/

bola *crearBola (GLfloat *posicion, GLfloat *orientacion, GLfloat radio, GLfloat *color)
{

	GLuint i;
	bola *b;


	if (!(b = (bola *) malloc (sizeof (bola))))
		return 0;

	for (i = 0; i < 3; i++)
	{

		if (fabs (posicion[i]) >= fabs (m->p->tamanyo / 2) - radio)
			if (posicion[i] < 0)
				b->posicion[i] = -m->p->tamanyo / 2 + radio;
			else
				b->posicion[i] = m->p->tamanyo / 2 - radio;
		
		else
			b->posicion[i] = posicion[i];

		b->orientacion[i] = orientacion[i];
		b->color[i] = color[i];

	}

	b->radio = radio;
	b->radioSq = pow (radio, 2);
	b->siguiente = NULL;

	return b;

}

/*****************************************************************************/

void destruirBola (bola *b)
{

	if (b)
		free (b);

}

/*****************************************************************************/

void moverBola (bola *b)
{

	
	bola *aux = b->siguiente;

	
	aplicarGravedad (b);
	colisionPared (b, m->p);

	while (aux)
	{

		colisionBola (b, aux);
		aux = aux->siguiente;

	}

	b->posicion[0] += b->orientacion[0] / fps;
	b->posicion[1] += b->orientacion[1] / fps;
	b->posicion[2] += b->orientacion[2] / fps;

}

/*****************************************************************************/

void dibujarBola (bola *b)
{
	
	GLfloat rotacion = 360.0;
	static GLfloat angulo = 0.0;
	
	
	glPushMatrix();
		angulo += rotacion / fps;
		if (angulo > 360.0)
			angulo -= 360.0;
		
		glTranslatef (b->posicion[0], b->posicion[1], b->posicion[2]);
		glRotatef (angulo, b->orientacion[0], b->orientacion[0], b->orientacion[0]);
		glColor3fv (b->color);
		glutWireSphere (b->radio, 10, 10);
	glPopMatrix();

}

/*****************************************************************************/

bola *insertarBola (bola **lista, bola *b)
{

	bola *aux = *lista; 
	
	
	if (aux)
	{
		
		// buscar el final de lista
		while (aux->siguiente)
			aux = aux->siguiente;

		// insertar
		aux->siguiente = b;

	}
	else
		*lista = b;
	
	// devolver el principio de lista
	return *lista;

}

/*****************************************************************************/

bola *sacarBola (bola **lista, bola *b)
{

	bola *aux = *lista;
	
	
	if (aux)
	{

		// buscar la bola en la lista
		if (aux == b)
			// si es el primer elemento
			*lista = aux->siguiente;
		else
		{

			// sino, recorrer los elementos hasta encontrarlo
			while (aux->siguiente != b)
				aux = aux->siguiente;

			if (aux)
				// si lo encuentra
				aux->siguiente = b->siguiente;

		}

	}

	// devolver el principio de lista
	return *lista;

}
