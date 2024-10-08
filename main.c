#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <GL/glut.h>
#include "interface.h"
#include "linha.h"
#include "ponto.h"
#include "poligono.h"
#include "transformacoes.h"

float window_height = 576.0f;
float window_width = 1024.0f;
float proportion_x = 204.8f/1024;
float proportion_y = 115.2f/576;
int mouse_x = 0;
int mouse_y = 0;
int tolerancia = 10;

Botao interfaceButtons[9];
GLclampf current_color[3] = {0.0f, 0.0f, 0.0f};
Linha currentLine;
Ponto firstPoint;
PointNode* pointList;
LineNode* lineList;
enum State current_state = NONE;

WindowBorder window_border;

PolygonNode *polygonList = NULL; // lista de poligonos
Linha currentLine;
Poligono currentPolygon;
int deleted = 0;
Ponto *pontoSelecionado = NULL;
Linha *linhaSelecionada = NULL;
Poligono *poligonoSelecionado = NULL;

int init(void){
    window_border.left = -window_width/2;
    window_border.right = window_width/2;
    window_border.top = window_height/2;
    window_border.bottom = -window_height/2;

    GLclampf Red = 1.0f, Green = 1.0f, Blue = 1.0f, Alpha = 0.0f;
    glClearColor(Red, Green, Blue, Alpha);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(window_border.left, window_border.right, window_border.bottom, window_border.top);
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(5.0f);
}

