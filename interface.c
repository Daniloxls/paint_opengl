#include "botao.h"

void drawResizeIcon(int x1, int y1, int iconSize)
{
    glLineWidth(3.0);
    glBegin(GL_LINES);
    glVertex2i(x1, y1 - iconSize);
    glVertex2i(x1 + iconSize, y1);
    glEnd();
    glLineWidth(1.0);

    glBegin(GL_POLYGON);
    glVertex2i(x1, y1 - iconSize);
    glVertex2i(x1, y1 - iconSize + 10);
    glVertex2i(x1 + 10, y1 - iconSize);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2i(x1 + iconSize, y1);
    glVertex2i(x1 + iconSize - 10, y1);
    glVertex2i(x1 + iconSize, y1 - 10);
    glEnd();
}

void drawRotateicon(int x1, int y1, int iconSize)
{
    glLineWidth(3.0);
    glBegin(GL_LINES);
    glVertex2i(x1 + 3, y1 - iconSize);
    glVertex2i(x1 + 3, y1 - (iconSize/2) - 2);
    glVertex2i(x1 + 3, y1 - iconSize);
    glVertex2i(x1 + iconSize - 3, y1 - iconSize);
    glVertex2i(x1 + 3, y1);
    glVertex2i(x1 + iconSize - 3, y1);
    glVertex2i(x1 + iconSize - 3, y1);
    glVertex2i(x1 + iconSize - 3, y1 - (iconSize/2) + 2);
    glEnd();
    glLineWidth(1.0);

    glBegin(GL_POLYGON);
    glVertex2i(x1 - 1, y1 - (iconSize/2) - 2);
    glVertex2i(x1 + 7, y1 - (iconSize/2) - 2);
    glVertex2i(x1 + 3, y1 - (iconSize/2) + 4);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2i(x1 + iconSize + 1, y1 - (iconSize/2) + 2);
    glVertex2i(x1 + iconSize - 7, y1 - (iconSize/2) + 2);
    glVertex2i(x1 + iconSize - 3, y1 - (iconSize/2) - 4);
    glEnd();
}

void drawPolygonIcon(int x1, int y1, int iconSize)
{
    glLineWidth(2.0);
    glBegin(GL_LINE_LOOP);
    glVertex2i(x1 + 5, y1);
    glVertex2i(x1 + iconSize - 5, y1);
    glVertex2i(x1 + iconSize, y1 - iconSize);
    glVertex2i(x1, y1 - iconSize);
    glEnd();
    glLineWidth(1.0);
}

void drawSelectIcon(int x1, int y1, int iconSize)
{
    glLineWidth(2.0);
    glBegin(GL_LINE_LOOP);
    glVertex2i(x1, y1);
    glVertex2i(x1 + iconSize, y1 - iconSize + 10);
    glVertex2i(x1 + iconSize - 10, y1 - iconSize + 8);
    glVertex2i(x1 + iconSize - iconSize / 2 - 5, y1 - iconSize);
    glEnd();
    glLineWidth(1.0);
}

void drawPointIcon(int x1, int y1)
{
    glPointSize(14.0f);
    glBegin(GL_POINTS);
    glVertex2i(x1, y1);
    glEnd();
    glPointSize(20.0f);
}

void drawLineIcon(int x1, int y1, int iconSize)
{
    glLineWidth(5.0);

    glBegin(GL_LINES);
    glVertex2i(x1, y1);
    glVertex2i(x1 + iconSize, y1 + iconSize);
    glEnd();
    glLineWidth(1.0);
}

void drawButton(int x1, int y1, int buttonSize, int iconType)
{
    glColor3f(0.95, 0.95, 0.95);
    glBegin(GL_POLYGON);
    glVertex2i(x1, y1);
    glVertex2i(x1 + buttonSize, y1);
    glVertex2i(x1 + buttonSize, y1 - buttonSize);
    glVertex2i(x1, y1 - buttonSize);
    glEnd();
    glColor3f(0.2, 0.2, 0.2);
    glBegin(GL_LINE_LOOP);
    glVertex2i(x1, y1);
    glVertex2i(x1 + buttonSize, y1);
    glVertex2i(x1 + buttonSize, y1 - buttonSize);
    glVertex2i(x1, y1 - buttonSize);
    glEnd();

    glColor3f(0.2, 0.2, 0.2);
    switch (iconType)
    {
    case 1:
        drawPolygonIcon(x1 + 5, y1 - 5, buttonSize - 10);
        break;
    case 2:
        drawPointIcon(x1 + 15, y1 - 15);
        break;
    case 3:
        drawLineIcon(x1 + 5, y1 - buttonSize + 5, 20);
        break;
    case 4:
        drawSelectIcon(x1 + 5, y1 - 5, 20);
        break;
    case 5:
        drawRotateicon(x1 + 5, y1 - 5, 20);
        break;

    default:
        drawResizeIcon(x1 + 5, y1 - 5, 20);
        break;
    }
}

