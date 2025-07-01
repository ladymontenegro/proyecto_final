#ifndef JUGADOR_H
#define JUGADOR_H
#include <QTimer>
#include "personaje.h"

class Jugador : public Personaje
{
public:
    Jugador(QPixmap _hojaSprite);

private:
    unsigned short contadorSpritePoderGoku;
    unsigned short anchoSpritePoderGoku;
    unsigned short posicionXPoderGoku;
    unsigned short cargaSuper;
    bool superCargado;
    QPixmap hojaBarraSuper;
    QPixmap barraSuper;
    QPixmap hojaPoderGoku;
    QPixmap spritePoderGoku;
    QTimer *timerPoderGoku;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void movimiento(int dx, int dy);
    void iniciarPoderGoku();
    void poderGoku();
    void verificarSuperCargado();
};

#endif // JUGADOR_H