void mouse(int button, int state, int x, int y){
    if (state == GLUT_UP && button == GLUT_LEFT_BUTTON){
        if(x < 90) {
            checkRGBSelector(x + window_border.left, window_border.top - y, interfaceButtons[RGBSelector], current_color);
            for (int i = 0; i < 9; i++) {
                if (checkInterfaceClick(x + window_border.left, window_border.top - y, interfaceButtons[i])) {
                    switch (i) {
                    case LineButton: current_state = LINHA; pontoSelecionado = NULL; linhaSelecionada = NULL; poligonoSelecionado = NULL; break;
                    case PointButton: current_state = PONTO; pontoSelecionado = NULL; linhaSelecionada = NULL; poligonoSelecionado = NULL; break;
                    case PolygonButton: current_state = POLIGONO; pontoSelecionado = NULL; linhaSelecionada = NULL; poligonoSelecionado = NULL; break;
                    case ResizeButton: current_state = ESCALA; break;
                    case RotateButton: current_state = ROTACAO; break;
                    case ReflexButton: current_state = ESPELHAMENTO; break;
                    case ShearButton: current_state = CISALHAMENTO; break;

                    default: current_state = NONE; break;
                    }
                }
            }
        }
        else if (current_state == POLIGONO) {
            Vertice *verticeslist = (Vertice*)malloc(sizeof(Vertice));
            firstPoint.vertice.x = x + window_border.left;
            firstPoint.vertice.y = window_border.top - y;
            verticeslist->x = x + window_border.left;
            verticeslist->y = window_border.top - y;
            verticeslist->next = NULL;
            Poligono p;
            p.qtd_Vertices = 1;
            p.vertices = verticeslist;
            p.color[0] = current_color[0];
            p.color[1] = current_color[1];
            p.color[2] = current_color[2];
            currentPolygon = p;
            current_state = DRAWING_POLYGON;
            // printf("[%d, %d]", currentPolygon.vertices[currentPolygon.qtd_Vertices - 1].x, currentPolygon.vertices[currentPolygon.qtd_Vertices - 1].y);
        }
        else if (current_state == DRAWING_POLYGON) {
            if(checkPointClick(firstPoint, x + window_border.left, y, window_border.top, 10)){
                AddPolygon(&polygonList, currentPolygon);
                Poligono p;
                currentPolygon = p;
                current_state = POLIGONO;
            }
            else{
                currentLine.coords[1].x = x + window_border.left;
                currentLine.coords[1].y = window_border.top - y;
                currentPolygon.qtd_Vertices += 1;

                Vertice *newVertice = (Vertice*)malloc(sizeof(Vertice));
                newVertice->x = x + window_border.left;
                newVertice->y = window_border.top - y;

                Vertice* temp = currentPolygon.vertices;
                Vertice* back = NULL;
                while(temp){
                    back = temp;
                    temp = temp->next;
                }
                newVertice->next = NULL;
                back->next = newVertice;

            }



            // printf("[%d, %d]", currentPolygon.vertices[currentPolygon.qtd_Vertices - 1].x, currentPolygon.vertices[currentPolygon.qtd_Vertices - 1].y);
        }
        else if (current_state == DRAWING_LINE){
            currentLine.coords[1].x = x + window_border.left;
            currentLine.coords[1].y = window_border.top - y;
            Linha novaLinha = currentLine;
            if (lineList){
                LineNode* temp = lineList;
                LineNode* back = NULL;
                while(temp){
                    back = temp;
                    temp = temp->next;
                }
                LineNode* insertLine = (LineNode*)malloc(sizeof(LineNode));
                insertLine->val = novaLinha;
                insertLine->next = NULL;
                back->next = insertLine;
                current_state = LINHA;
            }
            else{
                lineList = (LineNode*)malloc(sizeof(LineNode));
                lineList->val = novaLinha;
                lineList->next = NULL;
                current_state = LINHA;
            }
        }
        else if (current_state == LINHA){
            currentLine.coords[0].x = x + window_border.left;
            currentLine.coords[0].y = window_border.top - y;
            currentLine.color[0] = current_color[0];
            currentLine.color[1] = current_color[1];
            currentLine.color[2] = current_color[2];
            current_state = DRAWING_LINE;
        }

    }
    if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON){
        if(current_state == NONE) {
            if (pointList != NULL) {
                PointNode* temp = pointList;
                PointNode* back = NULL;

                while (temp) {
                    if (checkPointClick(temp->val, x + window_border.left, y, window_border.top, tolerancia)) {
                        pontoSelecionado = temp;
                        linhaSelecionada = NULL;
                        poligonoSelecionado = NULL;
                        break;
                    } else {
                        back = temp;
                        temp = temp->next;
                    }
                    if(x > 90) pontoSelecionado = NULL;
                }
            }
            if (lineList != NULL) {
                LineNode* temp = lineList;
                LineNode* back = NULL;

                while (temp) {
                    if (checkLineClick(temp->val, x + window_border.left, y, window_border.top, tolerancia)) {
                        pontoSelecionado = NULL;
                        linhaSelecionada = temp;
                        poligonoSelecionado = NULL;
                        break;

                    } else {
                        back = temp;
                        temp = temp->next;
                    }
                    if(x > 90) linhaSelecionada = NULL;
                }
            }
            if ((polygonList != NULL)) {
                PolygonNode* temp = polygonList;
                PolygonNode* back = NULL;

                while (temp) {
                    if (checkPoligonoClick(temp->poligono, x + window_border.left, y, window_border.top, tolerancia, &pointList)) {
                        pontoSelecionado = NULL;
                        linhaSelecionada = NULL;
                        poligonoSelecionado = temp;
                        break;
                    } else {
                        back = temp;
                        temp = temp->next;
                    }
                    if(x > 90) poligonoSelecionado = NULL;
                }
            }
        }
        if(current_state == PONTO){
            addPoint(x + window_border.left, y, window_border.top, current_color, &pointList);

        }

    }
    if (state == GLUT_DOWN && button == GLUT_MIDDLE_BUTTON){
        if(poligonoSelecionado != NULL){
            fechoConvexo(&polygonList, poligonoSelecionado);
        }
    }
    if (state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON){
        if (pointList != NULL) {
            PointNode* temp = pointList;
            PointNode* back = NULL;

            while (temp) {
                if (checkPointClick(temp->val, x + window_border.left, y, window_border.top, tolerancia)) {
                    pontoSelecionado = NULL;
                    PointNode* nextNode = temp->next;
                    if (back == NULL) {
                        pointList = nextNode;
                    } else {
                        back->next = nextNode;
                    }
                    free(temp);
                    temp = nextNode;
                    deleted = 1;
                    break;
                } else {
                    back = temp;
                    temp = temp->next;
                }
            }
        }
        if (lineList != NULL && !(deleted)) {
            LineNode* temp = lineList;
            LineNode* back = NULL;

            while (temp) {
                if (checkLineClick(temp->val, x + window_border.left, y, window_border.top, tolerancia)) {
                    linhaSelecionada = NULL;
                    LineNode* nextNode = temp->next;
                    if (back == NULL) {
                        lineList = nextNode;
                    } else {
                        back->next = nextNode;
                    }
                    free(temp);
                    temp = nextNode;
                    deleted = 1;
                    break;
                } else {
                    back = temp;
                    temp = temp->next;
                }
            }
        }
        if (polygonList != NULL && !(deleted)) {
            PolygonNode* temp = polygonList;
            PolygonNode* back = NULL;

            while (temp) {
                if (checkPoligonoClick(temp->poligono, x + window_border.left, y, window_border.top, tolerancia, &pointList)) {
                    PolygonNode* nextNode = temp->next;
                    if (back == NULL) {
                        polygonList = nextNode;
                    } else {
                        back->next = nextNode;
                    }
                    free(temp);
                    temp = nextNode;
                    break;
                } else {
                    back = temp;
                    temp = temp->next;
                }
            }
        }
        deleted = 0;
    }
}

