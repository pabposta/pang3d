#ifndef __CAMARA_H
#define __CAMARA_H

typedef struct _camara {
	GLfloat posicion[3];
	GLfloat orientacion[3];
} camara;

/*****************************************************************************/

camara *crearCamara (void);
void destruirCamara (camara *c);
void moverCamara (camara *c);


#endif