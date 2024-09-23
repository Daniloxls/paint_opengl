//
// Created by danil on 23/09/2024.
//

#include "poligono.h"
#include "linha.h"

void AddPolygon(PolygonNode **lista, Poligono poligono) {
    PolygonNode *novoPoligono = (PolygonNode *)malloc(sizeof(PolygonNode));
    novoPoligono->poligono = poligono;
    novoPoligono->next = *lista;
    *lista = novoPoligono;
}

void PrintPolygons(PolygonNode *p, enum State current_state, Poligono currentPolygon) {
    if(current_state == DRAWING_POLYGON) {
        glLineWidth(5.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2i(currentPolygon.vertices[0].x, currentPolygon.vertices[0].y);
        for (int i = 1; i < currentPolygon.qtd_Vertices - 1; i++) {
            glVertex2i(currentPolygon.vertices[i].x, currentPolygon.vertices[i].y);
            glVertex2i(currentPolygon.vertices[i].x, currentPolygon.vertices[i].y);
        }
        glVertex2i(currentPolygon.vertices[currentPolygon.qtd_Vertices-1].x, currentPolygon.vertices[currentPolygon.qtd_Vertices-1].y);
        glEnd();
        glLineWidth(1.0f);
    }

    PolygonNode *temp = p;
    while (temp != NULL) {
        if(temp == NULL) break;
        glColor3f(p->poligono.color[0], p->poligono.color[1], p->poligono.color[2]);
        glBegin(GL_POLYGON);
        for (int i = 0; i < temp->poligono.qtd_Vertices; i++) {
            // printf("[%d, %d]\n", temp->poligono.vertices[i].x, temp->poligono.vertices[i].y);
            glVertex2i(temp->poligono.vertices[i].x, temp->poligono.vertices[i].y);
        }
        glEnd();
        temp = temp->next;
    }
}

int checkPoligonoClick(Poligono poligono, int mouse_x, int mouse_y, int window_height, int tolerancia){
    int acertos = 0;
    int y = window_height - mouse_y;
    for (int i = window_height - mouse_y; i <= window_height; i++ ){
        for(int j = 0; j < poligono.qtd_Vertices - 1; j++){
            Linha aresta;
            aresta.coords[0][0] = poligono.vertices[j].x;
            aresta.coords[0][1] = poligono.vertices[j].y;
            aresta.coords[1][0] = poligono.vertices[j + 1].x;
            aresta.coords[1][1] = poligono.vertices[j + 1].y;
            acertos += checkLineClick(aresta, mouse_x, i, window_height, 0);
        }
    }
    return acertos % 2;
}