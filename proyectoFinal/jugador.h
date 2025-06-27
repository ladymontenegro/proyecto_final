#ifndef JUGADOR_H
#define JUGADOR_H
#include "personaje.h"

class Jugador : public Personaje
{
public:
    Jugador(QPixmap _hojaSprite);

private:
    unsigned short cargaSuper;
    bool superCargado;
    QPixmap hojaBarraSuper;
    QPixmap barraSuper;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void movimiento(int dx, int dy);
};

#endif // JUGADOR_H
