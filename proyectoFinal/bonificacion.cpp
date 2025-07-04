#include "bonificacion.h"
#include <QEasingCurve>
#include <QTimer>

Bonificacion::Bonificacion(const QPixmap &sprite, QGraphicsItem *parent)
    : QObject(nullptr), QGraphicsPixmapItem(sprite, parent), valorCarga(1)
{
    timerAnimacion = new QTimer(this);
    connect(timerAnimacion, &QTimer::timeout, this, &Bonificacion::actualizarAnimacion);
    direccionAnimacion = 0.7; // 1 = abajo, -1 = arriba
}

void Bonificacion::iniciarAnimacionFlotacion() {
    posicionYInicial = y();
    timerAnimacion->start(30); // Actualizar cada 50ms
}

void Bonificacion::detenerAnimacion() {
    timerAnimacion->stop();
    setY(posicionYInicial);
}

void Bonificacion::actualizarAnimacion() {
    qreal nuevaY = y() + (0.2 * direccionAnimacion);
    setY(nuevaY);

    // Cambiar dirección si se alcanzan los límites
    if (y() > posicionYInicial + 5) direccionAnimacion = -1;
    else if (y() < posicionYInicial - 5) direccionAnimacion = 1;
}

unsigned short Bonificacion::getValorCarga() const {
    return valorCarga;
}
