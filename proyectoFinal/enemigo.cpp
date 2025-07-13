#include "enemigo.h"

Enemigo::Enemigo(QPixmap _hojaSprite,
                 unsigned short _x,
                 unsigned short _y,
                 unsigned short _anchoSprite,
                 unsigned short _altoSprite,
                 unsigned short _anchoSpriteEscalar,
                 unsigned short _altoSpriteEscalar)
    : Personaje(_hojaSprite,
                _x,
                _y,
                _anchoSprite,
                _altoSprite,
                _anchoSpriteEscalar,
                _altoSpriteEscalar)
{}

void Enemigo::iniciarMovimientoPoderRochi()
{
    contadorspriteMovimientoPoderRochi = 0; // Reinicia la animacion al inicio
    movimientoPoderRochi();                 // Muestra el primer frame inmediatamente
    timerMovimientoPoderRochi->start();     // Inicia el timer para el resto de la animacion
}

void Enemigo::movimientoPoderRochi()
{
    if (contadorspriteMovimientoPoderRochi <= 6) {
        posicionXPoderRochi = 0;
        anchoSpriteMovimientoPoderRochi = 66; // Ancho por defecto
        posicionXPoderRochi = contadorspriteMovimientoPoderRochi * anchoSpriteMovimientoPoderRochi;
        spriteMovimientoPoderRochi = hojaMovimientoPoderRochi.copy(posicionXPoderRochi,
                                                                   0,
                                                                   anchoSpriteMovimientoPoderRochi,
                                                                   128);
        QPixmap spriteEscalado = spriteMovimientoPoderRochi.scaled(22, 32);

        //espejar el sprite
        if (ultimaDireccion == 1) {
            spriteEscalado = spriteEscalado.transformed(QTransform().scale(-1, 1));
        }

        setPixmap(spriteEscalado);
        contadorspriteMovimientoPoderRochi++;

    } else {
        timerMovimientoPoderRochi->stop();
        lanzarPoderRochi();
        mostrarSpriteQuieto();
    }
}

void Enemigo::lanzarPoderRochi()
{
    QPointF inicio = {ultimaDireccion != 1 ? x + 20 : x - 20, y + 4};
    QPixmap sprite(":/multimedia/poderRochi.png");
    QPixmap spriteEscalado = sprite.scaled(30, 20);

    Ataque *ataqueRochi = new Ataque(spriteEscalado, inicio, ultimaDireccion);
    scene()->addItem(ataqueRochi);
}
