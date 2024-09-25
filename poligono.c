//
// Created by danil on 23/09/2024.
//

#include <stdio.h>
#include "poligono.h"
#include "linha.h"
#include "ponto.h"

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

int checkPoligonoClick(Poligono poligono, int mouse_x, int mouse_y, int window_height, int tolerancia, PointNode** pointList){

    int acertos = 0;
    GLclampf point_color[3] = {1.0f, 0.0f, 0.0f};
    printf("x: %d\n y: %d\n height:%d\n ", mouse_x, mouse_y, window_height);

    for(int j = 0; j < poligono.qtd_Vertices - 1; j++){
        Linha aresta;
        aresta.coords[0][0] = poligono.vertices[j].x;
        aresta.coords[0][1] = poligono.vertices[j].y;
        aresta.coords[1][0] = poligono.vertices[j + 1].x;
        aresta.coords[1][1] = poligono.vertices[j + 1].y;
        int consty = (2*poligono.vertices[j].y) - window_height;
        addPoint(poligono.vertices[j].x, consty, window_height, point_color, pointList);
        for (int i = window_height - mouse_y; i <= window_height; i++ ){
            if(checkLineClick(aresta, mouse_x, i, window_height, 10)){
                acertos ++;
                break;
            }
        }
    }
    Linha aresta;
    aresta.coords[0][0] = poligono.vertices[poligono.qtd_Vertices].x;
    aresta.coords[0][1] = poligono.vertices[poligono.qtd_Vertices].y;
    aresta.coords[1][0] = poligono.vertices[0].x;
    aresta.coords[1][1] = poligono.vertices[0].y;
    int consty = (2*poligono.vertices[poligono.qtd_Vertices].y) - window_height;
    addPoint(poligono.vertices[poligono.qtd_Vertices].x, consty, window_height, point_color, pointList);
    for (int i = window_height - mouse_y; i <= window_height; i++ ){
        if(checkLineClick(aresta, mouse_x, i, window_height, 10)){
            acertos ++;
            break;
        }
    }
    printf("%d\n", acertos);
    //return acertos % 2;
    return 0;
}