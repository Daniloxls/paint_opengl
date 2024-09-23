#ifndef LINHA_H
#define LINHA_H

#include <GL/glut.h>
#include "state.h"

typedef struct linha{
    int coords[2][2];
    GLclampf color[3];
} Linha ;

typedef struct lines{
    struct linha val;
    struct lines *next;
} LineNode ;

void printSingleLine(Linha linha);
void printLines(enum State current_state, Linha currentLine, LineNode* lineList, int mouse_x, int mouse_y, int window_height);
int checkLineClick(Linha linha, int mouse_x, int mouse_y, int window_height, int tolerancia);
#endif