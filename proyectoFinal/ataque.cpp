#include "ataque.h"
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QList>
#include "obstaculo.h"

Ataque::Ataque(QPixmap _spriteAtaque, QPointF inicioPosicion, unsigned short _direccion)
    : QGraphicsPixmapItem()
    , velocidad(15)
    , direccion(_direccion)
    , spriteAtaque(_spriteAtaque)
{
    // Reflejar sprite si va a la izquierda
    if (direccion == 1) {
        spriteAtaque = spriteAtaque.transformed(QTransform().scale(-1, 1));
    }

    setPixmap(spriteAtaque);
    setPos(inicioPosicion);

    // Crear e iniciar el temporizador de movimiento
    timerMovimiento = new QTimer(this);
    connect(timerMovimiento, &QTimer::timeout, this, &Ataque::mover);
    timerMovimiento->start(40); //
}

void Ataque::mover()
{
    // Mueve el poder en la direccion correcta
    qreal nuevoX = x() + (direccion == 0 ? velocidad : -velocidad);
    setPos(nuevoX, y());

    // Verificar colisiones
    QList<QGraphicsItem *> colisionados = collidingItems();
    for (QGraphicsItem *item : colisionados) {
        // Si choca con un cuadrado
        if (Obstaculo *cuadrado = dynamic_cast<Obstaculo *>(item)) {
            cuadrado->destruirConResiduos(scene());
            scene()->removeItem(this);
            this->deleteLater();
            return;
        }

        // Si choca con un muro
        if (item->type() == QGraphicsRectItem::Type) {
            scene()->removeItem(this);
            this->deleteLater();
            return;
        }
    }
}
