//
// Created by danil on 23/09/2024.
//

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "poligono.h"
#include "linha.h"
#include "ponto.h"
#include <GL/gl.h>
#include <GL/glu.h>


Vertice mid;

void CALLBACK tessBeginCallback(GLenum type) {
    glBegin(type);
}

void CALLBACK tessVertexCallback(GLvoid *vertex) {
    const GLdouble *pointer = (GLdouble *)vertex;
    glVertex2dv(pointer);
}

void CALLBACK tessEndCallback() {
    glEnd();
}


void AddPolygon(PolygonNode **lista, Poligono poligono) {
    PolygonNode *insertpolygon = (PolygonNode *) malloc(sizeof(PolygonNode));
    insertpolygon->poligono = poligono;
    insertpolygon->next = NULL;

    if (*lista) {
        PolygonNode *temp = *lista;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = insertpolygon;
    } else {
        *lista = insertpolygon;
    }
}

void PrintPolygons(PolygonNode *p, enum State current_state, Poligono currentPolygon, GLclampf *current_color, int mouse_x, int mouse_y) {

    PolygonNode *temp = p;
    while (temp != NULL) {
        glColor3f(temp->poligono.color[0], temp->poligono.color[1], temp->poligono.color[2]);

        GLUtesselator *tess = gluNewTess();
        gluTessCallback(tess, GLU_TESS_BEGIN, (void (CALLBACK *)())tessBeginCallback);
        gluTessCallback(tess, GLU_TESS_VERTEX, (void (CALLBACK *)())tessVertexCallback);
        gluTessCallback(tess, GLU_TESS_END, (void (CALLBACK *)())tessEndCallback);

        gluTessBeginPolygon(tess, NULL);
        gluTessBeginContour(tess);

        Vertice* tempvertice = temp->poligono.vertices;
        while(tempvertice != NULL){
            GLdouble *vertex = (GLdouble *)malloc(3 * sizeof(GLdouble));
            if (vertex == NULL) {
                gluDeleteTess(tess);
                return;
            }
            vertex[0] = tempvertice->x;
            vertex[1] = tempvertice->y;
            vertex[2] = 0;
            gluTessVertex(tess, vertex, vertex);
            tempvertice = tempvertice->next;
        }

        gluTessEndContour(tess);
        gluTessEndPolygon(tess);
        gluDeleteTess(tess);

        temp = temp->next;
    }

    if(current_state == DRAWING_POLYGON) {
        glColor3f(current_color[0], current_color[1], current_color[2]);
        glLineWidth(5.0f);
        glBegin(GL_LINES);
        glVertex2i(currentPolygon.vertices->x, currentPolygon.vertices->y);
        Vertice* temp = currentPolygon.vertices->next;
        while(temp){
            glVertex2i(temp->x, temp->y);
            glVertex2i(temp->x, temp->y);
            temp = temp->next;
        }
        glVertex2i(mouse_x, mouse_y);
        glEnd();
        glLineWidth(1.0f);
    }


}

int checkPoligonoClick(Poligono poligono, int mouse_x, int mouse_y, int window_height, int tolerancia, PointNode** pointList) {
    int acertos = 0;
    int y = window_height - mouse_y;
    Vertice* temp = poligono.vertices->next;
    Vertice* back = poligono.vertices;

    while (temp != NULL) {
        if ((temp->y > y) != (back->y > y)) {
            int intersect_x = back->x + (y - back->y) * (temp->x - back->x) / (temp->y - back->y);

            if (mouse_x < intersect_x) {
                acertos++;
            }
        }
        back = temp;
        temp = temp->next;
    }

    temp = poligono.vertices;
    if ((temp->y > y) != (back->y > y)) {
        int intersect_x = back->x + (y - back->y) * (temp->x - back->x) / (temp->y - back->y);

        if (mouse_x < intersect_x) {
            acertos++;
        }
    }

    return acertos % 2;
}

