#include "practica02.h"


/*****************************************************************************/

extern mundo *m;
extern GLfloat fps;
extern GLuint nivel;
extern GLint puntuacion;
extern GLint disparos, aciertos;
extern GLfloat vectorUp[3];
extern sonidos *son;
extern GLubyte reproducir;


/*****************************************************************************/

GLubyte colisionBola (rayo *r, bola **b)
{

	GLfloat radioMinimo = 0.2;

	
	GLfloat u[3];
	GLfloat a[3], c, d, e, f;
	GLfloat p[3], o[3], cl[3] = { 1.0, 0.0, 0.0 };
	bola *aux;
	puntos *s;
	GLint nuevosPuntos;
	GLuint i;
	

	u[0] = r->destino[0] - r->origen[0];
	u[1] = r->destino[1] - r->origen[1];
	u[2] = r->destino[2] - r->origen[2];

	a[0] = r->origen[0] - (*b)->posicion[0];
	a[1] = r->origen[1] - (*b)->posicion[1];
	a[2] = r->origen[2] - (*b)->posicion[2];

	c = pow (u[0], 2) + pow (u[1], 2) + pow (u[2], 2);
	d = 2 * (u[0] * a[0] + u[1] * a[1] + u[2] * a[2]);
	e = pow (a[0], 2) + pow (a[1], 2) + pow (a[2], 2);
	f = 0.25 * pow (d, 2) / c - e + (*b)->radioSq;

	if (f >= 0.0)
	{

		aux = m->lista;
		
		
		// sumar el acierto
		aciertos++;
		
		// sumar los puntos del acierto, dependiendo del tamanyo de bola
		if (*b && (*b)->radio > 2 * radioMinimo)
			nuevosPuntos = 100;
		else
		if (*b && (*b)->radio > radioMinimo)
			nuevosPuntos = 250;
		else
			nuevosPuntos = 500;
		
		// si es la ultima bola sumar puntos por acabar el nivel y reproducir sonido de 
		// victoria
		if (!m->lista->siguiente && (*b)->radio < radioMinimo)
		{
			
			nuevosPuntos += nivel * 100;
			if (reproducir)
			{
				
				alSourcefv (son->fuenteGanas, AL_POSITION, m->j->posicion);
				alSourcePlay (son->fuenteGanas);

			}

		}

		puntuacion += nuevosPuntos;
		
		// crear la puntuacion
		s = crearPuntos ((*b)->posicion, vectorUp, nuevosPuntos);
		m->ls = insertarPuntos (&m->ls, s);

		
		// ajustar el destino a la bola
		for (i = 0; i < 3; i++)
			r->destino[i] = (*b)->posicion[i];

		// reproducir sonido de acierto
		if (reproducir)
		{
			
			alSourcefv (son->fuenteAcierto, AL_POSITION, (*b)->posicion);
			alSourcePlay (son->fuenteAcierto);

		}
		
		// crear dos bolas nuevas
		if (aux != *b)
		{
			
			while (aux->siguiente != *b)
				aux = aux->siguiente;
		
			if ((*b)->radio > radioMinimo)
			{	
				
				// crear dos bolas nuevas
				p[0] = (*b)->posicion[0] + (*b)->radio;
				p[1] = (*b)->posicion[1] + (*b)->radio;
				p[2] = (*b)->posicion[2];

				o[0] = (*b)->orientacion[0];
				o[1] = (*b)->orientacion[1];
				o[2] = (*b)->orientacion[2];

				insertarBola (&m->lista, crearBola (p, o, (*b)->radio / 2, cl));
				
				
				p[0] = (*b)->posicion[0] - (*b)->radio;
				p[1] = (*b)->posicion[1] - (*b)->radio;

				o[0] = - o[0];
				o[1] = - o[1];
				
				insertarBola (&m->lista, crearBola (p, o, (*b)->radio / 2, cl));

			}
			
			// destruir la bola vieja
			m->lista = sacarBola (&m->lista, *b);
			destruirBola (*b);
			// asignar nueva posicion al puntero a las bolas
			*b = aux;

		}
		else
		{
			
			if ((*b)->radio > radioMinimo)
			{	
				
				// crear dos bolas nuevas
				p[0] = (*b)->posicion[0] + (*b)->radio;
				p[1] = (*b)->posicion[1] + (*b)->radio;
				p[2] = (*b)->posicion[2];

				o[0] = (*b)->orientacion[0];
				o[1] = (*b)->orientacion[1];
				o[2] = (*b)->orientacion[2];

				insertarBola (&m->lista, crearBola (p, o, (*b)->radio / 2, cl));
				
				
				p[0] = (*b)->posicion[0] - (*b)->radio;
				p[1] = (*b)->posicion[1] - (*b)->radio;

				o[0] = - o[0];
				o[1] = - o[1];
				
				insertarBola (&m->lista, crearBola (p, o, (*b)->radio / 2, cl));

			}

			// destruir la bola vieja
			m->lista = sacarBola (&m->lista, *b);
			destruirBola (*b);
			// asignar nueva posicion al puntero a las bolas
			*b = m->lista;
			
		}
	
		return TRUE; 

	}

	return FALSE;

}