void reshape(int newWidth, int newHeight){
    window_width = newWidth;
    window_height = newHeight;

    window_border.left = -newWidth/2;
    window_border.right = newWidth/2;
    window_border.top = newHeight/2;
    window_border.bottom = -newHeight/2;

    interfaceButtons[PolygonButton].x = 10 + window_border.left;
    interfaceButtons[PointButton].x = 50 + window_border.left;
    interfaceButtons[LineButton].x = 10 + window_border.left;
    interfaceButtons[SelectButton].x = 50 + window_border.left;
    interfaceButtons[RotateButton].x = 10 + window_border.left;
    interfaceButtons[ResizeButton].x = 50 + window_border.left;
    interfaceButtons[ReflexButton].x = 10 + window_border.left;
    interfaceButtons[ShearButton].x = 50 + window_border.left;
    interfaceButtons[RGBSelector].x = 10 + window_border.left;

    interfaceButtons[PolygonButton].y = window_border.top - 10;
    interfaceButtons[PointButton].y = window_border.top - 10;
    interfaceButtons[LineButton].y = window_border.top - 50;
    interfaceButtons[SelectButton].y = window_border.top - 50;
    interfaceButtons[RotateButton].y = window_border.top - 90;
    interfaceButtons[ResizeButton].y = window_border.top - 90;
    interfaceButtons[ReflexButton].y = window_border.top - 130;
    interfaceButtons[ShearButton].y = window_border.top - 130;
    interfaceButtons[RGBSelector].y = window_border.top - 170;

    glViewport( 0, 0, newWidth, newHeight );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( window_border.left,  window_border.right, window_border.bottom, window_border.top);
}

void mouseMove(int x, int y){
    mouse_x = x;
    mouse_y = y;
}

