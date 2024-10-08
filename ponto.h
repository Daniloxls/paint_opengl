#include "state.h"

#ifndef PONTO_H
#define PONTO_H

typedef struct vertice{
    GLfloat x;
    GLfloat y;
    struct vertice *next;
} Vertice;

typedef struct ponto{
    Vertice vertice;
    GLclampf color[3];
} Ponto ;

typedef struct points {
    struct ponto val;
    struct points *next;
} PointNode ;

int checkPointClick(Ponto ponto, int mouse_x, int mouse_y, int window_height, int tolerancia);
void printSinglePoint(Ponto ponto);
void printPoints(PointNode* pointList);
void addPoint(float x, float y, float window_height, GLclampf *current_color, PointNode** pointList);
#endif

