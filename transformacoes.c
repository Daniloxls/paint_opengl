#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include "transformacoes.h"

Vertice *getCentro(Vertice vertices[], int qtd_vertices) {
    float x = 0.0;
    float y = 0.0;
    for (int i = 0; i < qtd_vertices; i++) {
        x += vertices[i].x;
        y += vertices[i].y;
    }

    Vertice *centro;
    centro->x = x / qtd_vertices;
    centro->y = y / qtd_vertices;

    return centro;
}

void AplicarTransf(GLfloat matriz[3][3], Vertice *vertices, int qtd_vertices) {
    for (int i = 0; i < qtd_vertices; i++) {
        GLfloat temp_x = vertices[i].x;
        GLfloat temp_y = vertices[i].y;

        vertices[i].x = matriz[0][0] * temp_x + matriz[0][1] * temp_y + matriz[0][2] * 1;
        vertices[i].y = matriz[1][0] * temp_x + matriz[1][1] * temp_y + matriz[1][2] * 1;
    }
}

void transladar(GLfloat tx, GLfloat ty, Vertice *vertices, int qtd_vertices) {
    GLfloat matriz[3][3] = {
        {1.0, 0.0, tx},
        {0.0, 1.0, ty},
        {0.0, 0.0, 1.0}
    };
    AplicarTransf(matriz, vertices, qtd_vertices);
}

void transladarPonto(int tx, int ty, Ponto *p) {
    GLfloat matriz[3][3] = {
        {1, 0, tx},
        {0, 1, ty},
        {0, 0, 1}
    };

    int temp_x = p->x;
    int temp_y = p->y;

    p->x = matriz[0][0] * temp_x + matriz[0][1] * temp_y + matriz[0][2] * 1;
    p->y = matriz[1][0] * temp_x + matriz[1][1] * temp_y + matriz[1][2] * 1;

}

void rotacionarPonto(int angle, Ponto *p) {
    GLfloat cose = cos(angle * M_PI / 180);
    GLfloat sen = sin(angle * M_PI / 180);

    GLfloat matriz[3][3] = {
        {cose, -sen, 0},
        {sen, cose, 0},
        {0, 0, 1}
    };

    int temp_x = p->x;
    int temp_y = p->y;

    p->x = matriz[0][0] * temp_x + matriz[0][1] * temp_y + matriz[0][2] * 1;
    p->y = matriz[1][0] * temp_x + matriz[1][1] * temp_y + matriz[1][2] * 1;
}