void teclado(unsigned char key, int x, int y){
    if(current_state == DRAWING_POLYGON && key == 13 && currentPolygon.qtd_Vertices > 2) {
        AddPolygon(&polygonList, currentPolygon);
        Poligono p;
        currentPolygon = p;
        current_state = POLIGONO;
    }
    // operações do ponto
    else if (current_state == NONE && pontoSelecionado != NULL) {
        switch(key){
            case 97: // A
                transladar(-5, 0, &pontoSelecionado->vertice, 1, 0);
                break;
            case 100: // D
                transladar(5, 0, &pontoSelecionado->vertice, 1, 0);
                break;
            case 119: // W
                transladar(0, 5, &pontoSelecionado->vertice, 1, 0);
                break;
            case 115: // S
                transladar(0, -5, &pontoSelecionado->vertice, 1, 0);
                break;
            default: break;
        };
    } else if (current_state == ROTACAO && pontoSelecionado != NULL) {
        switch(key){
            case 97: // A
                rotacionar(1, &pontoSelecionado->vertice, 1, 0);
                break;
            case 100: // D
                rotacionar(-1, &pontoSelecionado->vertice, 1, 0);
                break;
            default: break;
        };
    } else if (current_state == ESPELHAMENTO && pontoSelecionado != NULL) {
        switch(key){
            case 119: // W
                reflexao_x(&pontoSelecionado->vertice, 1, 0);
                break;
            case 115: // S
                reflexao_x(&pontoSelecionado->vertice, 1, 0);
                break;
            case 97: // A
                reflexao_y(&pontoSelecionado->vertice, 1, 0);
                break;
            case 100: // D
                reflexao_y(&pontoSelecionado->vertice, 1, 0);
                break;
            case 113: // Q
                reflexao_xy(&pontoSelecionado->vertice, 1, 0);
                break;
            case 101: // E
                reflexao_xy(&pontoSelecionado->vertice, 1, 0);
                break;
            default: break;
        };
    }
    // operações da linha
    else if (current_state == NONE && linhaSelecionada != NULL) {
        Vertice centro;
        getCentro(&linhaSelecionada->coords, 2, &centro, 0);
        switch(key){
            case 97: // A
                transladar(-5, 0, &linhaSelecionada->coords, 2, 0);
                break;
            case 100: // D
                transladar(5, 0, &linhaSelecionada->coords, 2, 0);
                break;
            case 119: // W
                transladar(0, 5, &linhaSelecionada->coords, 2, 0);
                break;
            case 115: // S
                transladar(0, -5, &linhaSelecionada->coords, 2, 0);
                break;
            default: break;
        };
    } else if (current_state == ROTACAO && linhaSelecionada != NULL) {
        Vertice centro;
        getCentro(&linhaSelecionada->coords, 2, &centro, 0);
        switch(key){
            case 97: // A
                transladar(-centro.x, -centro.y, &linhaSelecionada->coords, 2, 0);
                rotacionar(1, &linhaSelecionada->coords, 2, 0);
                transladar(centro.x, centro.y, &linhaSelecionada->coords, 2, 0);
                break;
            case 100: // D
                transladar(-centro.x, -centro.y, &linhaSelecionada->coords, 2, 0);
                rotacionar(-1, &linhaSelecionada->coords, 2, 0);
                transladar(centro.x, centro.y, &linhaSelecionada->coords, 2, 0);
                break;
            default: break;
        };
    } else if (current_state == ESCALA && linhaSelecionada != NULL) {
        Vertice centro;
        getCentro(&linhaSelecionada->coords, 2, &centro, 0);
        switch(key){
            case 119: // W
                transladar(-centro.x, -centro.y, &linhaSelecionada->coords, 2, 0);
                escalar(1.5, 1.5, &linhaSelecionada->coords, 2, 0);
                transladar(centro.x, centro.y, &linhaSelecionada->coords, 2, 0);
                break;
            case 115: // S
                transladar(-centro.x, -centro.y, &linhaSelecionada->coords, 2, 0);
                escalar(0.5, 0.5, &linhaSelecionada->coords, 2, 0);
                transladar(centro.x, centro.y, &linhaSelecionada->coords, 2, 0);
                break;
            default: break;
        };
    } else if (current_state == CISALHAMENTO && linhaSelecionada != NULL) {
        Vertice centro;
        getCentro(&linhaSelecionada->coords, 2, &centro, 0);
        switch(key){
            case 119: // W
                transladar(-centro.x, -centro.y, &linhaSelecionada->coords, 2, 0);
                cisalhamento_y(0.5, &linhaSelecionada->coords, 2, 0);
                transladar(centro.x, centro.y, &linhaSelecionada->coords, 2, 0);
                break;
            case 115: // S
                transladar(-centro.x, -centro.y, &linhaSelecionada->coords, 2, 0);
                cisalhamento_y(-0.5, &linhaSelecionada->coords, 2, 0);
                transladar(centro.x, centro.y, &linhaSelecionada->coords, 2, 0);
                break;
            case 97: // A
                transladar(-centro.x, -centro.y, &linhaSelecionada->coords, 2, 0);
                cisalhamento_x(-0.5, &linhaSelecionada->coords, 2, 0);
                transladar(centro.x, centro.y, &linhaSelecionada->coords, 2, 0);
                break;
            case 100: // D
                transladar(-centro.x, -centro.y, &linhaSelecionada->coords, 2, 0);
                cisalhamento_x(0.5, &linhaSelecionada->coords, 2, 0);
                transladar(centro.x, centro.y, &linhaSelecionada->coords, 2, 0);
                break;
            default: break;
        };
    } else if (current_state == ESPELHAMENTO && linhaSelecionada != NULL) {
        switch(key){
            case 119: // W
                reflexao_x(&linhaSelecionada->coords, 2, 0);
                break;
            case 115: // S
                reflexao_x(&linhaSelecionada->coords, 2, 0);
                break;
            case 97: // A
                reflexao_y(&linhaSelecionada->coords, 2, 0);
                break;
            case 100: // D
                reflexao_y(&linhaSelecionada->coords, 2, 0);
                break;
            case 113: // Q
                reflexao_xy(&linhaSelecionada->coords, 2, 0);
                break;
            case 101: // E
                reflexao_xy(&linhaSelecionada->coords, 2, 0);
                break;
            default: break;
        };
    }
    //operações do poligono
    else if (current_state == NONE && poligonoSelecionado != NULL) {
        switch(key){
            case 97: // A
                transladar(-5, 0, poligonoSelecionado->vertices, poligonoSelecionado->qtd_Vertices, 1);
                break;
            case 100: // D
                transladar(5, 0, poligonoSelecionado->vertices, poligonoSelecionado->qtd_Vertices, 1);
                break;
            case 119: // W
                transladar(0, 5, poligonoSelecionado->vertices, poligonoSelecionado->qtd_Vertices, 1);
                break;
            case 115: // S
                transladar(0, -5, poligonoSelecionado->vertices, poligonoSelecionado->qtd_Vertices, 1);
                break;
            default: break;
        };
    } else if (current_state == ROTACAO && poligonoSelecionado != NULL) {
        Vertice centro;
        getCentro(poligonoSelecionado->vertices, poligonoSelecionado->qtd_Vertices, &centro, 1);
        switch(key){
            case 97: // A
                transladar(-centro.x, -centro.y, poligonoSelecionado->vertices , poligonoSelecionado->qtd_Vertices, 1);
                rotacionar(1, poligonoSelecionado->vertices , poligonoSelecionado->qtd_Vertices, 1);
                transladar(centro.x, centro.y, poligonoSelecionado->vertices , poligonoSelecionado->qtd_Vertices, 1);
                break;
            case 100: // D
                transladar(-centro.x, -centro.y, poligonoSelecionado->vertices , poligonoSelecionado->qtd_Vertices, 1);
                rotacionar(-1, poligonoSelecionado->vertices , poligonoSelecionado->qtd_Vertices, 1);
                transladar(centro.x, centro.y, poligonoSelecionado->vertices , poligonoSelecionado->qtd_Vertices, 1);
                break;
            default: break;
        };
    } else if (current_state == ESCALA && poligonoSelecionado != NULL) {
        Vertice centro;
        getCentro(poligonoSelecionado->vertices, poligonoSelecionado->qtd_Vertices, &centro, 1);
        switch(key){
            case 119: // W
                transladar(-centro.x, -centro.y, poligonoSelecionado->vertices , poligonoSelecionado->qtd_Vertices, 1);
                escalar(1.5, 1.5, poligonoSelecionado->vertices , poligonoSelecionado->qtd_Vertices, 1);
                transladar(centro.x, centro.y, poligonoSelecionado->vertices , poligonoSelecionado->qtd_Vertices, 1);
                break;
            case 115: // S
                transladar(-centro.x, -centro.y, poligonoSelecionado->vertices , poligonoSelecionado->qtd_Vertices, 1);
                escalar(0.5, 0.5, poligonoSelecionado->vertices , poligonoSelecionado->qtd_Vertices, 1);
                transladar(centro.x, centro.y, poligonoSelecionado->vertices , poligonoSelecionado->qtd_Vertices, 1);
                break;
            default: break;
        };
    } else if (current_state == CISALHAMENTO && poligonoSelecionado != NULL) {
        Vertice centro;
        getCentro(poligonoSelecionado->vertices, poligonoSelecionado->qtd_Vertices, &centro, 1);
        switch(key){
            case 119: // W
                transladar(-centro.x, -centro.y, poligonoSelecionado->vertices , poligonoSelecionado->qtd_Vertices, 1);
                cisalhamento_y(0.5, poligonoSelecionado->vertices , poligonoSelecionado->qtd_Vertices, 1);
                transladar(centro.x, centro.y, poligonoSelecionado->vertices , poligonoSelecionado->qtd_Vertices, 1);
                break;
            case 115: // S
                transladar(-centro.x, -centro.y, poligonoSelecionado->vertices , poligonoSelecionado->qtd_Vertices, 1);
                cisalhamento_y(-0.5, poligonoSelecionado->vertices , poligonoSelecionado->qtd_Vertices, 1);
                transladar(centro.x, centro.y, poligonoSelecionado->vertices , poligonoSelecionado->qtd_Vertices, 1);
                break;
            case 97: // A
                transladar(-centro.x, -centro.y, poligonoSelecionado->vertices , poligonoSelecionado->qtd_Vertices, 1);
                cisalhamento_x(-0.5, poligonoSelecionado->vertices , poligonoSelecionado->qtd_Vertices, 1);
                transladar(centro.x, centro.y, poligonoSelecionado->vertices , poligonoSelecionado->qtd_Vertices, 1);
                break;
            case 100: // D
                transladar(-centro.x, -centro.y, poligonoSelecionado->vertices , poligonoSelecionado->qtd_Vertices, 1);
                cisalhamento_x(0.5, poligonoSelecionado->vertices , poligonoSelecionado->qtd_Vertices, 1);
                transladar(centro.x, centro.y, poligonoSelecionado->vertices , poligonoSelecionado->qtd_Vertices, 1);
                break;
            default: break;
        };
    } else if (current_state == ESPELHAMENTO && poligonoSelecionado != NULL) {
        switch(key){
            case 119: // W
                reflexao_x(poligonoSelecionado->vertices , poligonoSelecionado->qtd_Vertices, 1);
                break;
            case 115: // S
                reflexao_x(poligonoSelecionado->vertices , poligonoSelecionado->qtd_Vertices, 1);
                break;
            case 97: // A
                reflexao_y(poligonoSelecionado->vertices , poligonoSelecionado->qtd_Vertices, 1);
                break;
            case 100: // D
                reflexao_y(poligonoSelecionado->vertices , poligonoSelecionado->qtd_Vertices, 1);
                break;
            case 113: // Q
                reflexao_xy(poligonoSelecionado->vertices , poligonoSelecionado->qtd_Vertices, 1);
                break;
            case 101: // E
                reflexao_xy(poligonoSelecionado->vertices , poligonoSelecionado->qtd_Vertices, 1);
                break;
            default: break;
        };
    }
    else {
        switch(key){
            //Quando apertar P entra no modo de desenho de pontos
            case 112:
                current_state = PONTO;
                break;
            //Quando apertar L entra no modo de desenho de linhas
            case 108:
                current_state = LINHA;
                break;
            default:
                current_state = NONE;
                break;
        };
    }
}

