//
// Created by danil on 23/09/2024.
//

#include <GL/glut.h>
#include <stdio.h>
#include "linha.h"

void printSingleLine(Linha linha) {
    glColor3f(linha.color[0], linha.color[1], linha.color[2]);
    glBegin(GL_LINES);
    glVertex2i(linha.coords[0][0], linha.coords[0][1]);
    glVertex2i(linha.coords[1][0], linha.coords[1][1]);
    glEnd();
}

void printLines(enum State current_state, Linha currentLine, LineNode* lineList, int mouse_x, int mouse_y, int window_height) {
    if (current_state == DRAWING_LINE) {
        currentLine.coords[1][0] = mouse_x;
        currentLine.coords[1][1] = window_height - mouse_y;
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
    if (x  + tolerancia < linha.coords[0][0]){
        vertice_a += 8 ;
    }
    else if(linha.coords[0][0] < x  - tolerancia){
        vertice_a += 4 ;
    }
    if (y  + tolerancia  < linha.coords[0][1]){
        vertice_a += 2 ;
    }
    else if(linha.coords[0][1]< y  - tolerancia){
        vertice_a += 1 ;
    }

    if (x  + tolerancia < linha.coords[1][0]){
        vertice_b += 8 ;
    }
    else if(linha.coords[1][0] < x  - tolerancia){
        vertice_b += 4 ;
    }
    if (y < linha.coords[1][1] + tolerancia){
        vertice_b += 2 ;
    }
    else if(linha.coords[1][1]  < y  - tolerancia){
        vertice_b += 1 ;
    }

    if (!vertice_a || !vertice_b)
        return 1;

    if (vertice_a & vertice_b){
        return 0;
    }

    Linha new_line;

    if (vertice_a & 4){
        new_line.coords[0][0] = x  - tolerancia;
        new_line.coords[0][1] = linha.coords[0][1] + ((
                new_line.coords[0][0] - linha.coords[0][0]
                ) * (linha.coords[1][1] - linha.coords[0][1])) / (linha.coords[1][0] - linha.coords[0][0]);

        return checkLineClick(new_line,mouse_x,mouse_y, window_height, tolerancia);
    }

    if (vertice_a & 8){
        new_line.coords[0][0] = x  + tolerancia;
        new_line.coords[0][1] = linha.coords[0][1] + ((
                new_line.coords[0][0] - linha.coords[0][0]
                ) * (linha.coords[1][1] - linha.coords[0][1])) / (linha.coords[1][0] - linha.coords[0][0]);

        return checkLineClick(new_line,mouse_x,mouse_y, window_height, tolerancia);
    }

    if (vertice_a & 1){
        new_line.coords[0][1] = y  - tolerancia;
        new_line.coords[0][0] = linha.coords[0][0] + ((
                new_line.coords[0][1] - linha.coords[0][1]
                ) * (linha.coords[1][0] - linha.coords[0][0])) / (linha.coords[1][1] - linha.coords[0][1]);

        return checkLineClick(new_line,mouse_x,mouse_y, window_height, tolerancia);
    }
    if (vertice_a & 2){
        new_line.coords[0][1] = y  + tolerancia;
        new_line.coords[0][0] = linha.coords[0][0] + ((
                new_line.coords[0][1] - linha.coords[0][1]
                ) * (linha.coords[1][0] - linha.coords[0][0])) / (linha.coords[1][1] - linha.coords[0][1]);

        return checkLineClick(new_line,mouse_x,mouse_y, window_height, tolerancia);
    }

    return 0;
}
