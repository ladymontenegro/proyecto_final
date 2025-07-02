#include "bonificacion.h"
#include <QGraphicsPixmapItem>

Bonificacion::Bonificacion(const QPixmap &_sprite, QGraphicsItem *parent): QGraphicsPixmapItem(_sprite, parent) {
    valorCarga = 1;
}
unsigned short Bonificacion::getValorCarga() const {
    return valorCarga;
}
