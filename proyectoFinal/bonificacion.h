#ifndef BONIFICACION_H
#define BONIFICACION_H
#include <QGraphicsPixmapItem>

class Bonificacion
{
public:
    Bonificacion();
    void posicionPredeterminada();
private:
    QPixmap sprite;
    int valorCarga;
    int altoSprite;
    int anchoSprite;
    qreal x;
    qreal y;
};

#endif // BONIFICACION_H
