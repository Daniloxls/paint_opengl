//
// Created by danil on 23/09/2024.
//

#include <stdio.h>
#include "poligono.h"
#include "linha.h"
#include "ponto.h"

void AddPolygon(PolygonNode **lista, Poligono poligono) {
    PolygonNode *insertpolygon = (PolygonNode *) malloc(sizeof(PolygonNode));
    insertpolygon->poligono = poligono;
    insertpolygon->next = NULL;

    if (*lista) {
        PolygonNode *temp = *lista;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = insertpolygon;
    } else {
        *lista = insertpolygon;
    }
}

void PrintPolygons(PolygonNode *p, enum State current_state, Poligono currentPolygon, GLclampf *current_color, int mouse_x, int mouse_y) {

    PolygonNode *temp = p;
    while (temp != NULL) {
        if(temp == NULL) break;
        glColor3f(temp->poligono.color[0], temp->poligono.color[1], temp->poligono.color[2]);
        glBegin(GL_POLYGON);
        for (int i = 0; i < temp->poligono.qtd_Vertices; i++) {
            // printf("[%d, %d]\n", temp->poligono.vertices[i].x, temp->poligono.vertices[i].y);
            glVertex2i(temp->poligono.vertices[i].x, temp->poligono.vertices[i].y);
        }
        glEnd();
        temp = temp->next;
    }

    if(current_state == DRAWING_POLYGON) {
        glLineWidth(5.0f);
        glColor3f(current_color[0], current_color[1], current_color[2]);
        glBegin(GL_LINES);
        glVertex2i(currentPolygon.vertices[0].x, currentPolygon.vertices[0].y);
        for (int i = 1; i < currentPolygon.qtd_Vertices - 1; i++) {
            glVertex2i(currentPolygon.vertices[i].x, currentPolygon.vertices[i].y);
            glVertex2i(currentPolygon.vertices[i].x, currentPolygon.vertices[i].y);
        }
        glVertex2i(currentPolygon.vertices[currentPolygon.qtd_Vertices-1].x, currentPolygon.vertices[currentPolygon.qtd_Vertices-1].y);
        glVertex2i(currentPolygon.vertices[currentPolygon.qtd_Vertices-1].x, currentPolygon.vertices[currentPolygon.qtd_Vertices-1].y);
        glVertex2i(mouse_x, mouse_y);
        glEnd();
        glLineWidth(1.0f);
    }


}

int checkPoligonoClick(Poligono poligono, int mouse_x, int mouse_y, int window_height, int tolerancia, PointNode** pointList){

    return 0;
}

