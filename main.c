#include <stdio.h>
#include <windows.h>
#include <GL/glut.h>

int window_height = 576;
int window_width = 1024;
float proportion_x = 204.8f/1024;
float proportion_y = 115.2f/576;

int mouse_x = 0;
int mouse_y = 0;

int tolerancia = 10;

enum State {
    LINHA,
    PONTO,
    DRAWING_LINE,
    DRAWING_POLYGON,
    NONE
};

typedef struct ponto{
    int x;
    int y;
    GLclampf color[3];
}Ponto;

typedef struct linha{
    int coords[2][2];
    GLclampf color[3];
}Linha;

typedef struct points {
    struct ponto val;
    struct points *next;
}PointNode;

typedef struct lines{
    struct linha val;
    struct lines *next;
}LineNode;

Linha currentLine;
PointNode* pointList;
LineNode* lineList;
enum State current_state = NONE;

int checkPointClick(Ponto ponto, int mouse_x, int mouse_y){
    int y = window_height - mouse_y;
    int x = mouse_x;
    if((x - tolerancia <= ponto.x && ponto.x <= x + tolerancia) &&
    (y - tolerancia <= ponto.y && ponto.y <= y + tolerancia)){
        return 1;
    }
    return 0;
}

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
        if (current_state == DRAWING_LINE){
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
            currentLine.color[0] = 0.0f;
            currentLine.color[1] = 0.0f;
            currentLine.color[2] = 0.0f;
            current_state = DRAWING_LINE;
        }

    }
    if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON){

        if(current_state == PONTO){
            Ponto novoPonto;
            novoPonto.x = x;
            novoPonto.y = window_height - y;
            novoPonto.color[0] = 0.0f;
            novoPonto.color[1] = 0.0f;
            novoPonto.color[2] = 0.0f;
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
                if (checkPointClick(temp->val, x, y)) {
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
    }
}

void reshape(int newWidth, int newHeight){
    window_width = newWidth;
    window_height = newHeight;
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
void printSinglePoint(Ponto ponto){
    glColor3f(ponto.color[0], ponto.color[1], ponto.color[2]);
    glBegin(GL_POINTS);
        glVertex2i(ponto.x,ponto.y);
    glEnd();
}

void printPoints(){
    if (pointList){
        PointNode* temp = pointList;
        while(temp){
            printSinglePoint(temp->val);
            temp = temp->next;
        }
    }
}

void printSingleLine(Linha linha){
    glColor3f(linha.color[0], linha.color[1], linha.color[2]);
    glBegin(GL_LINES);
    glVertex2i(linha.coords[0][0],linha.coords[0][1]);
    glVertex2i(linha.coords[1][0],linha.coords[1][1]);
    glEnd();
}

void printLines(){
    if (current_state == DRAWING_LINE){
        currentLine.coords[1][0] = mouse_x;
        currentLine.coords[1][1] = window_height - mouse_y;
        printSingleLine(currentLine);
    }
    if (lineList){
        LineNode* temp = lineList;
        while(temp){
            printSingleLine(temp->val);
            temp = temp->next;
        }
    }
}

void display(void){
    glutMouseFunc(mouse);
    glutReshapeFunc(reshape);
    glutMotionFunc(mouseMove);
    glutPassiveMotionFunc(mouseMove);
    glutKeyboardFunc(teclado);
    printPoints();
    printLines();
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
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
