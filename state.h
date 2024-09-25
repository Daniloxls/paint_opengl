#ifndef STATE_H
#define STATE_H

enum State {
    LINHA,
    PONTO,
    POLIGONO,
    DRAWING_LINE,
    DRAWING_POLYGON,
    ROTACAO,
    ESCALA,
    ESPELHAMENTO,
    CISALHAMENTO,
    NONE
};

typedef struct {
    float left;
    float right;
    float top;
    float bottom;

}WindowBorder;

#endif

