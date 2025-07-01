#ifndef BONIFICACION_H
#define BONIFICACION_H
#include <QGraphicsPixmapItem>

class Bonificacion
{
private:
    QPixmap sprite;
    unsigned short valorCarga;
    unsigned short altoSprite;
    unsigned short anchoSprite;
    qreal x;
    qreal y;

public:
    Bonificacion();
    void posicionPredeterminada();
};

#endif // BONIFICACION_H
