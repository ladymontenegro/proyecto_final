#ifndef ENEMIGO_H
#define ENEMIGO_H
#include "Personaje.h"

class Enemigo : public Personaje
{
    Q_OBJECT
public:
    Enemigo(QPixmap _hojaSprite);
    void movimientoPredeterminado();

private:
    QTimer *tempAtaque;
};

#endif // ENEMIGO_H
