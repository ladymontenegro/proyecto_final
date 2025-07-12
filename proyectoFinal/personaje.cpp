#include "personaje.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QMessageBox>
#include "jugador.h"
#include "obstaculo.h"

Personaje::Personaje(QPixmap _hojaSprite,
                     unsigned short _x,
                     unsigned short _y,
                     unsigned short _anchoSprite,
                     unsigned short _altoSprite,
                     unsigned short _anchoSpriteEscalar,
                     unsigned short _altoSpriteEscalar)
    : x(_x)
    , y(_y)
    , hojaSprite(_hojaSprite)
    , posicionX(0)
    , posicionY(0)
    , anchoSprite(_anchoSprite)
    , altoSprite(_altoSprite)
    , anchoSpriteEscalar(_anchoSpriteEscalar)
    , altoSpriteEscalar(_altoSpriteEscalar)
    , conteoSprite(0)
    , cargaVida(4)
{
    sprite = hojaSprite.copy(posicionX, posicionY, anchoSprite, altoSprite);
    QPixmap spriteEscalado = sprite.scaled(anchoSpriteEscalar, altoSpriteEscalar);
    setPixmap(spriteEscalado);
}

unsigned short Personaje::getCargaVida() const {return cargaVida;}

void Personaje::movimientoSprite(int direccion, unsigned short cantidadDeSprites)
{
    posicionY = direccion;
    posicionX = anchoSprite * conteoSprite;
    sprite = hojaSprite.copy(posicionX, posicionY, anchoSprite, altoSprite);
    QPixmap spriteEscalado = sprite.scaled(anchoSpriteEscalar, altoSpriteEscalar);
    setPixmap(spriteEscalado);
    conteoSprite++;

    if (conteoSprite == cantidadDeSprites) {
        conteoSprite = 0;
    }
}

void Personaje::movimientoSpriteYajirobe()
{
    posicionY = 0;
    posicionX = anchoSprite * conteoSprite;

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
            QPixmap spriteEscalado = sprite.scaled(anchoSpriteEscalar, altoSpriteEscalar);
            setPixmap(spriteEscalado);
            break;
        }
    }
}
bool Personaje::verificarVictoriaNivel1()
{
    QList<QGraphicsItem *> itemsChocados = collidingItems();
    for (QGraphicsItem *item : itemsChocados) {
        if (dynamic_cast<Jugador *>(item)) {
            return true;
        }
    }
    return false;
}
