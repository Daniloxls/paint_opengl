#define PolygonIcon 1
#define PointIcon 2
#define LineIcon 3
#define SelectIcon 4
#define RotateIcon 5
#define ReSizeIcon 6
#define ReflectionIcon 7
#define ShearIcon 8


#ifndef BOTAO_H
#define BOTAO_H

typedef struct {
  int x;
  int y;
  int size;
  int icon;
} Botao;

enum BotaoID {
  PolygonButton,
  PointButton,
  LineButton,
  SelectButton,
  RotateButton,
  ResizeButton,
  ReflexButton,
  ShearButton,
  RGBSelector,
};


#endif