/*****************************************************************************/

void colisionPared (rayo *r, pared *p)
{

	GLfloat n[6][3] = {
		{-1.0, 0.0, 0.0 },
		{ 1.0, 0.0, 0.0 },
		{ 0.0,-1.0, 0.0 },
		{ 0.0, 1.0, 0.0 },
		{ 0.0, 0.0,-1.0 },
		{ 0.0, 0.0, 1.0 }
	};
	GLfloat u[3];
	GLfloat d = -p->tamanyo / 2; // en este caso la d es fija
	GLfloat na, nu;
	GLfloat lambda, lambdaFinal = -1.0;
	GLfloat angulo[3];
	GLfloat productoEscalar;
	GLuint paredFinal;
	GLuint i;


	if (r->vida > 0.0)
	{
	
		// calcular la interseccion con la pared mas "cercana" (con la que primero da)
		// empezamos por calcular el vector de direccion del rayo
		u[0] = r->destino[0] - r->origen[0];
		u[1] = r->destino[1] - r->origen[1];
		u[2] = r->destino[2] - r->origen[2];

		for (i = 0; i < 6; i++)
		{
			
			// calcular para cada pared la interseccion del rayo con el plano que la
			// representa. para esto se calcula la solucion del sistema de ecuaciones
			// n * (a + lambda * u) = d

			na = r->origen[0] * n[i][0] + r->origen[1] * n[i][1] + r->origen[2] * n[i][2];
			nu = u[0] * n[i][0] + u[1] * n[i][1] + u[2] * n[i][2];

			// si n y u son perpendiculares (n * u = 0), el rayo y la pared son
			// perpendiculares y no hay solucion
			if (nu != 0)
			{
			
				// calcular lambda
				lambda = (d - na) / nu;
				// comprobar que el rayo venga desde dentro del cubo
				productoEscalar = u[0] * n[i][0] + u[1] * n[i][1] + u[2] * n[i][2];
				// y quedarse con la mas pequenya (la pared mas cercana)
				if (lambda > 0.0 && (lambda < lambdaFinal || lambdaFinal < 0.0) &&
				    productoEscalar < 0.0)
				{	
					
					lambdaFinal = lambda;
					paredFinal = i;

				}

			}

		}

		r->destino[0] = r->origen[0] + lambdaFinal * u[0];
		r->destino[1] = r->origen[1] + lambdaFinal * u[1];
		r->destino[2] = r->origen[2] + lambdaFinal * u[2];

		// crear una marca en la pared
		// angulos para alinear con la pared
		angulo[0] = (n[paredFinal][1] + n[paredFinal][2]) * 90.0;
		angulo[1] = (n[paredFinal][1] + n[paredFinal][2]) * 90.0;
		angulo[2] = (n[paredFinal][1] + n[paredFinal][0]) * 90.0;
		
		m->lw = insertarMarca (&m->lw, crearMarca (r->destino, angulo, r->color));

	}

}

