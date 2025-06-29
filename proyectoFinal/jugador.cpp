#include "jugador.h"
//#include <QDebug>

Jugador::Jugador(QPixmap _hojaSprite)
    : Personaje(_hojaSprite)
{
    x = 187;
    y = 87;
    setFlag(QGraphicsItem::ItemIsFocusable);
}

void Jugador::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_A:
        movimiento(-5, 0);
        movimientoSprite(2496);
        break;
    case Qt::Key_W:
        movimiento(0, -5);
        movimientoSprite(2432);
        break;
    case Qt::Key_S:
        movimiento(0, 5);
        movimientoSprite(2560);
        break;
    case Qt::Key_D:
        movimiento(5, 0);
        movimientoSprite(2624);
        break;
    default:
        QGraphicsItem::keyPressEvent(event);
        break;
    }
}

void Jugador::movimiento(int dx, int dy)
{
    // Posicion actual antes de intentar el movimiento
    qreal oldX = x;
    qreal oldY = y;

    // Intenta mover al jugador
    setPos(x + dx, y + dy);

    // Actualiza la nueva posicion
    x = pos().x();
    y = pos().y();

    QList<QGraphicsItem *> itemsChocados
        = collidingItems(); // Obtiene los elementos con los que colisiona

    for (QGraphicsItem *item : itemsChocados) {        // Itera sobre los elementos colisionados
        if (item->type() == QGraphicsRectItem::Type) { // Si colisiona con un rectangulo
            // Deshace el movimiento y pone la posicion anterior
            setPos(oldX, oldY);
            x = oldX; //Actualizar variables miembro
            y = oldY;
            break;
        }
    }
}
