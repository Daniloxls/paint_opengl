#ifndef TRANSFORMACOES_H
#define TRANSFORMACOES_H

#include <GL/glut.h>
#include "poligono.h"
#include "ponto.h"

void desenharSelecao(Vertice *, int);
void getCentro(Vertice[], int, Vertice*);
void AplicarTransf(GLfloat[3][3], Vertice*, int);
void transladar(GLfloat, GLfloat, Vertice *, int);
void rotacionar(GLfloat, Vertice *, int);
void escalar(GLfloat, GLfloat, Vertice *, int);
void cisalhamento_x(GLfloat, Vertice *, int);
void cisalhamento_y(GLfloat, Vertice *, int);
void reflexao_x(Vertice *, int);
void reflexao_y(Vertice *, int);
void reflexao_xy(Vertice *, int);

#endif //TRANSFORMACOES_H
