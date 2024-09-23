
#include "botao.h"

#ifndef INTERFACE_H
#define INTERFACE_H

void drawPolygonIcon(int, int, int);
void drawSelectIcon(int, int, int);
void drawLineIcon(int, int, int);
void drawPointIcon(int, int);
void drawButton(int, int, int, int);
void drawSelectorRGB(int, int, int, GLclampf[]);
void drawInterface(int, Botao[], int, GLclampf[]);
int checkInterfaceClick(int, int, Botao);
void checkRGBSelector(int, int, Botao, GLclampf*);


#endif