#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <GL/glut.h>
#include "interface.h"
#include "linha.h"
#include "ponto.h"
#include "poligono.h"

int window_height = 576;
int window_width = 1024;
float proportion_x = 204.8f/1024;
float proportion_y = 115.2f/576;
int mouse_x = 0;
int mouse_y = 0;
int tolerancia = 10;
Botao interfaceButtons[7];
GLclampf current_color[3] = {0.0, 0.0, 0.0};
Linha currentLine;
PointNode* pointList;
LineNode* lineList;
enum State current_state = NONE;


// COMEÇA AQUI AAAAAAAAAA



PolygonNode *polygonList = NULL; // lista de poligonos
Poligono currentPolygon;


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
            for (int i = 0; i < 4; i++) {
                if (checkInterfaceClick(x, window_height - y, interfaceButtons[i])) {
                    switch (i) {
                    case LineButton: current_state = LINHA; break;
                    case PointButton: current_state = PONTO; break;
                    case PolygonButton: current_state = POLIGONO; break;

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
            currentLine.coords[1][0] = x;
            currentLine.coords[1][1] = window_height - y;
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
            currentLine.coords[0][0] = x;
            currentLine.coords[0][1] = window_height - y;
            currentLine.color[0] = current_color[0];
            currentLine.color[1] = current_color[1];
            currentLine.color[2] = current_color[2];
            current_state = DRAWING_LINE;
        }

    }
    if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON){

        if(current_state == PONTO){
            Ponto novoPonto;
            novoPonto.x = x;
            novoPonto.y = window_height - y;
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
    interfaceButtons[RGBSelector].y = window_height - 140;


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
        // printf("\nFechando poligono\n");
        // for (int i = 0; i < currentPolygon.qtd_Vertices; i++) {
        //     printf("[%d, %d]\n", currentPolygon.vertices[i].x, currentPolygon.vertices[i].y);
        // }

        AddPolygon(&polygonList, currentPolygon);
        Poligono p;
        currentPolygon = p;
        current_state = NONE;
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
    drawInterface(window_height, interfaceButtons, 7, current_color);
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
    interfaceButtons[RGBSelector] = (Botao){.x = 10, .y = window_height - 140, .size = 70, .icon = 0};

    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
