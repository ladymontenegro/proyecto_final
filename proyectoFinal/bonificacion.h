
#ifndef BONIFICACION_H
#define BONIFICACION_H

#include <QGraphicsPixmapItem>

class Bonificacion : public QGraphicsPixmapItem
{
private:
    QPixmap sprite;
    unsigned short valorCarga;
    unsigned short altoSprite;
    unsigned short anchoSprite;
    qreal x;
    qreal y;

public:
    Bonificacion(const QPixmap &_sprite, QGraphicsItem *parent = nullptr);
    unsigned short getValorCarga() const;
};

#endif // BONIFICACION_H