void drawSelectorRGB(int x1, int y1, int buttonSize, GLclampf current_color[])
{
    glColor3f(1, 1, 1);
    glBegin(GL_POLYGON);
    glVertex2i(x1, y1);
    glVertex2i(x1 + buttonSize, y1);
    glVertex2i(x1 + buttonSize, y1 - buttonSize / 4);
    glVertex2i(x1, y1 - buttonSize / 4);
    glEnd();

    glColor3f(1, 0, 0);
    glBegin(GL_POLYGON);
    glVertex2i(x1, y1);
    glVertex2i(x1 + (buttonSize * current_color[0]), y1);
    glVertex2i(x1 + (buttonSize * current_color[0]), y1 - buttonSize / 4);
    glVertex2i(x1, y1 - buttonSize / 4);
    glEnd();

    glColor3f(0.2, 0.2, 0.2);
    glBegin(GL_LINE_LOOP);
    glVertex2i(x1, y1);
    glVertex2i(x1 + buttonSize, y1);
    glVertex2i(x1 + buttonSize, y1 - buttonSize / 4);
    glVertex2i(x1, y1 - buttonSize / 4);
    glEnd();

    y1 = y1 - (buttonSize / 4) - 10;
    glColor3f(1, 1, 1);
    glBegin(GL_POLYGON);
    glVertex2i(x1, y1);
    glVertex2i(x1 + buttonSize, y1);
    glVertex2i(x1 + buttonSize, y1 - buttonSize / 4);
    glVertex2i(x1, y1 - buttonSize / 4);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0, 1, 0);
    glVertex2i(x1, y1);
    glVertex2i(x1 + (buttonSize * current_color[1]), y1);
    glVertex2i(x1 + (buttonSize * current_color[1]), y1 - buttonSize / 4);
    glVertex2i(x1, y1 - buttonSize / 4);
    glEnd();

    glColor3f(0.2, 0.2, 0.2);
    glBegin(GL_LINE_LOOP);
    glVertex2i(x1, y1);
    glVertex2i(x1 + buttonSize, y1);
    glVertex2i(x1 + buttonSize, y1 - buttonSize / 4);
    glVertex2i(x1, y1 - buttonSize / 4);
    glEnd();

    y1 = y1 - (buttonSize / 4) - 10;
    glColor3f(1, 1, 1);
    glBegin(GL_POLYGON);
    glVertex2i(x1, y1);
    glVertex2i(x1 + buttonSize, y1);
    glVertex2i(x1 + buttonSize, y1 - buttonSize / 4);
    glVertex2i(x1, y1 - buttonSize / 4);
    glEnd();

    glColor3f(0, 0, 1);
    glBegin(GL_POLYGON);
    glVertex2i(x1, y1);
    glVertex2i(x1 + (buttonSize * current_color[2]), y1);
    glVertex2i(x1 + (buttonSize * current_color[2]), y1 - buttonSize / 4);
    glVertex2i(x1, y1 - buttonSize / 4);
    glEnd();

    glColor3f(0.2, 0.2, 0.2);
    glBegin(GL_LINE_LOOP);
    glVertex2i(x1, y1);
    glVertex2i(x1 + buttonSize, y1);
    glVertex2i(x1 + buttonSize, y1 - buttonSize / 4);
    glVertex2i(x1, y1 - buttonSize / 4);
    glEnd();

    y1 = y1 - (buttonSize / 4) - 10;
    glColor3f(current_color[0], current_color[1], current_color[2]);
    glBegin(GL_POLYGON);
    glVertex2i(x1, y1);
    glVertex2i(x1 + buttonSize, y1);
    glVertex2i(x1 + buttonSize, y1 - buttonSize);
    glVertex2i(x1, y1 - buttonSize);
    glEnd();

    glColor3f(0.2, 0.2, 0.2);
    glBegin(GL_LINE_LOOP);
    glVertex2i(x1, y1);
    glVertex2i(x1 + buttonSize, y1);
    glVertex2i(x1 + buttonSize, y1 - buttonSize);
    glVertex2i(x1, y1 - buttonSize);
    glEnd();
}

void drawInterface(int window_height, Botao botoes[], int tam, GLclampf current_color[])
{
    glColor3f(0.85, 0.9, 0.95);
    glBegin(GL_POLYGON);
    glVertex2i(0, 0);
    glVertex2i(90, 0);
    glVertex2i(90, window_height);
    glVertex2i(0, window_height);
    glEnd();

    for (int i = 0; i < tam - 1; i++)
        drawButton(botoes[i].x, botoes[i].y, botoes[i].size, botoes[i].icon);
    drawSelectorRGB(botoes[RGBSelector].x, botoes[RGBSelector].y,
                    botoes[RGBSelector].size, current_color);

    glColor3f(0.80, 0.80, 0.80);
    glBegin(GL_LINES);
    glVertex2i(90, window_height);
    glVertex2i(90, 0);
    glEnd();
}

int checkInterfaceClick(int x, int y, Botao b)
{
    if (b.x < x && b.x + b.size > x && b.y > y && b.y - b.size < y)
        return 1;
    else
        return 0;
}

void checkRGBSelector(int x, int y, Botao b, GLclampf *current_color)
{
    if (b.x < x && b.x + b.size > x && b.y > y && b.y - (b.size / 4) < y)
    {
        current_color[0] = (((float)x - (float)b.x) / (float)b.size);
    }

    b.y = b.y - (b.size / 4) - 10;
    if (b.x < x && b.x + b.size > x && b.y > y && b.y - (b.size / 4) < y)
    {
        current_color[1] = (((float)x - (float)b.x) / (float)b.size);
    }

    b.y = b.y - (b.size / 4) - 10;
    if (b.x < x && b.x + b.size > x && b.y > y && b.y - (b.size / 4) < y)
    {
        current_color[2] = (((float)x - (float)b.x) / (float)b.size);
    }
}