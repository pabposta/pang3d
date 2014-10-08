#include "practica02.h"


/*****************************************************************************/

extern GLfloat fps;
extern GLfloat vectorUp[3];


/*****************************************************************************/

puntos *crearPuntos (GLfloat *posicion, GLfloat *orientacion, GLint puntuacion)
{

	puntos *s;
	GLfloat velocidad = 0.5;
	GLfloat color[3] = { 1.0, 0.65, 0.0 };
	GLfloat vida = 3.0;
	GLfloat magnitud;
	GLuint i;
	
	
	if (!(s = (puntos *) malloc (sizeof (puntos))))
		return 0;

	for (i = 0; i< 3; i++)
	{
			
		s->posicion[i] = posicion[i];
		s->orientacion[i] = orientacion[i];
		s->color[i] = color[i];
	
	}
	
	// hacer la orientacion unitaria, ya que mas tarde no cambiara
	magnitud = sqrt (pow (s->orientacion[0], 2) + pow (s->orientacion[1], 2) +
	                 pow (s->orientacion[2], 2));

	for (i = 0; i < 3; i++)
		s->orientacion[i] /= magnitud;

	s->angulo = 0.0;
	s->velocidad = velocidad;
	s->vida = vida;
	s->puntuacion = puntuacion;
	s->siguiente = 0;


	return s;

}

/*****************************************************************************/

void destruirPuntos (puntos *s)
{

	if (s)
		free (s);

}

/*****************************************************************************/

void moverPuntos (puntos *s)
{

	GLfloat incrementoAngulo = 360.0;
	GLuint i;


	if (s->vida > 0.0)
	{

		for (i = 0; i < 3; i++)
			s->posicion[i] += s->orientacion[i] * s->velocidad / fps;

		s->angulo += incrementoAngulo / fps;
		
		if (s->angulo >= 360.0)
			s->angulo -= 360.0;
	
	}

}

/*****************************************************************************/

void dibujarPuntos (puntos *s)
{

	unsigned char puntuacionTexto[8];
	char aux[5];
	GLuint i;
	GLfloat escala = 0.002;
	GLfloat longitud;

	
	if (s->vida > 0.0)
	{

		// convertir la puntuacion a texto
		sprintf (aux, "%d", s->puntuacion);
		for (i = 0; aux[i]; i++)
			puntuacionTexto[i] = aux[i];
		puntuacionTexto[i] = 0;

		// calcular la longitud del texto
		longitud = glutStrokeLength (GLUT_STROKE_ROMAN, puntuacionTexto);
		
		glMatrixMode (GL_MODELVIEW);
		glPushMatrix();
		
		// color
		glColor3fv (s->color);

		// trasladar
		glTranslatef (s->posicion[0], s->posicion[1], s->posicion[2]);
				
		// rotar
		glRotatef (s->angulo, s->orientacion[0], s->orientacion[1], s->orientacion[2]);
		
		// ajustar al sistema de coordenadas { 0.0, 0.0, 1.0 }
		glRotatef (90.0, vectorUp[2], vectorUp[0], vectorUp[1]);

		// centrar el texto
		glTranslatef (-longitud * escala * 0.5 , 0.0, 0.0);
		
		// ajustar tamanyo
		glScalef (escala, escala, escala);

		// dibujar
		for (i = 0; puntuacionTexto[i]; i++)
			glutStrokeCharacter (GLUT_STROKE_ROMAN, puntuacionTexto[i]);

		glPopMatrix();

	}

}

/*****************************************************************************/

puntos *insertarPuntos (puntos **ls, puntos *s)
{
	
	puntos *aux = *ls; 
	
	
	if (aux)
	{
		
		// buscar el final de lista
		while (aux->siguiente)
			aux = aux->siguiente;

		// insertar
		aux->siguiente = s;

	}
	else
		*ls = s;
	
	// devolver el principio de lista
	return *ls;

}

/*****************************************************************************/

puntos *sacarPuntos (puntos **ls, puntos *s)
{
	
	puntos *aux = *ls;
	
	
	if (aux)
	{

		// buscar los puntos en la lista
		if (aux == s)
			// si es el primer elemento
			*ls = aux->siguiente;
		else
		{

			// sino, recorrer los elementos hasta encontrarlo
			while (aux->siguiente != s)
				aux = aux->siguiente;

			if (aux)
				// si lo encuentra
				aux->siguiente = s->siguiente;

		}

	}

	// devolver el principio de lista
	return *ls;

}
