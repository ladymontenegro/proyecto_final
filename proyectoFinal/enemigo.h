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

    virtual ~Enemigo() override;

    //void movimientoPredeterminado();

private:
    QTimer *timerMovimientoAtaque;

private slots:
    void movimientoAtaque();

signals:
    void golpeRecibido();
};

#endif // ENEMIGO_H
