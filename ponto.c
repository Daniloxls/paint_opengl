//
// Created by danil on 23/09/2024.
//

#include <GL/glut.h>
#include "ponto.h"

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

