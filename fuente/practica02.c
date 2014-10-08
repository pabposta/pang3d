#include "practica02.h"


/*****************************************************************************/

mundo *m;
GLfloat fps = 500.0;
GLfloat vectorUp[3] = { 0.0, 0.0, 1.0 };
GLuint frames = 0;
GLubyte pausa = FALSE;
GLuint nivel = 1;
GLint puntuacion = 0;
GLint disparos = 0, aciertos = 0;
GLuint dificultad = 0;
GLuint posicionCamara = 0;
GLubyte dibujarEsfera = FALSE;
sonidos *son;
GLubyte reproducir = TRUE;


/*****************************************************************************/

void inicializar (void)
{

	GLfloat p[3] = { 0.0, 1.0, 0.0 },
			o[3] = { 2.0, 1.0, 0.0 },
	        r = 0.5,
			c[3] = { 1.0, 0.0, 0.0 },
			jp[3] = { 0.0, 0.0, 0.0 },
			a[3] = { -180.0, 30.0, 45.0 };
	GLuint i;


	m = crearMundo();
	m->p = crearPared (8.0);
	jp[2] -= m->p->tamanyo / 2;
	m->j = crearJugador (jp, a);
	m->c = crearCamara();
				
	o[0] = (14.0 - 14.0 / (1.0 + (((GLfloat) nivel - 1.0) / 100.5))) + 2.0;
	o[1] = o[0] * 0.5;
	o[2] = 0.0;
	m->lista = insertarBola (&m->lista, crearBola (p, o, r, c));

	if (dificultad > 0)
	{
		
		for (i = 0; i < 3; i++)
		{	
		
			p[i] = -p[i];
			o[i] = -o[i];

		}

		p[2] = 2.0 / (1.0 + (((GLfloat) nivel - 1.0) / 9.0)) - 1.0;
		insertarBola (&m->lista, crearBola (p, o, r, c));

	}
	
	glutSetCursor (GLUT_CURSOR_CROSSHAIR);

	// reproducir musica de indy
	if (reproducir)
	{
	
		alSourcefv (son->fuenteIndy, AL_POSITION, vectorUp);
    	alSourcePlay (son->fuenteIndy);

	}

}

/*****************************************************************************/

void reshape (GLint x, GLint y)
{

	glutReshapeWindow (TAMANYOVENTANA, TAMANYOVENTANA);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glViewport (0, 0, TAMANYOVENTANA, TAMANYOVENTANA);
	gluPerspective (90.0, (GLfloat)  TAMANYOVENTANA/ (GLfloat) TAMANYOVENTANA, 0.01, 1000.0);

}

/*****************************************************************************/

