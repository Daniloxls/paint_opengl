//
// Created by danil on 23/09/2024.
//
#include "state.h"

#ifndef PONTO_H
#define PONTO_H

typedef struct ponto{
    int x;
    int y;
    GLclampf color[3];
} Ponto ;



typedef struct points {
    struct ponto val;
    struct points *next;
} PointNode ;

int checkPointClick(Ponto ponto, int mouse_x, int mouse_y, int window_height, int tolerancia);
void printSinglePoint(Ponto ponto);
void printPoints(PointNode* pointList);

#endif