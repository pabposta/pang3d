#ifndef __ENTRADA_H
#define __ENTRADA_H

#include "practica02.h"

/*****************************************************************************/

void motion (GLint x, GLint y);
void keyboard (GLubyte key, GLint x, GLint y);
void keyboardUp (GLubyte key, GLint x, GLint y);
void special (GLint key, GLint x, GLint y);
void specialUp (GLint key, GLint x, GLint y);
void mouse (GLint button, GLint state, GLint x, GLint y);


#endif