void display (void)
{

	bola *aux = m->lista;
	puntos *auxPuntos = m->ls;
	rayo *auxRayo = m->lr;
	marca *auxMarca = m->lw;
	GLuint i;
	unsigned char ganas[] = "ganas";
	unsigned char pierdes[] = "pierdes";
	unsigned char adicional[] = "espacio para jugar   escape para salir";
	unsigned char textoPausa[] = "pausa";
	unsigned char pausaAdicional[] = "p o espacio para continuar";
	unsigned char puntuacionTexto[] = "00000000000";
	unsigned char aciertosTexto[] = "00000/00000";
	unsigned char nivelTexto[]  = "000000";
	char auxTexto[12];


	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode (GL_MODELVIEW);
	
	// dibujar la pared
	dibujarPared (m->p);
	
	// dibujar las bolas
	while (aux)
	{

		dibujarBola (aux); 
		aux = aux->siguiente;

	}
	
	// dibujar los rayos
	while (auxRayo)
	{	
		
		dibujarRayo (auxRayo);
		auxRayo = auxRayo->siguiente;

	}
	
	// dibujar las puntuaciones
	while (auxPuntos)
	{

		dibujarPuntos (auxPuntos);
		auxPuntos = auxPuntos->siguiente;

	}

	// dibujar las marcas en las paredes
	while (auxMarca)
	{

		dibujarMarca (auxMarca);
		auxMarca = auxMarca->siguiente;

	}

	// dibujar al jugador
	if (posicionCamara != 0) // en primera persona no se dibuja
		dibujarJugador (m->j);
		
	// mover la camara
	moverCamara (m->c);	
	
	// escribir texto
	glPushMatrix();
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D (0.0, TAMANYOVENTANA, 0.0, TAMANYOVENTANA);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
	glColor3f (0.0, 0.5, 1.0);
	
	// si se ha perdido
	if (!m->j->vivo)
	{
		
		glutSetCursor (GLUT_CURSOR_NONE);
		glRasterPos2i (TAMANYOVENTANA / 2 - glutBitmapLength (GLUT_BITMAP_HELVETICA_18,
		               pierdes) / 2, TAMANYOVENTANA / 2 + 18);

		for (i = 0; pierdes[i]; i++)
			glutBitmapCharacter (GLUT_BITMAP_HELVETICA_18, pierdes[i]);

		glRasterPos2i (TAMANYOVENTANA / 2 - glutBitmapLength (GLUT_BITMAP_HELVETICA_12,
		               adicional) / 2, TAMANYOVENTANA / 2 - 12);
		for (i = 0; adicional[i]; i++)
			glutBitmapCharacter (GLUT_BITMAP_HELVETICA_12, adicional[i]);
		
	}
	// si se ha ganado
	else if (!m->lista)
	{
		
		glutSetCursor (GLUT_CURSOR_NONE);
		glRasterPos2i (TAMANYOVENTANA / 2 - glutBitmapLength (GLUT_BITMAP_HELVETICA_18,
		               ganas) / 2, TAMANYOVENTANA / 2 + 18);
		for (i = 0; ganas[i]; i++)
			glutBitmapCharacter (GLUT_BITMAP_HELVETICA_18, ganas[i]);

		glRasterPos2i (TAMANYOVENTANA / 2 - glutBitmapLength (GLUT_BITMAP_HELVETICA_12,
		               adicional) / 2, TAMANYOVENTANA / 2 - 12);
		for (i = 0; adicional[i]; i++)
			glutBitmapCharacter (GLUT_BITMAP_HELVETICA_12, adicional[i]);

	}

	// si se esta en modo pausa
	if (pausa)
	{
		
		glutSetCursor (GLUT_CURSOR_LEFT_ARROW);
		glRasterPos2i (TAMANYOVENTANA / 2 - glutBitmapLength (GLUT_BITMAP_HELVETICA_18,
		               textoPausa) / 2, TAMANYOVENTANA / 2 + 18);
		for (i = 0; textoPausa[i]; i++)
			glutBitmapCharacter (GLUT_BITMAP_HELVETICA_18, textoPausa[i]);

		glRasterPos2i (TAMANYOVENTANA / 2 - glutBitmapLength (GLUT_BITMAP_HELVETICA_12,
		               pausaAdicional) / 2, TAMANYOVENTANA / 2 - 12);
		for (i = 0; pausaAdicional[i]; i++)
			glutBitmapCharacter (GLUT_BITMAP_HELVETICA_12, pausaAdicional[i]);

	}

	// dibujar puntuacion
	// covertir puntuacion a texto
	sprintf (auxTexto, "%06d", puntuacion);
		
	for (i = 0; auxTexto[i]; i++)
		puntuacionTexto[i] = auxTexto[i];
	puntuacionTexto[i] = '\0';
	
	glRasterPos2i (TAMANYOVENTANA - glutBitmapLength (GLUT_BITMAP_HELVETICA_18,
		           puntuacionTexto), TAMANYOVENTANA - 18);
	for (i = 0; puntuacionTexto[i]; i++)
		glutBitmapCharacter (GLUT_BITMAP_HELVETICA_18, puntuacionTexto[i]);
	
	// dibujar aciertos
	// convertir aciertos y disparos a texto
	sprintf (auxTexto, "%03d/%03d", aciertos, disparos);

	for (i = 0; auxTexto[i]; i++)
		aciertosTexto[i] = auxTexto[i];
	aciertosTexto[i] = '\0';

	glRasterPos2i (TAMANYOVENTANA - glutBitmapLength (GLUT_BITMAP_HELVETICA_12,
		           aciertosTexto), TAMANYOVENTANA - 18 - 12 - 4);
	for (i = 0; aciertosTexto[i]; i++)
		glutBitmapCharacter (GLUT_BITMAP_HELVETICA_12, aciertosTexto[i]);

	
	// dibujar el nivel
	// covertir nivel a texto
	sprintf (auxTexto, "%d", nivel);
		
	for (i = 0; auxTexto[i]; i++)
		nivelTexto[i] = auxTexto[i];
	nivelTexto[i] = '\0';
	
	glRasterPos2i (TAMANYOVENTANA - glutBitmapLength (GLUT_BITMAP_HELVETICA_12,
		           nivelTexto), TAMANYOVENTANA - 18 - 12 - 4 - 12 - 4);
	for (i = 0; nivelTexto[i]; i++)
		glutBitmapCharacter (GLUT_BITMAP_HELVETICA_12, nivelTexto[i]);
	

	reshape (TAMANYOVENTANA, TAMANYOVENTANA);
	
	glMatrixMode (GL_MODELVIEW);
	glPopMatrix();


	glutSwapBuffers();

	frames++;

}

