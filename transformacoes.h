#ifndef TRANSFORMACOES_H
#define TRANSFORMACOES_H

#include <GL/glut.h>
#include "poligono.h"
#include "ponto.h"

Vertice *getCentro(Vertice[], int);
void AplicarTransf(GLfloat[3][3], Vertice*, int);
void transladar(GLfloat, GLfloat, Vertice *, int);
void transladarPonto(int, int, Ponto*);
void rotacionarPonto(int, Ponto*);
void rotacionar();
void escalar();

#endif //TRANSFORMACOES_H
