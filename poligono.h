#ifndef BANDEIRA_POLIGONO_H
#define BANDEIRA_POLIGONO_H

#include <GL/glut.h>
#include "state.h"
#include "ponto.h"
#include "linha.h"
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
void PrintPolygons(PolygonNode *p, enum State current_state, Poligono currentPolygon, GLclampf *current_color, int mouse_x, int mouse_y) ;
int checkPoligonoClick(Poligono poligono, int mouse_x, int mouse_y, int window_height, int tolerancia, PointNode** pointList);

#endif //BANDEIRA_POLIGONO_H
