#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <GL/glut.h>
#include "interface.h"
#include "linha.h"
#include "ponto.h"
#include "poligono.h"
#include "transformacoes.h"

int window_height = 576;
int window_width = 1024;
float proportion_x = 204.8f/1024;
float proportion_y = 115.2f/576;
int mouse_x = 0;
int mouse_y = 0;
int tolerancia = 10;
Botao interfaceButtons[9];
GLclampf current_color[3] = {0.0, 0.0, 0.0};
enum State current_state = NONE;

PointNode* pointList;
LineNode* lineList;
PolygonNode *polygonList = NULL; // lista de poligonos
Linha currentLine;
Poligono currentPolygon;

Ponto *pontoSelecionado = NULL;
Linha *linhaSelecionada = NULL;
Poligono *poligonoSelecionado = NULL;

int init(void){
    GLclampf Red = 1.0f, Green = 1.0f, Blue = 1.0f, Alpha = 0.0f;
    glClearColor(Red, Green, Blue, Alpha);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, window_width, 0.0, window_height);
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(20.0f);
}

void mouse(int button, int state, int x, int y){
    if (state == GLUT_UP && button == GLUT_LEFT_BUTTON){
        if(x < 90) {
            checkRGBSelector(x, window_height - y, interfaceButtons[RGBSelector], current_color);
            for (int i = 0; i < 9; i++) {
                if (checkInterfaceClick(x, window_height - y, interfaceButtons[i])) {
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
            Vertice v = {.x = x, .y = window_height - y};
            Vertice *verticeslist = malloc(99 * sizeof(Vertice));
            verticeslist[0] = v;
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
            currentPolygon.qtd_Vertices += 1;
            Vertice v = {.x = x, .y = window_height - y};
            Vertice *newVerticeslist = malloc(currentPolygon.qtd_Vertices * sizeof(Vertice));
            newVerticeslist = currentPolygon.vertices;
            newVerticeslist[currentPolygon.qtd_Vertices - 1] = v;
            currentPolygon.vertices = newVerticeslist;
            // printf("[%d, %d]", currentPolygon.vertices[currentPolygon.qtd_Vertices - 1].x, currentPolygon.vertices[currentPolygon.qtd_Vertices - 1].y);
        }
        else if (current_state == DRAWING_LINE){
            currentLine.coords[1].x = x;
            currentLine.coords[1].y = window_height - y;
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
            currentLine.coords[0].x = x;
            currentLine.coords[0].y = window_height - y;
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
                    if (checkPointClick(temp->val, x, y, window_height, tolerancia)) {
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
                    if (checkLineClick(temp->val, x, y, window_height, tolerancia)) {
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
        }

        if(current_state == PONTO){
            Ponto novoPonto;
            novoPonto.vertice.x = x;
            novoPonto.vertice.y = window_height - y;
            novoPonto.color[0] = current_color[0];
            novoPonto.color[1] = current_color[1];
            novoPonto.color[2] = current_color[2];
            if (pointList) {
                PointNode *temp = pointList;
                PointNode *back = NULL;
                while (temp) {
                    back = temp;
                    temp = temp->next;
                }
                PointNode *insertPoint = (PointNode *) malloc(sizeof(PointNode));
                insertPoint->val = novoPonto;
                insertPoint->next = NULL;
                back->next = insertPoint;
            } else {
                pointList = (PointNode *) malloc(sizeof(PointNode));
                pointList->val = novoPonto;
                pointList->next = NULL;
            }
        }

    }
    if (state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON){
        if (pointList != NULL) {
            PointNode* temp = pointList;
            PointNode* back = NULL;

            while (temp) {
                if (checkPointClick(temp->val, x, y, window_height, tolerancia)) {
                    pontoSelecionado = NULL;
                    PointNode* nextNode = temp->next;
                    if (back == NULL) {
                        pointList = nextNode;
                    } else {
                        back->next = nextNode;
                    }
                    free(temp);
                    temp = nextNode;
                } else {
                    back = temp;
                    temp = temp->next;
                }
            }
        }
        if (lineList != NULL) {
            LineNode* temp = lineList;
            LineNode* back = NULL;

            while (temp) {
                if (checkLineClick(temp->val, x, y, window_height, tolerancia)) {
                    linhaSelecionada = NULL;
                    LineNode* nextNode = temp->next;
                    if (back == NULL) {
                        lineList = nextNode;
                    } else {
                        back->next = nextNode;
                    }
                    free(temp);
                    temp = nextNode;
                } else {
                    back = temp;
                    temp = temp->next;
                }
            }
        }
        if (polygonList != NULL) {
            PolygonNode* temp = polygonList;
            PolygonNode* back = NULL;

            while (temp) {
                if (checkPoligonoClick(temp->poligono, x, y, window_height, tolerancia)) {
                    PolygonNode* nextNode = temp->next;
                    if (back == NULL) {
                        polygonList = nextNode;
                    } else {
                        back->next = nextNode;
                    }
                    free(temp);
                    temp = nextNode;
                } else {
                    back = temp;
                    temp = temp->next;
                }
            }
        }
    }
}

void reshape(int newWidth, int newHeight){
    window_width = newWidth;
    window_height = newHeight;

    interfaceButtons[PolygonButton].y = window_height - 10;
    interfaceButtons[PointButton].y = window_height - 10;
    interfaceButtons[LineButton].y = window_height - 50;
    interfaceButtons[SelectButton].y = window_height - 50;
    interfaceButtons[RotateButton].y = window_height - 90;
    interfaceButtons[ResizeButton].y = window_height - 90;
    interfaceButtons[ReflexButton].y = window_height - 130;
    interfaceButtons[ShearButton].y = window_height - 130;
    interfaceButtons[RGBSelector].y = window_height - 170;


    glViewport( 0, 0, newWidth, newHeight );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0, newWidth, 0, newHeight);
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
        current_state = NONE;
    }
    // operações do ponto
    else if (current_state == NONE && pontoSelecionado != NULL) {
        switch(key){
            case 97: // A
                transladar(-5, 0, &pontoSelecionado->vertice, 1);
                break;
            case 100: // D
                transladar(5, 0, &pontoSelecionado->vertice, 1);
                break;
            case 119: // W
                transladar(0, 5, &pontoSelecionado->vertice, 1);
                break;
            case 115: // S
                transladar(0, -5, &pontoSelecionado->vertice, 1);
                break;
            default: break;
        };
    } else if (current_state == ROTACAO && pontoSelecionado != NULL) {
        switch(key){
            case 97: // A
                rotacionar(1, &pontoSelecionado->vertice, 1);
                break;
            case 100: // D
                rotacionar(-1, &pontoSelecionado->vertice, 1);
                break;
            default: break;
        };
    }
    // operações da linha
    else if (current_state == NONE && linhaSelecionada != NULL) {
        Vertice centro;
        getCentro(&linhaSelecionada->coords, 2, &centro);
        switch(key){
            case 97: // A
                transladar(-5, 0, &linhaSelecionada->coords, 2);
                break;
            case 100: // D
                transladar(5, 0, &linhaSelecionada->coords, 2);
                break;
            case 119: // W
                transladar(0, 5, &linhaSelecionada->coords, 2);
                break;
            case 115: // S
                transladar(0, -5, &linhaSelecionada->coords, 2);
                break;
            default: break;
        };
    } else if (current_state == ROTACAO && linhaSelecionada != NULL) {
        Vertice centro;
        getCentro(&linhaSelecionada->coords, 2, &centro);
        switch(key){
            case 97: // A
                transladar(-centro.x, -centro.y, &linhaSelecionada->coords, 2);
                rotacionar(1, &linhaSelecionada->coords, 2);
                transladar(centro.x, centro.y, &linhaSelecionada->coords, 2);
                break;
            case 100: // D
                transladar(-centro.x, -centro.y, &linhaSelecionada->coords, 2);
                rotacionar(-1, &linhaSelecionada->coords, 2);
                transladar(centro.x, centro.y, &linhaSelecionada->coords, 2);
                break;
            default: break;
        };
    } else if (current_state == ESCALA && linhaSelecionada != NULL) {
        Vertice centro;
        getCentro(&linhaSelecionada->coords, 2, &centro);
        switch(key){
            case 119: // W
                transladar(-centro.x, -centro.y, &linhaSelecionada->coords, 2);
                escalar(1.5, 1.5, &linhaSelecionada->coords, 2);
                transladar(centro.x, centro.y, &linhaSelecionada->coords, 2);
                break;
            case 115: // S
                transladar(-centro.x, -centro.y, &linhaSelecionada->coords, 2);
                escalar(0.5, 0.5, &linhaSelecionada->coords, 2);
                transladar(centro.x, centro.y, &linhaSelecionada->coords, 2);
                break;
            default: break;
        };
    } else if (current_state == CISALHAMENTO && linhaSelecionada != NULL) {
        Vertice centro;
        getCentro(&linhaSelecionada->coords, 2, &centro);
        switch(key){
            case 119: // W
                transladar(-centro.x, -centro.y, &linhaSelecionada->coords, 2);
                cisalhamento_y(0.5, &linhaSelecionada->coords, 2);
                transladar(centro.x, centro.y, &linhaSelecionada->coords, 2);
                break;
            case 115: // S
                transladar(-centro.x, -centro.y, &linhaSelecionada->coords, 2);
                cisalhamento_y(-0.5, &linhaSelecionada->coords, 2);
                transladar(centro.x, centro.y, &linhaSelecionada->coords, 2);
                break;
            case 97: // A
                transladar(-centro.x, -centro.y, &linhaSelecionada->coords, 2);
                cisalhamento_x(-0.5, &linhaSelecionada->coords, 2);
                transladar(centro.x, centro.y, &linhaSelecionada->coords, 2);
                break;
            case 100: // D
                transladar(-centro.x, -centro.y, &linhaSelecionada->coords, 2);
                cisalhamento_x(0.5, &linhaSelecionada->coords, 2);
                transladar(centro.x, centro.y, &linhaSelecionada->coords, 2);
                break;
            default: break;
        };
    } else if (current_state == ESPELHAMENTO && linhaSelecionada != NULL) {
        Vertice centro;
        getCentro(&linhaSelecionada->coords, 2, &centro);
        switch(key){
            case 119: // W
                reflexao_x(&linhaSelecionada->coords, 2);
                break;
            case 115: // S
                reflexao_x(&linhaSelecionada->coords, 2);
                break;
            case 97: // A
                reflexao_y(&linhaSelecionada->coords, 2);
                break;
            case 100: // D
                reflexao_y(&linhaSelecionada->coords, 2);
                break;
            case 113: // Q
                reflexao_xy(&linhaSelecionada->coords, 2);
                break;
            case 101: // E
                reflexao_xy(&linhaSelecionada->coords, 2);
                break;
            default: break;
        };
    } else {
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
    PrintPolygons(polygonList, current_state, currentPolygon);
    printLines(current_state, currentLine, lineList,mouse_x, mouse_y, window_height);
    printPoints(pointList);

    if(linhaSelecionada!= NULL) desenharSelecao(&linhaSelecionada->coords, 2);
    else if(pontoSelecionado!= NULL) desenharSelecao(&pontoSelecionado->vertice, 1);

    drawInterface(window_height, interfaceButtons, 9, current_color);
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

    interfaceButtons[PolygonButton] = (Botao){.x = 10, .y = window_height - 10, .size = 30, .icon = PolygonIcon};
    interfaceButtons[PointButton] = (Botao){.x = 50, .y = window_height - 10, .size = 30, .icon = PointIcon};
    interfaceButtons[LineButton] = (Botao){.x = 10, .y = window_height - 50, .size = 30, .icon = LineIcon};
    interfaceButtons[SelectButton] = (Botao){.x = 50, .y = window_height - 50, .size = 30, .icon = SelectIcon};
    interfaceButtons[RotateButton] = (Botao){.x = 10, .y = window_height - 90, .size = 30, .icon = RotateIcon};
    interfaceButtons[ResizeButton] = (Botao){.x = 50, .y = window_height - 90, .size = 30, .icon = ReSizeIcon};
    interfaceButtons[ReflexButton] = (Botao){.x = 10, .y = window_height - 130, .size = 30, .icon = ReflectionIcon};
    interfaceButtons[ShearButton] = (Botao){.x = 50, .y = window_height - 130, .size = 30, .icon = ShearIcon};
    interfaceButtons[RGBSelector] = (Botao){.x = 10, .y = window_height - 170, .size = 70, .icon = 0};

    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
