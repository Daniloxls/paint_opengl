

#include <GL/glut.h>
#include "poligono.h"
#include "ponto.h"

#ifndef TRANSFORMACOES_H
#define TRANSFORMACOES_H

    void desenharSelecao(Vertice *, int, int);
    void getCentro(Vertice[], int, Vertice*, int);
    void AplicarTransf(GLfloat[3][3], Vertice*, int, int);
    void transladar(GLfloat, GLfloat, Vertice *, int, int);
    void rotacionar(GLfloat, Vertice *, int, int);
    void escalar(GLfloat, GLfloat, Vertice *, int, int);
    void cisalhamento_x(GLfloat, Vertice *, int, int);
    void cisalhamento_y(GLfloat, Vertice *, int, int);
    void reflexao_x(Vertice *, int, int);
    void reflexao_y(Vertice *, int, int);
    void reflexao_xy(Vertice *, int, int);

#endif //TRANSFORMACOES_H