/*****************************************************************************/

void idle (void)
{
	
	bola *aux = m->lista;
	puntos *auxPuntos = m->ls;
	rayo *auxRayo = m->lr;
	marca *auxMarca = m->lw;
	static Uint32 tiempoAnterior = 0; // en ms
	GLfloat tiempo; // en s

	
	// medir el tiempo pasado desde el ultimo frame
	tiempo = -((GLfloat) tiempoAnterior);
	tiempo += (GLfloat) (tiempoAnterior = SDL_GetTicks());
	
	if (tiempo < 2)	// un maximo de 500 fps para no consumir cpu
	{	
		
		SDL_Delay ((Uint32) (3 - tiempo));
    	tiempo += -((GLfloat) tiempoAnterior);
		tiempo += (GLfloat) (tiempoAnterior = SDL_GetTicks());

	}

	tiempo /= 1000;
	fps = 1 / tiempo;

	
	// inhibir pausa si el jugador esta muerto
	if (!m->j->vivo)
		pausa = FALSE;
	
	if (!pausa)
	{

		// siempre mover el jugador, para poder mirar libremente despues de morir
		moverJugador (m->j);
		
		// comprobar que los rayo aun esten vivos y sino restarles vida
		while (auxRayo)
		{

			if (auxRayo->vida > 0.0)
			{
				
				auxRayo->vida -= 1 / fps;
				auxRayo = auxRayo->siguiente;

			}
			else
			{	
		
				// los rayos tienen orden cronologico y todos la misma vida (ver puntos)
				m->lr = sacarRayo (&m->lr, auxRayo);
				destruirRayo (auxRayo);
				auxRayo = m->lr;
				
			}

		}

		
		// comprobar que los puntos esten vivos y restarles vida
		// y moverlos
		while (auxPuntos)
		{
			
			if (auxPuntos->vida < 0.0)
			{

				// se puede aprovechar que la lista tiene orden cronologico y que
				// ademas todos los puntos viven el mismo tiempo, siempre se saca
				// el primer elemento de la lista
				m->ls = sacarPuntos (&m->ls, auxPuntos);
				destruirPuntos (auxPuntos);
				auxPuntos = m->ls;

			}
			else
			{
				
				moverPuntos (auxPuntos);
				auxPuntos->vida -= 1 / fps;
				auxPuntos = auxPuntos->siguiente;
				
			}

		}


		// procesar las marcas en las paredas
		while (auxMarca)
		{

			if (auxMarca->vida < 0.0)
			{

				// las marcas tienen orden cronologico y todos la misma vida (ver puntos)
				m->lw = sacarMarca (&m->lw, auxMarca);
				destruirMarca (auxMarca);
				auxMarca = m->lw;

			}
			else
			{

				moverMarca (auxMarca);
				auxMarca->vida -= 1 / fps;
				auxMarca = auxMarca->siguiente;

			}

		}


		// si el jugador esta vivo, mover los objetos restantes
		if (m->j->vivo)
		{

			while (aux)
			{
		
				moverBola (aux);
				aux = aux->siguiente;

			}

		}
		
	}
	
	glutPostRedisplay();
	
}


/*****************************************************************************/

void timer (GLint daigual)
{

	fps = 500.0;//(GLfloat) frames;
	frames = 0;

	glutTimerFunc (1000, timer, 0);

}

/*****************************************************************************/

void imprimirInstrucciones (void)
{

	printf ("Instrucciones\n***************\n\n");

	printf ("raton - mover vista\n");
	printf ("w, a, s, d - desplazarse\n");
	printf ("espacio, boton derecho del raton - saltar\n");
	printf ("boton izquierdo del raton - disparar\n");
	printf ("p - pausa\n");
	printf ("escape - salir\n");
	printf ("n - cambiar el nivel de dificultad\n");
	printf ("k - reiniciar la partida\n");
	printf ("b, v - incrementar (b) / decrementar (v) sensibilidad del raton\n");
	printf ("c - mostrar / esconder esfera de colision\n");
	printf ("f1 a f5 - cambiar posicion de camara\n");
	printf ("m - habilitar / deshabilitar musica\n");
	printf ("\n");

}

/*****************************************************************************/

