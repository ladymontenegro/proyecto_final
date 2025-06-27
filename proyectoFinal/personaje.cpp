#include "personaje.h"
#include <QDebug>

Personaje::Personaje(QPixmap _hojaSprite)
    : hojaSprite(_hojaSprite)
    , posicionX(0)
    , posicionY(0)
    , anchoSprite(64)
    , altoSprite(64)
    , conteoSprite(0)
{
    sprite = hojaSprite.copy(posicionX, posicionY, anchoSprite, altoSprite);
    QPixmap spriteEscalado = sprite.scaled(25, 25);
    setPixmap(spriteEscalado);
}

void Personaje::movimientoSprite(int direccion)
{
    posicionY = direccion;
    posicionX = anchoSprite * conteoSprite;
    sprite = hojaSprite.copy(posicionX, posicionY, anchoSprite, altoSprite);
    QPixmap spriteEscalado = sprite.scaled(25, 25);
    setPixmap(spriteEscalado);
    conteoSprite++;

    if (conteoSprite == 8) {
        conteoSprite = 0;
    }
}