/*****************************************************************************/

rayo *dispararRayo (GLfloat *origen, GLfloat *destino)
{

	rayo *r;
	bola **aux, *aux2;
	GLubyte acierto = FALSE;
	GLfloat vida = 0.3;
	GLfloat color[3] = { 0.85, 1.0, 1.0 };
	GLuint i;


	// crear el rayo
	if (!(r = (rayo *) malloc (sizeof (rayo))))
		return 0;
	
	for (i = 0; i < 3; i++)
	{
	
		r->origen[i] = origen[i];
		r->destino[i] = destino[i];
		r->color[i] = color[i];

	}

	r->vida = vida;
	r->siguiente = 0;
	
		
	// recorrer todas las bolas
	aux2 = m->lista;
	aux = &aux2;
	while (*aux)
	{

		// colision con una bola
		if (colisionBola (r, aux))
		{
			
			// marcar que se ha acertado
			acierto = TRUE;
			
			// avanzar en la lista
			if (*aux && *aux != m->lista)
				*aux = (*aux)->siguiente;

		}
		else
			// avanzar en la lista
			*aux = (*aux)->siguiente;
		
	}

	
	if (acierto)
	{
		// a partir de aqui viene la parte de dibujar el rayo, para esto cambiaremos
		// el origen del rayo de la posicion de la camara a la posicion del jugador
		for (i = 0; i < 3; i++)
			r->origen[i] = m->j->posicion[i];

		// colision con la pared
		colisionPared (r, m->p);
	
	}
	else 
	{

		// primero la colision con la pared, ya que el destino aun no se ha modificado
		colisionPared (r, m->p);

		// y ahora cambiar el origen
		for (i = 0; i < 3; i++)
			r->origen[i] = m->j->posicion[i];
		
		// restar puntuacion si se ha fallado el disparo
		puntuacion -= 75;

	}
		
	// sumar el disparo
	disparos++;

	// reproducir el sonido del disparo
	if (reproducir)
	{
		
		alSourcefv (son->fuenteRayo, AL_POSITION, r->origen);
		alSourcePlay (son->fuenteRayo);

	}

	
	return r;

}

/*****************************************************************************/

void destruirRayo (rayo *r)
{

	if (r)
		free (r);
		
}

/*****************************************************************************/

void dibujarRayo (rayo *r)
{
		
		// dibujar la linea
		glBegin (GL_LINES);
			glColor3fv (r->color);
			glVertex3f (r->origen[0], r->origen[1], r->origen[2]);
			glVertex3f (r->destino[0], r->destino[1], r->destino[2]);
		glEnd();
		
}

/*****************************************************************************/

rayo *insertarRayo (rayo **lr, rayo *r)
{
	
	rayo *aux = *lr; 
	
	
	if (aux)
	{
		
		// buscar el final de lista
		while (aux->siguiente)
			aux = aux->siguiente;

		// insertar
		aux->siguiente = r;

	}
	else
		*lr = r;
	
	// devolver el principio de lista
	return *lr;

}

/*****************************************************************************/

rayo *sacarRayo (rayo **lr, rayo *r)
{
	
	rayo *aux = *lr;
	
	
	if (aux)
	{

		// buscar el rayo en la lista
		if (aux == r)
			// si es el primer elemento
			*lr = aux->siguiente;
		else
		{

			// sino, recorrer los elementos hasta encontrarlo
			while (aux->siguiente != r)
				aux = aux->siguiente;

			if (aux)
				// si lo encuentra
				aux->siguiente = r->siguiente;

		}

	}

	// devolver el principio de lista
	return *lr;

}