Vertice* converterArray(Poligono poligono){
    Vertice* vetor = (Vertice*)malloc(poligono.qtd_Vertices * sizeof(Vertice));
    Poligono temp = poligono;
    for (int i = 0; i< poligono.qtd_Vertices; i++){
        vetor[i].x = temp.vertices->x;
        vetor[i].y = temp.vertices->y;
        temp.vertices = temp.vertices->next;
    }
    return vetor;
}

int quad(Vertice p) {
    if (p.x >= 0 && p.y >= 0)
        return 1;
    if (p.x <= 0 && p.y >= 0)
        return 2;
    if (p.x <= 0 && p.y <= 0)
        return 3;
    return 4;
}

int orientation(Vertice a, Vertice b, Vertice c) {
    int val = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
    if (val == 0) return 0;
    return (val > 0) ? 1 : -1;
}

int compare(const void* p1, const void* q1) {
    Vertice p = *(Vertice*)p1;
    Vertice q = *(Vertice*)q1;
    if (p.x != q.x) {
        return (p.x < q.x) ? -1 : 1;
    }
    return (p.y < q.y) ? -1 : 1;
}

Vertice* merge(Vertice* a, int n1, Vertice* b, int n2, int* result_size) {
    int ia = 0, ib = 0;

    // Find the rightmost point of a
    for (int i = 1; i < n1; i++)
        if (a[i].x > a[ia].x)
            ia = i;

    // Find the leftmost point of b
    for (int i = 1; i < n2; i++)
        if (b[i].x < b[ib].x)
            ib = i;

    int inda = ia, indb = ib;
    int done = 0;

    // Finding the upper tangent
    while (!done) {
        done = 1;
        while (orientation(b[indb], a[inda], a[(inda + 1) % n1]) >= 0)
            inda = (inda + 1) % n1;

        while (orientation(a[inda], b[indb], b[(n2 + indb - 1) % n2]) <= 0) {
            indb = (n2 + indb - 1) % n2;
            done = 0;
        }
    }

    int uppera = inda, upperb = indb;

    // Finding the lower tangent
    inda = ia, indb = ib;
    done = 0;
    while (!done) {
        done = 1;
        while (orientation(a[inda], b[indb], b[(indb + 1) % n2]) >= 0)
            indb = (indb + 1) % n2;

        while (orientation(b[indb], a[inda], a[(n1 + inda - 1) % n1]) <= 0) {
            inda = (n1 + inda - 1) % n1;
            done = 0;
        }
    }

    int lowera = inda, lowerb = indb;

    // Allocating space for the result
    Vertice* result = (Vertice*)malloc((n1 + n2) * sizeof(Vertice));
    int idx = 0;

    // Add points from the first polygon
    int ind = uppera;
    result[idx++] = a[uppera];
    while (ind != lowera) {
        ind = (ind + 1) % n1;
        // Verificar duplicatas
        int alreadyExists = 0;
        for (int m = 0; m < idx; m++) {
            if (result[m].x == a[ind].x && result[m].y == a[ind].y) {
                alreadyExists = 1;
                break;
            }
        }
        if (!alreadyExists) {
            result[idx++] = a[ind];
        }
    }

    // Add points from the second polygon
        ind = lowerb;
        result[idx++] = b[lowerb];
        while (ind != upperb) {
            ind = (ind + 1) % n2;
            // Verificar duplicatas
            int alreadyExists = 0;
            for (int m = 0; m < idx; m++) {
                if (result[m].x == b[ind].x && result[m].y == b[ind].y) {
                    alreadyExists = 1;
                    break;
                }
            }
            if (!alreadyExists) {
                result[idx++] = b[ind];
            }
        }

    *result_size = idx;
    return result;
}

