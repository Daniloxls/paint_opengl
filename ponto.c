//
// Created by danil on 23/09/2024.
//

#include <GL/glut.h>
#include "ponto.h"

void addPoint(int x, int y, int window_height, GLclampf *current_color, PointNode** pointList){
    Ponto novoPonto;
    novoPonto.x = x;
    novoPonto.y = window_height - y;
    novoPonto.color[0] = current_color[0];
    novoPonto.color[1] = current_color[1];
    novoPonto.color[2] = current_color[2];

    PointNode *insertPoint = (PointNode *) malloc(sizeof(PointNode));
    insertPoint->val = novoPonto;
    insertPoint->next = NULL;

    if (*pointList) {
        PointNode *temp = *pointList;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = insertPoint;
    } else {
        *pointList = insertPoint;
    }
}

int checkPointClick(Ponto ponto, int mouse_x, int mouse_y, int window_height, int tolerancia){
    int y = window_height - mouse_y;
    int x = mouse_x;
    if((x - tolerancia <= ponto.vertice.x && ponto.vertice.x <= x + tolerancia) &&
       (y - tolerancia <= ponto.vertice.y && ponto.vertice.y <= y + tolerancia)){
        return 1;
    }
    return 0;
}

void printSinglePoint(Ponto ponto){
    glColor3f(ponto.color[0], ponto.color[1], ponto.color[2]);
    glBegin(GL_POINTS);
    glVertex2i(ponto.vertice.x,ponto.vertice.y);
    glEnd();
}

void printPoints(PointNode* pointList){
    if (pointList){
        PointNode* temp = pointList;
        while(temp){
            printSinglePoint(temp->val);
            temp = temp->next;
        }
    }
}

