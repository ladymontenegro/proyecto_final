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
    unsigned short ultimaDireccion;  // 0: derecha, 1: izquierda, 2: arriba, 3: abajo
    bool superCargado;
    QPixmap hojaBarraSuper;
    QPixmap barraSuper;
    QPixmap hojaPoderGoku;
    QPixmap spritePoderGoku;
    QTimer *timerPoderGoku;
    QList<int> teclasPresionadas;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mostrarSpriteQuieto();
    void movimiento(int dx, int dy);
    void iniciarPoderGoku();
    void poderGoku();
    void verificarSuperCargado();
};

#endif // JUGADOR_H