Vertice* bruteHull(Vertice* a, int size, int* result_size) {
    int i, j, k;
    int pos, neg;
    size_t max_size = 100;

    Vertice* ret = (Vertice*)malloc(sizeof(Vertice) * max_size);
    if (ret == NULL) {
        printf("Memory allocation failed for ret.\n");
        exit(1);
    }
    int count = 0;

    for (i = 0; i < size; i++) {
        for (j = i + 1; j < size; j++) {
            int a1 = a[i].y - a[j].y;
            int b1 = a[j].x - a[i].x;
            int c1 = a[i].x * a[j].y - a[i].y * a[j].x;

            pos = 0;
            neg = 0;
            for (k = 0; k < size; k++) {
                int val = a1 * a[k].x + b1 * a[k].y + c1;
                if (val < 0) neg++;
                if (val > 0) pos++;
            }

            // Se um lado não tiver pontos, adicione os pontos ao resultado
            if (pos == 0 || neg == 0) {
                // Verificar se o ponto já está no vetor de resultados
                int alreadyExists = 0;
                for (int m = 0; m < count; m++) {
                    if (ret[m].x == a[i].x && ret[m].y == a[i].y) {
                        alreadyExists = 1;
                        break;
                    }
                }
                if (!alreadyExists) {
                    ret[count++] = a[i];
                }

                alreadyExists = 0;
                for (int m = 0; m < count; m++) {
                    if (ret[m].x == a[j].x && ret[m].y == a[j].y) {
                        alreadyExists = 1;
                        break;
                    }
                }
                if (!alreadyExists) {
                    ret[count++] = a[j];
                }
            }
        }
    }

    // Sorting and removing duplicates
    *result_size = count;
    qsort(ret, count, sizeof(Vertice), compare);
    return ret;
}

Vertice* divide(Vertice* a, int size, int* result_size) {
    if (size <= 5)
        return bruteHull(a, size, result_size);

    int mid = size / 2;
    int left_size, right_size;

    Vertice* left = divide(a, mid, &left_size);
    Vertice* right = divide(a + mid, size - mid, &right_size);

    Vertice* merged = merge(left, left_size, right, right_size, result_size);
    free(left);
    free(right);
    return merged;
}

void fechoConvexo(PolygonNode **lista, Poligono *poligono) {
    Vertice *a = converterArray(*poligono);
    int n = poligono->qtd_Vertices;
    if (n == 0) {
        printf("O polígono não possui vértices.\n");
        return;
    }
    qsort(a, n, sizeof(Vertice), compare);
    for (int i = 0; i < poligono->qtd_Vertices ; i++) {
        printf("%f %f\n", a[i].x, a[i].y);
    }
    int result_size;
    Vertice* convex_hull = divide(a, n, &result_size);
    printf("qtd vertices %d\n", result_size);
    Poligono fecho;
    fecho.qtd_Vertices = result_size; // Corrigido para ser o tamanho do fecho convexo
    fecho.color[0] = 1.0f;
    fecho.color[1] = 0.0f;
    fecho.color[2] = 0.0f;

    Vertice *newVertice = (Vertice*)malloc(sizeof(Vertice));
    if (newVertice == NULL) {
        printf("Erro de alocação de memória.\n");
        free(convex_hull);
        return;
    }
    newVertice->x = convex_hull[0].x;
    newVertice->y = convex_hull[0].y;
    newVertice->next = NULL;
    fecho.vertices = newVertice;
    Vertice *current = newVertice;

    printf("%f %f\n", convex_hull[0].x, convex_hull[0].y);
    for (int i = 1; i < result_size ; i++) {
        Vertice *newVertice = (Vertice*)malloc(sizeof(Vertice));
        if (newVertice == NULL) {
            printf("Erro de alocação de memória.\n");
            free(convex_hull);
            return;
        }
        newVertice->x = convex_hull[i].x;
        newVertice->y = convex_hull[i].y;
        newVertice->next = NULL;
        current->next = newVertice;
        current = newVertice;
        printf("%f %f\n", convex_hull[i].x, convex_hull[i].y);
    }
    printf("Fecho coords:");
    Vertice* tempvertice = fecho.vertices;
    while(tempvertice != NULL){
        printf("%f %f\n", tempvertice->x, tempvertice->y);
        tempvertice = tempvertice->next;
    }

    AddPolygon(lista, fecho);

    free(convex_hull);
}
