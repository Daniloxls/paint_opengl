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

typedef struct {
    Vertice vertices[0];
}PoligonoArray;

typedef struct polygonElement PolygonNode;

void AddPolygon(PolygonNode **lista, Poligono poligono);
void PrintPolygons(PolygonNode *p, enum State current_state, Poligono currentPolygon, GLclampf *current_color, int mouse_x, int mouse_y) ;
int checkPoligonoClick(Poligono poligono, int mouse_x, int mouse_y, int window_height, int tolerancia, PointNode** pointList);
Vertice* converterArray(Poligono poligono);
int quad(Vertice p);
int orientation(Vertice a, Vertice b, Vertice c);
int compare(const void* p1, const void* q1);
Vertice* merge(Vertice* a, int n1, Vertice* b, int n2, int* result_size);
Vertice* bruteHull(Vertice* a, int size, int* result_size);
Vertice* divide(Vertice* a, int size, int* result_size);
void fechoConvexo(PolygonNode **lista, Poligono *poligono);
#endif //BANDEIRA_POLIGONO_H
