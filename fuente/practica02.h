#ifndef __PRACTICA02_H
#define __PRACTICA02_H

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <gl/glut.h>
#include <al/al.h>
#include <al/alut.h>
#include <sdl/sdl.h>

#include "bola.h"
#include "jugador.h"
#include "pared.h"
#include "rayo.h"
#include "camara.h"
#include "puntos.h"
#include "marcas.h"
#include "mundo.h"
#include "entrada.h"

/*****************************************************************************/

#ifndef M_PI
	#define M_PI	3.14159265358979323846
#endif

#ifndef TRUE
	#define TRUE	1
#endif

#ifndef FALSE
	#define FALSE	0
#endif

#define TAMANYOVENTANA	600

/*****************************************************************************/

typedef struct _sonidos {
	ALuint bufferRayo;
	ALuint fuenteRayo;
	ALuint bufferGanas;
	ALuint fuenteGanas;
	ALuint bufferPierdes;
	ALuint fuentePierdes;
	ALuint bufferAcierto;
	ALuint fuenteAcierto;
	ALuint bufferMusica;
	ALuint fuenteMusica;
	ALuint bufferIndy;
	ALuint fuenteIndy;
	ALuint bufferRebote1;
	ALuint fuenteRebote1;
	ALuint bufferRebote2;
	ALuint fuenteRebote2;
} sonidos;


#endif