void cargarSonido (void)
{

	ALsizei tamanyo;
	Uint8  *datos;
	ALfloat velocity[3]= { 0.0, 0.0, 0.0 };
	ALfloat pitch = 1.0, gain = 1.0;
	SDL_AudioSpec wav_spec;

	
	// reservar memoria para los sonidos
	son = (sonidos *) malloc (sizeof (sonidos));

	// generar el buffer del rayo
	alGenBuffers (1, &son->bufferRayo);

	// cargar el sonido de un fichero
	SDL_LoadWAV ("blaster02.wav", &wav_spec, &datos, &tamanyo); 
	
	alBufferData (son->bufferRayo, AL_FORMAT_STEREO8, datos, tamanyo, wav_spec.freq);
	
	SDL_FreeWAV (datos);

	// generar la fuente del rayo
	alGenSources (1, &son->fuenteRayo);

    // inicializar los valores de la fuente
	alSourcef (son->fuenteRayo, AL_PITCH, pitch);
    alSourcef (son->fuenteRayo, AL_GAIN, gain);
    alSourcefv (son->fuenteRayo, AL_VELOCITY, velocity);
    alSourcei (son->fuenteRayo, AL_BUFFER, son->bufferRayo);
    alSourcei (son->fuenteRayo, AL_LOOPING, AL_FALSE);


	// generar el buffer de ganas
	alGenBuffers (1, &son->bufferGanas);

	// cargar el sonido de un fichero
	SDL_LoadWAV ("chewie04.wav", &wav_spec, &datos, &tamanyo); 
	
	alBufferData (son->bufferGanas, AL_FORMAT_STEREO8, datos, tamanyo, wav_spec.freq);
	
	SDL_FreeWAV (datos);

	// generar la fuente de ganas
	alGenSources (1, &son->fuenteGanas);

    // inicializar los valores de la fuente
	alSourcef (son->fuenteGanas, AL_PITCH, pitch);
    alSourcef (son->fuenteGanas, AL_GAIN, gain);
    alSourcefv (son->fuenteGanas, AL_VELOCITY, velocity);
    alSourcei (son->fuenteGanas, AL_BUFFER, son->bufferGanas);
    alSourcei (son->fuenteGanas, AL_LOOPING, AL_FALSE);


	// generar el buffer de pierdes
	alGenBuffers (1, &son->bufferPierdes);

	// cargar el sonido de un fichero
	SDL_LoadWAV ("chewie03.wav", &wav_spec, &datos, &tamanyo); 
	
	alBufferData (son->bufferPierdes, AL_FORMAT_STEREO8, datos, tamanyo, wav_spec.freq);
	
	SDL_FreeWAV (datos);

	// generar la fuente de pierdes
	alGenSources (1, &son->fuentePierdes);

    // inicializar los valores de la fuente
	alSourcef (son->fuentePierdes, AL_PITCH, pitch);
    alSourcef (son->fuentePierdes, AL_GAIN, gain);
    alSourcefv (son->fuentePierdes, AL_VELOCITY, velocity);
    alSourcei (son->fuentePierdes, AL_BUFFER, son->bufferPierdes);
    alSourcei (son->fuentePierdes, AL_LOOPING, AL_FALSE);


	// generar el buffer de acierto
	alGenBuffers (1, &son->bufferAcierto);

	// cargar el sonido de un fichero
	SDL_LoadWAV ("expl02.wav", &wav_spec, &datos, &tamanyo); 
	
	alBufferData (son->bufferAcierto, AL_FORMAT_STEREO8, datos, tamanyo, wav_spec.freq);
	
	SDL_FreeWAV (datos);

	// generar la fuente del acierto
	alGenSources (1, &son->fuenteAcierto);

    // inicializar los valores de la fuente
	alSourcef (son->fuenteAcierto, AL_PITCH, pitch);
    alSourcef (son->fuenteAcierto, AL_GAIN, gain);
    alSourcefv (son->fuenteAcierto, AL_VELOCITY, velocity);
    alSourcei (son->fuenteAcierto, AL_BUFFER, son->bufferAcierto);
    alSourcei (son->fuenteAcierto, AL_LOOPING, AL_FALSE);


	// generar el buffer de indy
	alGenBuffers (1, &son->bufferIndy);

	// cargar el sonido de un fichero
	SDL_LoadWAV ("indy.wav", &wav_spec, &datos, &tamanyo); 
	
	alBufferData (son->bufferIndy, AL_FORMAT_STEREO8, datos, tamanyo, wav_spec.freq);
	
	SDL_FreeWAV (datos);

	// generar la fuente de indy
	alGenSources (1, &son->fuenteIndy);

    // inicializar los valores de la fuente
	alSourcef (son->fuenteIndy, AL_PITCH, pitch);
    alSourcef (son->fuenteIndy, AL_GAIN, gain);
    alSourcefv (son->fuenteIndy, AL_VELOCITY, velocity);
    alSourcei (son->fuenteIndy, AL_BUFFER, son->bufferIndy);
    alSourcei (son->fuenteIndy, AL_LOOPING, AL_FALSE);

	
	// generar el buffer de rebote 1
	alGenBuffers (1, &son->bufferRebote1);

	// cargar el sonido de un fichero
	SDL_LoadWAV ("bounce1.wav", &wav_spec, &datos, &tamanyo); 
	
	alBufferData (son->bufferRebote1, AL_FORMAT_STEREO8, datos, tamanyo, wav_spec.freq);
	
	SDL_FreeWAV (datos);

	// generar la fuente de rebote 1
	alGenSources (1, &son->fuenteRebote1);

    // inicializar los valores de la fuente
	alSourcef (son->fuenteRebote1, AL_PITCH, pitch);
    alSourcef (son->fuenteRebote1, AL_GAIN, gain);
    alSourcefv (son->fuenteRebote1, AL_VELOCITY, velocity);
    alSourcei (son->fuenteRebote1, AL_BUFFER, son->bufferRebote1);
    alSourcei (son->fuenteRebote1, AL_LOOPING, AL_FALSE);


	// generar el buffer de rebote 2
	alGenBuffers (1, &son->bufferRebote2);

	// cargar el sonido de un fichero
	SDL_LoadWAV ("bounce2.wav", &wav_spec, &datos, &tamanyo); 
	
	alBufferData (son->bufferRebote2, AL_FORMAT_STEREO8, datos, tamanyo, wav_spec.freq);
	
	SDL_FreeWAV (datos);

	// generar la fuente de rebote 2
	alGenSources (1, &son->fuenteRebote2);

    // inicializar los valores de la fuente
	alSourcef (son->fuenteRebote2, AL_PITCH, pitch);
    alSourcef (son->fuenteRebote2, AL_GAIN, gain);
    alSourcefv (son->fuenteRebote2, AL_VELOCITY, velocity);
    alSourcei (son->fuenteRebote2, AL_BUFFER, son->bufferRebote2);
    alSourcei (son->fuenteRebote2, AL_LOOPING, AL_FALSE);


	
	// inicializar los valores del oyente
	alListenerfv (AL_VELOCITY, velocity);
	
}


