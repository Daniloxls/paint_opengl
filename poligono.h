//
// Created by danil on 23/09/2024.
//

#ifndef BANDEIRA_POLIGONO_H
#define BANDEIRA_POLIGONO_H

#include <GL/glut.h>
#include "state.h"
typedef struct {
    int x;
    int y;
} Vertice;

typedef struct {
    int qtd_Vertices;
    Vertice *vertices;
    GLclampf color[3];
} Poligono;

struct polygonElement {
    Poligono poligono;
    struct polygonElement *next;
};

typedef struct polygonElement PolygonNode;

void AddPolygon(PolygonNode **lista, Poligono poligono);
void PrintPolygons(PolygonNode *p, enum State current_state, Poligono currentPolygon);
int checkPoligonoClick(Poligono poligono, int mouse_x, int mouse_y, int window_height, int tolerancia);
#endif //BANDEIRA_POLIGONO_H
