#ifndef __JUGADOR_H
#define __JUGADOR_H

#include "practica02.h"

typedef struct _jugador {
	GLfloat posicion[3];
	GLfloat angulo[3];	
	GLfloat orientacion[3];
	GLfloat orientacionVista[3];
	GLfloat avanzar;
	GLfloat lateral;
	GLfloat velocidad;
	GLfloat radio;
	bool vivo;
} jugador;

/*****************************************************************************/

jugador *crearJugador (GLfloat *posicion, GLfloat *angulo);
void destruirJugador (jugador *j);
void moverJugador (jugador *j);

// funciones para el control del jugador por el usuario
void avanzarJugador (jugador *j);
void retrocederJugador (jugador *j);
void izquierdaJugador (jugador *j);
void derechaJugador (jugador *j);
void pararAvanzarJugador (jugador *j);
void pararRetrocederJugador (jugador *j);
void pararIzquierdaJugador (jugador *j);
void pararDerechaJugador (jugador *j);
void saltarJugador (jugador *j);
void dispararJugador (jugador *j);
void dibujarJugador (jugador *j);
GLubyte saltandoJugador (jugador *j);


#endif