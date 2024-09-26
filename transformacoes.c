#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include "transformacoes.h"

void desenharSelecao(Vertice *vertices, int qtd_vertices, int isPolygon) {
    Vertice p1 = {.x = vertices[0].x, .y = vertices[0].y};
    Vertice p2 = {.x = vertices[0].x, .y = vertices[0].y};
    
    if(!isPolygon) {
        for(int i = 0; i < qtd_vertices; i++) {
            if(vertices[i].x < p1.x) p1.x = vertices[i].x;
            if(vertices[i].y > p1.y) p1.y = vertices[i].y;

            if(vertices[i].x > p2.x) p2.x = vertices[i].x;
            if(vertices[i].y < p2.y) p2.y = vertices[i].y;
        }
    } else {
        Vertice* tempvertice = vertices;
        while(tempvertice != NULL){
            if(tempvertice == NULL) break;
            
            if(tempvertice->x < p1.x) p1.x = tempvertice->x;
            if(tempvertice->y > p1.y) p1.y = tempvertice->y;

            if(tempvertice->x > p2.x) p2.x = tempvertice->x;
            if(tempvertice->y < p2.y) p2.y = tempvertice->y;

            tempvertice = tempvertice->next;
        }

    }

    glColor3f(0.9, 0.9, 0.9);
    glBegin(GL_LINE_LOOP);
    glVertex2i(p1.x - 10, p1.y + 10);
    glVertex2i(p2.x + 10, p1.y + 10);
    glVertex2i(p2.x + 10, p2.y - 10);
    glVertex2i(p1.x - 10, p2.y - 10);
    glEnd();
}

void getCentro(Vertice vertices[], int qtd_vertices, Vertice *c, int isPolygon) {
    float x = 0.0;
    float y = 0.0;

    if(!isPolygon) {
        for (int i = 0; i < qtd_vertices; i++) {
            x += vertices[i].x;
            y += vertices[i].y;
        }
    } else {
        Vertice* tempvertice = vertices;
        while(tempvertice != NULL){
            if(tempvertice == NULL) break;
            x += tempvertice->x;
            y += tempvertice->y;

            tempvertice = tempvertice->next;
        }
    }

    c->x = x / qtd_vertices;
    c->y = y / qtd_vertices;
}

void AplicarTransf(GLfloat matriz[3][3], Vertice *vertices, int qtd_vertices, int isPolygon) {
    if(!isPolygon) {
        for (int i = 0; i < qtd_vertices; i++) {
            GLfloat temp_x = vertices[i].x;
            GLfloat temp_y = vertices[i].y;

            vertices[i].x = matriz[0][0] * temp_x + matriz[0][1] * temp_y + matriz[0][2] * 1;
            vertices[i].y = matriz[1][0] * temp_x + matriz[1][1] * temp_y + matriz[1][2] * 1;
        }
    } else {
        Vertice* tempvertice = vertices;
        while(tempvertice != NULL){
            if(tempvertice == NULL) break;
            GLfloat temp_x = tempvertice->x;
            GLfloat temp_y = tempvertice->y;

            tempvertice->x = matriz[0][0] * temp_x + matriz[0][1] * temp_y + matriz[0][2] * 1;
            tempvertice->y = matriz[1][0] * temp_x + matriz[1][1] * temp_y + matriz[1][2] * 1;

            tempvertice = tempvertice->next;
        }
    }
}

void transladar(GLfloat tx, GLfloat ty, Vertice *vertices, int qtd_vertices, int isPolygon) {
    GLfloat matriz[3][3] = {
        {1.0, 0.0, tx},
        {0.0, 1.0, ty},
        {0.0, 0.0, 1.0}
    };

    AplicarTransf(matriz, vertices, qtd_vertices, isPolygon);
}

void escalar(GLfloat ex, GLfloat ey, Vertice *vertices, int qtd_vertices, int isPolygon) {
    GLfloat matriz[3][3] = {
        {ex, 0.0, 0.0},
        {0.0, ey, 0.0},
        {0.0, 0.0, 1.0}
    };

    AplicarTransf(matriz, vertices, qtd_vertices, isPolygon);

}

void rotacionar(GLfloat angulo, Vertice *vertices, int qtd_vertices, int isPolygon) {
    GLfloat c = cos(angulo * M_PI / 180.0); // calcula o cosseno
    GLfloat s = sin(angulo * M_PI / 180.0); // calcula o seno

    GLfloat matriz[3][3] = {
        {c, -s, 0.0},
        {s, c, 0.0},
        {0.0, 0.0, 1.0}
    };

    AplicarTransf(matriz, vertices, qtd_vertices, isPolygon);
}

void cisalhamento_x(GLfloat c, Vertice *vertices, int qtd_vertices, int isPolygon) {
    GLfloat matriz[3][3] = {
        {1.0, c, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}
    };

    AplicarTransf(matriz, vertices, qtd_vertices, isPolygon);
}

void cisalhamento_y(GLfloat c, Vertice *vertices, int qtd_vertices, int isPolygon) {
    GLfloat matriz[3][3] = {
        {1.0, 0.0, 0.0},
        {c, 1.0, 0.0},
        {0.0, 0.0, 1.0}
    };

    AplicarTransf(matriz, vertices, qtd_vertices, isPolygon);
}

void reflexao_x(Vertice *vertices, int qtd_vertices, int isPolygon) {
    GLfloat matriz[3][3] = {
        {1.0, 0.0, 0.0},
        {0.0, -1.0, 0.0},
        {0.0, 0.0, 1.0}
    };

    AplicarTransf(matriz, vertices, qtd_vertices, isPolygon);
}


void reflexao_y(Vertice *vertices, int qtd_vertices, int isPolygon) {
    GLfloat matriz[3][3] = {
        {-1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}
    };

    AplicarTransf(matriz, vertices, qtd_vertices, isPolygon);
}

void reflexao_xy(Vertice *vertices, int qtd_vertices, int isPolygon) {
    GLfloat matriz[3][3] = {
        {-1.0, 0.0, 0.0},
        {0.0, -1.0, 0.0},
        {0.0, 0.0, 1.0}
    };

    AplicarTransf(matriz, vertices, qtd_vertices, isPolygon);
}
