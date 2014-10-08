#include "practica02.h"


/*****************************************************************************/

extern GLfloat fps;
extern GLfloat vectorUp[3];


/*****************************************************************************/

marca *crearMarca (GLfloat *posicion, GLfloat *angulo, GLfloat *color)
{

	GLfloat vida = 0.6;
	GLfloat anchura = 0.02;
	GLuint i;
	marca *w;
	

	if (!(w = (marca *) malloc (sizeof (marca))))
		return 0;

	for (i = 0; i < 3; i++)
	{
		
		w->posicion[i] = posicion[i];
		w->angulo[i] = angulo[i];
		w->color[i] = color[i];

	}

	w->anchura = anchura;
	w->radio = 0.0;
	w->vida = vida;
	w->siguiente = 0;


	return w;

}

/*****************************************************************************/

void destruirMarca (marca *w)
{

	if (w)
		free (w);

}

/*****************************************************************************/

void moverMarca (marca *w)
{

	GLfloat incrementoRadio = 0.75;
	GLfloat incrementoColor = 1.75;
	GLuint i;


	// aumentar el radio
	w->radio += incrementoRadio / fps;
	
	// cambiar el color hacia mas oscuro
	for (i = 0; i < 3; i++)
		w->color[i] -= incrementoColor / fps;

}

/*****************************************************************************/

void dibujarMarca (marca *w)
{
	GLfloat aux;


	aux = w->radio - w->anchura;
	if (aux < 0.0)
		aux = 0.0;

	glPushMatrix();
		glColor3fv (w->color);
		
		// trasladar
		glTranslatef (w->posicion[0], w->posicion[1], w->posicion[2]);
		
		// alinear con la pared
		glRotatef (w->angulo[0], 1.0, 0.0, 0.0);
		glRotatef (w->angulo[1], 0.0, 1.0, 0.0);
		glRotatef (w->angulo[2], 0.0, 0.0, 1.0);
		
		// ajustar al sistema de coordenadas { 0.0, 0.0, 1.0 }
		glRotatef (90.0, vectorUp[2], vectorUp[0], vectorUp[1]);
		
		// dibujar
		glutWireTorus (w->anchura, w->radio, 20, 20);
	glPopMatrix();

}

/*****************************************************************************/

marca *insertarMarca (marca **lw, marca *w)
{

	marca *aux = *lw; 
	
	
	if (aux)
	{
		
		// buscar el final de lista
		while (aux->siguiente)
			aux = aux->siguiente;

		// insertar
		aux->siguiente = w;

	}
	else
		*lw = w;
	
	// devolver el principio de lista
	return *lw;

}

/*****************************************************************************/

marca *sacarMarca (marca **lw, marca *w)
{

	marca *aux = *lw;
	
	
	if (aux)
	{

		// buscar el rayo en la lista
		if (aux == w)
			// si es el primer elemento
			*lw = aux->siguiente;
		else
		{

			// sino, recorrer los elementos hasta encontrarlo
			while (aux->siguiente != w)
				aux = aux->siguiente;

			if (aux)
				// si lo encuentra
				aux->siguiente = w->siguiente;

		}

	}

	// devolver el principio de lista
	return *lw;

}
