#include "personaje.h"
#include <QDebug>
#include <QGraphicsScene>

#include "obstaculo.h"

Personaje::Personaje(QPixmap _hojaSprite)
    : hojaSprite(_hojaSprite)
    , posicionX(0)
    , posicionY(0)
    , anchoSprite(64)
    , altoSprite(64)
    , conteoSprite(0)
{
    sprite = hojaSprite.copy(posicionX, posicionY, anchoSprite, altoSprite);
    QPixmap spriteEscalado = sprite.scaled(22, 22);
    setPixmap(spriteEscalado);
}

void Personaje::movimientoSprite(int direccion)
{
    posicionY = direccion;
    posicionX = anchoSprite * conteoSprite;
    sprite = hojaSprite.copy(posicionX, posicionY, anchoSprite, altoSprite);
    QPixmap spriteEscalado = sprite.scaled(22, 22);
    setPixmap(spriteEscalado);
    conteoSprite++;

    if (conteoSprite == 8) {
        conteoSprite = 0;
    }
}

void Personaje::movimientoSpriteYajirobe()
{
    posicionY = 0;
    posicionX = 63 * conteoSprite;

    conteoSprite++;

    if (conteoSprite == 2)
        conteoSprite = 0;

    QPointF posActual = pos();
    setPos(posActual.x() + direccionXYajirobe, posActual.y());

    QList<QGraphicsItem *> itemsChocados = collidingItems();
    for (QGraphicsItem *item : itemsChocados) {
        if (dynamic_cast<Obstaculo *>(item)) {
            // Invertir direccion
            direccionXYajirobe *= -1;
            sprite = sprite.transformed(QTransform().scale(-1, 1));
            QPixmap spriteEscalado = sprite.scaled(22, 22);
            setPixmap(spriteEscalado);
            break;
        }
    }
}
