#include "practica02.h"


/*****************************************************************************/

extern mundo *m;
extern GLubyte pausa;
extern GLuint nivel;
extern GLint puntuacion;
extern GLint disparos, aciertos;
extern GLuint dificultad;
extern GLuint posicionCamara;
extern GLubyte dibujarEsfera;
extern sonidos *son;
extern GLubyte reproducir;
GLfloat sensibilidad = 0.5;

extern void inicializar (void);


/*****************************************************************************/

void motion (GLint x, GLint y)
{

	GLint xAnterior = TAMANYOVENTANA / 2, yAnterior = TAMANYOVENTANA / 2;
	GLfloat sensibilidadYextra = 0.5;
	// el flag es porque glutWarpPointer() crea un evento de movimiento que hay que ignorar
	static GLubyte flag = TRUE;


	if (flag)
	{
	
		flag = FALSE;
		
		if (!pausa)
		{

			m->j->angulo[0] += (GLfloat) (x - xAnterior) * sensibilidad;
			m->j->angulo[1] += (GLfloat) (yAnterior - y) * sensibilidad * sensibilidadYextra;
	
			if (m->j->angulo[0] > 180.0)
				m->j->angulo[0] -= 360.0;
			if (m->j->angulo[0] < -180.0)
				m->j->angulo[0] += 360.0;
		
			if (m->j->angulo[1] >= 90.0)
				m->j->angulo[1] = 89.99;
			if (m->j->angulo[1] <= -90.0)
				m->j->angulo[1] = -89.99;

			glutWarpPointer (TAMANYOVENTANA / 2, TAMANYOVENTANA / 2);
		
		}
	
		xAnterior = x;
		yAnterior = y;
		
	}
	else
		flag = TRUE;

	glutPostRedisplay();

}

/*****************************************************************************/

void keyboard (GLubyte key, GLint x, GLint y)
{

	bola *b;
	puntos *s;
	rayo *r;
	marca *w;

	
	switch (key)
	{

	// escape
	case 27:
		exit (0);
		break;
	
	// espacio
	case 32:
		// salir de modo pausa
		if (pausa)
		{	
			
			pausa = FALSE;
			glutSetCursor (GLUT_CURSOR_CROSSHAIR);
			glutWarpPointer (TAMANYOVENTANA / 2, TAMANYOVENTANA / 2);
			break;

		}
		else
		// reiniciar paritda si esta ha acabado
		if (!m->j->vivo || !m->lista)
		{

			// resetear valores de nivel y puntuacion a 1 y 0 respectivamente si se pierde
			if (!m->j->vivo)
			{
			
				nivel = 1;
				puntuacion = 0;
				disparos = 0;
				aciertos = 0;

			}
			// sino, subir el nivel
			else
				nivel++;
			
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


			destruirMundo (m);
			
			inicializar();

		}
		else
		// saltar
		if (!pausa)	
			saltarJugador (m->j);
		
		break;

	case 'p':
		// entrar o salir de modo pausa
		if (pausa)
		{
			
			pausa = FALSE;
			glutSetCursor (GLUT_CURSOR_CROSSHAIR);
			glutWarpPointer (TAMANYOVENTANA / 2, TAMANYOVENTANA / 2);

		}
		else
			if (m->j->vivo && m->lista)
				pausa = TRUE;
		
		break;
	
	// cambiar flag de dibujar o no de esfera de colision
	case 'c':
		dibujarEsfera = !dibujarEsfera;
		break;
	
	// ajustar sensibilidad del raton
	case 'v':
		if (sensibilidad > 0.1)
			sensibilidad -= 0.1;
		break;
	
	case 'b':
		sensibilidad += 0.1;
		break;

	// ajustar dificultad
	case 'n':
		// cambiar dificultad
		if (dificultad <= 0)
			// subir dificultad
			dificultad++;
		else
			// volver al nivel inicial si se ha superado el maximo
			dificultad = 0;
		
		// matar al jugador para reiniciar la partida
		m->j->vivo = FALSE;

		// reproducir sonido de muerte
		alSourcefv (son->fuentePierdes, AL_POSITION, m->j->posicion);
		alSourcePlay (son->fuentePierdes);
		
		break;

	case 'k':
		// matar al jugador para reiniciar la partida
		m->j->vivo = FALSE;

		// reproducir sonido de muerte
		if (reproducir)
		{
			
			alSourcefv (son->fuentePierdes, AL_POSITION, m->j->posicion);
			alSourcePlay (son->fuentePierdes);

		}

		break;
		
	// sonido
	case 'm':
		reproducir = !reproducir;
		break;
		
	// mover
	case 'w':
		if (m->j->avanzar == 0.0)
			avanzarJugador (m->j);
		break;
	
	case 's':
		if (m->j->avanzar == 0.0)
			retrocederJugador (m->j);
		break;
	
	case 'a':
		if (m->j->lateral == 0.0)
			izquierdaJugador (m->j);
		break;

	case 'd':
		if (m->j->lateral == 0.0)
			derechaJugador (m->j);
		break;
	
	default:
		break;

	}

	glutPostRedisplay();

}

/*****************************************************************************/

void keyboardUp (GLubyte key, GLint x, GLint y)
{

	switch (key)
	{

	case 'w':
		pararAvanzarJugador (m->j);
		break;
	
	case 's':
		pararRetrocederJugador (m->j);
		break;
	
	case 'a':
		pararIzquierdaJugador (m->j);
		break;

	case 'd':
		pararDerechaJugador (m->j);
		break;
	
	default:
		break;

	}
	
	glutPostRedisplay();

}

/*****************************************************************************/

void special (GLint key, GLint x, GLint y)
{

	switch (key)
	{

	case GLUT_KEY_F1:
		posicionCamara = 0;
		break;

	case GLUT_KEY_F2:
		posicionCamara = 1;
		break;

	case GLUT_KEY_F3:
		posicionCamara = 2;
		break;

	case GLUT_KEY_F4:
		posicionCamara = 3;
		break;

	case GLUT_KEY_F5:
		posicionCamara = 4;
		break;
	
	case GLUT_KEY_UP:
		break;
	
	case GLUT_KEY_DOWN:
		break;
	
	case GLUT_KEY_LEFT:
		break;

	case GLUT_KEY_RIGHT:
		break;

	default:
		break;

	}

	glutPostRedisplay();

}

/*****************************************************************************/

void specialUp (GLint key, GLint x, GLint y)
{

	switch (key)
	{

	case GLUT_KEY_UP:
		break;

	case GLUT_KEY_DOWN:
		break;
	
	case GLUT_KEY_LEFT:
		break;

	case GLUT_KEY_RIGHT:
		break;

	default:
		break;

	}

	glutPostRedisplay();

}

/*****************************************************************************/

void mouse (GLint button, GLint state, GLint x, GLint y)
{
	
	switch (button)
	{

	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
		
			if (m->j->vivo && m->lista && !pausa)
				dispararJugador (m->j);
		
		}

		break;
		
	case GLUT_RIGHT_BUTTON:		
		if (state == GLUT_DOWN)
		{
		
			if (!pausa)
				saltarJugador (m->j);
		
		}
			
		break;

	default:
		break;

	}

	glutPostRedisplay();

}
