//
// Created by danil on 23/09/2024.
//

#include <stdio.h>
#include "poligono.h"
#include "linha.h"
#include "ponto.h"
#include <GL/gl.h>   // OpenGL header
#include <GL/glu.h>

// Callback functions with the correct signature
void CALLBACK tessBeginCallback(GLenum type) {
    glBegin(type);
}

void CALLBACK tessVertexCallback(GLvoid *vertex) {
    const GLdouble *pointer = (GLdouble *)vertex;
    glVertex2dv(pointer);
}

void CALLBACK tessEndCallback() {
    glEnd();
}


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
        glColor3f(temp->poligono.color[0], temp->poligono.color[1], temp->poligono.color[2]);

        GLUtesselator *tess = gluNewTess();
        gluTessCallback(tess, GLU_TESS_BEGIN, (void (CALLBACK *)())tessBeginCallback);
        gluTessCallback(tess, GLU_TESS_VERTEX, (void (CALLBACK *)())tessVertexCallback);
        gluTessCallback(tess, GLU_TESS_END, (void (CALLBACK *)())tessEndCallback);

        gluTessBeginPolygon(tess, NULL);
        gluTessBeginContour(tess);

        Vertice* tempvertice = temp->poligono.vertices;
        while(tempvertice != NULL){
            GLdouble *vertex = (GLdouble *)malloc(3 * sizeof(GLdouble));
            if (vertex == NULL) {
                gluDeleteTess(tess);
                return;
            }
            vertex[0] = tempvertice->x;
            vertex[1] = tempvertice->y;
            vertex[2] = 0;
            gluTessVertex(tess, vertex, vertex);
            tempvertice = tempvertice->next;
        }

        gluTessEndContour(tess);
        gluTessEndPolygon(tess);
        gluDeleteTess(tess);

        temp = temp->next;
    }

    if(current_state == DRAWING_POLYGON) {
        glColor3f(current_color[0], current_color[1], current_color[2]);
        glLineWidth(5.0f);
        glBegin(GL_LINES);
        glVertex2i(currentPolygon.vertices->x, currentPolygon.vertices->y);
        Vertice* temp = currentPolygon.vertices->next;
        while(temp){
            glVertex2i(temp->x, temp->y);
            glVertex2i(temp->x, temp->y);
            temp = temp->next;
        }
        glVertex2i(mouse_x, mouse_y);
        glEnd();
        glLineWidth(1.0f);
    }


}

int checkPoligonoClick(Poligono poligono, int mouse_x, int mouse_y, int window_height, int tolerancia, PointNode** pointList){

    return 0;
}

