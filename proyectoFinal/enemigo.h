#ifndef ENEMIGO_H
#define ENEMIGO_H
#include "Personaje.h"

class Enemigo : public Personaje
{
    Q_OBJECT
public:
    Enemigo(QPixmap _hojaSprite,
            unsigned short _x,
            unsigned short _y,
            unsigned short _anchoSprite,
            unsigned short _altoSprite,
            unsigned short _anchoSpriteEscalar,
            unsigned short _altoSpriteEscalar);

    void movimientoPredeterminado();

private:
    QTimer *timerMovimientoPoderRochi;
    unsigned short contadorspriteMovimientoPoderRochi;

    void iniciarMovimientoPoderRochi();
    void movimientoPoderRochi();
    void lanzarPoderRochi();
};

#endif // ENEMIGO_H