/*****************************************************************************/

void salir (void)
{

	bola *b;
	puntos *s;
	rayo *r;
	marca *w;

	
	// destruir los objetos del mundo
	destruirJugador (m->j);
	destruirPared (m->p);
	destruirCamara (m->c);
		
	while (m->lista)
	{

		b = m->lista;
		m->lista = sacarBola (&m->lista, b);
		destruirBola (b);
	
	}

	while (m->lr)
	{
		
		r = m->lr;
		m->lr = sacarRayo (&m->lr, r);
		destruirRayo (r);

	}

	while (m->ls)
	{
				
		s = m->ls;
		m->ls = sacarPuntos (&m->ls, s);
		destruirPuntos (s);

	}

	while (m->lw)
	{

		w = m->lw;
		m->lw = sacarMarca (&m->lw, w);
		destruirMarca (w);

	}

	// destruir el mundo
	destruirMundo (m);
	
	
	// cerrar sdl
	SDL_Quit();

	// cerrar alut
	alutExit();
    
}

/*****************************************************************************/

int main (int argc, char *argv[])
{
	
	// inicializar glut
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize (TAMANYOVENTANA, TAMANYOVENTANA);
	glutInitWindowPosition (200, 200);
	glutCreateWindow ("practica 02");
	glutDisplayFunc (display);
	glutReshapeFunc (reshape);
	glEnable (GL_DEPTH_TEST);
	glLineWidth (1.4);
	glutWarpPointer (TAMANYOVENTANA / 2, TAMANYOVENTANA / 2);

	glutIdleFunc (idle);
	
	glutPassiveMotionFunc (motion);
	glutMotionFunc (motion);
	glutKeyboardFunc (keyboard);
	glutKeyboardUpFunc (keyboardUp);
	glutSpecialFunc (special);
	glutSpecialUpFunc  (specialUp);
	glutMouseFunc (mouse);
		
	// inicializar sdl
	SDL_Init (SDL_INIT_AUDIO | SDL_INIT_TIMER);

	// inicializar alut
	alutInit (&argc, argv);
	
	cargarSonido();

	inicializar();
	
	imprimirInstrucciones();

	atexit (salir);


	glutMainLoop();

	
	return 0;

}
