//
// Created by danil on 23/09/2024.
//

#include <GL/glut.h>
#include <stdio.h>
#include "linha.h"

void printSingleLine(Linha linha) {
    glColor3f(linha.color[0], linha.color[1], linha.color[2]);
    glBegin(GL_LINES);
    glVertex2i(linha.coords[0].x, linha.coords[0].y);
    glVertex2i(linha.coords[1].x, linha.coords[1].y);
    glEnd();
}

void printLines(enum State current_state, Linha currentLine, LineNode* lineList, int mouse_x, int mouse_y, int window_height) {
    if (current_state == DRAWING_LINE) {
        currentLine.coords[1].x = mouse_x;
        currentLine.coords[1].y = window_height - mouse_y;
        printSingleLine(currentLine);
    }
    if (lineList) {
        LineNode* temp = lineList;
        while (temp) {
            printSingleLine(temp->val);
            temp = temp->next;
        }
    }
}

int checkLineClick(Linha linha, int mouse_x, int mouse_y, int window_height, int tolerancia){
    int y = window_height - mouse_y;
    int x = mouse_x;
    unsigned int vertice_a = 0;
    unsigned int vertice_b = 0;
    if (x  + tolerancia < linha.coords[0].x){
        vertice_a += 8 ;
    }
    else if(linha.coords[0].x < x  - tolerancia){
        vertice_a += 4 ;
    }
    if (y  + tolerancia  < linha.coords[0].y){
        vertice_a += 2 ;
    }
    else if(linha.coords[0].y< y  - tolerancia){
        vertice_a += 1 ;
    }

    if (x  + tolerancia < linha.coords[1].x){
        vertice_b += 8 ;
    }
    else if(linha.coords[1].x < x  - tolerancia){
        vertice_b += 4 ;
    }
    if (y < linha.coords[1].y + tolerancia){
        vertice_b += 2 ;
    }
    else if(linha.coords[1].y  < y  - tolerancia){
        vertice_b += 1 ;
    }

    if (!vertice_a || !vertice_b)
        return 1;

    if (vertice_a & vertice_b){
        return 0;
    }

    Linha new_line;

    if (vertice_a & 4){
        new_line.coords[0].x = x  - tolerancia;
        new_line.coords[0].y = linha.coords[0].y + ((
                new_line.coords[0].x - linha.coords[0].x
                ) * (linha.coords[1].y - linha.coords[0].y)) / (linha.coords[1].x - linha.coords[0].x);

        return checkLineClick(new_line,mouse_x,mouse_y, window_height, tolerancia);
    }

    if (vertice_a & 8){
        new_line.coords[0].x = x  + tolerancia;
        new_line.coords[0].y = linha.coords[0].y + ((
                new_line.coords[0].x - linha.coords[0].x
                ) * (linha.coords[1].y - linha.coords[0].y)) / (linha.coords[1].x - linha.coords[0].x);

        return checkLineClick(new_line,mouse_x,mouse_y, window_height, tolerancia);
    }

    if (vertice_a & 1){
        new_line.coords[0].y = y  - tolerancia;
        new_line.coords[0].x = linha.coords[0].x + ((
                new_line.coords[0].y - linha.coords[0].y
                ) * (linha.coords[1].x - linha.coords[0].x)) / (linha.coords[1].y - linha.coords[0].y);

        return checkLineClick(new_line,mouse_x,mouse_y, window_height, tolerancia);
    }
    if (vertice_a & 2){
        new_line.coords[0].y = y  + tolerancia;
        new_line.coords[0].x = linha.coords[0].x + ((
                new_line.coords[0].y - linha.coords[0].y
                ) * (linha.coords[1].x - linha.coords[0].x)) / (linha.coords[1].y - linha.coords[0].y);

        return checkLineClick(new_line,mouse_x,mouse_y, window_height, tolerancia);
    }

    return 0;
}
