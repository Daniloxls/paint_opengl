#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include "transformacoes.h"

void desenharSelecao(Vertice vertices[], int qtd_vertices) {
    Vertice p1 = {.x = vertices[0].x, .y = vertices[0].y};
    Vertice p2 = {.x = vertices[0].x, .y = vertices[0].y};
    for(int i = 0; i < qtd_vertices; i++) {
        if(vertices[i].x < p1.x) p1.x = vertices[i].x;
        if(vertices[i].y > p1.y) p1.y = vertices[i].y;

        if(vertices[i].x > p2.x) p2.x = vertices[i].x;
        if(vertices[i].y < p2.y) p2.y = vertices[i].y;
    }

    glColor3f(0.9, 0.9, 0.9);
    glBegin(GL_LINE_LOOP);
    glVertex2i(p1.x - 10, p1.y + 10);
    glVertex2i(p2.x + 10, p1.y + 10);
    glVertex2i(p2.x + 10, p2.y - 10);
    glVertex2i(p1.x - 10, p2.y - 10);
    glEnd();
}

void getCentro(Vertice vertices[], int qtd_vertices, Vertice *c) {
    float x = 0.0;
    float y = 0.0;
    for (int i = 0; i < qtd_vertices; i++) {
        x += vertices[i].x;
        y += vertices[i].y;
    }

    c->x = x / qtd_vertices;
    c->y = y / qtd_vertices;
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

void escalar(GLfloat ex, GLfloat ey, Vertice *vertices, int qtd_vertices) {
    GLfloat matriz[3][3] = {
        {ex, 0.0, 0.0},
        {0.0, ey, 0.0},
        {0.0, 0.0, 1.0}
    };

    AplicarTransf(matriz, vertices, qtd_vertices);

}

void rotacionar(GLfloat angulo, Vertice *vertices, int qtd_vertices) {
    GLfloat c = cos(angulo * M_PI / 180.0); // calcula o cosseno
    GLfloat s = sin(angulo * M_PI / 180.0); // calcula o seno

    GLfloat matriz[3][3] = {
        {c, -s, 0.0},
        {s, c, 0.0},
        {0.0, 0.0, 1.0}
    };

    AplicarTransf(matriz, vertices, qtd_vertices);
}

void cisalhamento_x(GLfloat c, Vertice *vertices, int qtd_vertices) {
    GLfloat matriz[3][3] = {
        {1.0, c, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}
    };

    AplicarTransf(matriz, vertices, qtd_vertices);
}

void cisalhamento_y(GLfloat c, Vertice *vertices, int qtd_vertices) {
    GLfloat matriz[3][3] = {
        {1.0, 0.0, 0.0},
        {c, 1.0, 0.0},
        {0.0, 0.0, 1.0}
    };

    AplicarTransf(matriz, vertices, qtd_vertices);
}