void display(void){
    glutMouseFunc(mouse);
    glutReshapeFunc(reshape);
    glutMotionFunc(mouseMove);
    glutPassiveMotionFunc(mouseMove);
    glutKeyboardFunc(teclado);
    PrintPolygons(polygonList, current_state, currentPolygon, current_color, mouse_x + window_border.left, window_border.top - mouse_y);
    printLines(current_state, currentLine, lineList,mouse_x + window_border.left, mouse_y, window_border.top);
    printPoints(pointList);

    if(linhaSelecionada!= NULL) desenharSelecao(&linhaSelecionada->coords, 2, 0);
    else if(pontoSelecionado!= NULL) desenharSelecao(&pontoSelecionado->vertice, 1, 0);
    else if(poligonoSelecionado!= NULL) desenharSelecao(poligonoSelecionado->vertices, poligonoSelecionado->qtd_Vertices, 1);

    drawInterface(window_border, interfaceButtons, 9, current_color);
    glFlush();
    glClearColor(1.0f,1.0f,1.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE| GLUT_RGB);
    glutInitWindowSize(window_width,window_height);
    int x = 200;
    int y = 100;
    glutInitWindowPosition(x,y);
    glutCreateWindow("Paint");
    init();

    interfaceButtons[PolygonButton] = (Botao){.x = 10 + window_border.left, .y = window_border.top - 10, .size = 30, .icon = PolygonIcon};
    interfaceButtons[PointButton] = (Botao){.x = 50 + window_border.left, .y = window_border.top - 10, .size = 30, .icon = PointIcon};
    interfaceButtons[LineButton] = (Botao){.x = 10 + window_border.left, .y = window_border.top - 50, .size = 30, .icon = LineIcon};
    interfaceButtons[SelectButton] = (Botao){.x = 50 + window_border.left, .y = window_border.top - 50, .size = 30, .icon = SelectIcon};
    interfaceButtons[RotateButton] = (Botao){.x = 10 + window_border.left, .y = window_border.top - 90, .size = 30, .icon = RotateIcon};
    interfaceButtons[ResizeButton] = (Botao){.x = 50 + window_border.left, .y = window_border.top - 90, .size = 30, .icon = ReSizeIcon};
    interfaceButtons[ReflexButton] = (Botao){.x = 10 + window_border.left, .y = window_border.top - 130, .size = 30, .icon = ReflectionIcon};
    interfaceButtons[ShearButton] = (Botao){.x = 50 + window_border.left, .y = window_border.top - 130, .size = 30, .icon = ShearIcon};
    interfaceButtons[RGBSelector] = (Botao){.x = 10 + window_border.left, .y = window_border.top - 170, .size = 70, .icon = 0};

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
