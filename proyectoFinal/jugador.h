#ifndef JUGADOR_H
#define JUGADOR_H
#include <QTimer>
#include "personaje.h"
#include "bonificacion.h"

class Jugador : public Personaje
{
    Q_OBJECT

public:
    Jugador(QPixmap _hojaSprite);

private:
    unsigned short contadorspriteMovimientoPoderGoku;
    unsigned short anchoSpriteMovimientoPoderGoku;
    unsigned short posicionXPoderGoku;
    unsigned short cargaSuper;
    unsigned short ultimaDireccion;  // 0: derecha, 1: izquierda, 2: arriba, 3: abajo
    bool superCargado;
    QPixmap hojaBarraSuper;
    QPixmap barraSuper;
    QPixmap hojaMovimientoPoderGoku;
    QPixmap spriteMovimientoPoderGoku;
    QTimer *timerMovimientoPoderGoku;
    QList<int> teclasPresionadas;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mostrarSpriteQuieto();
    void movimiento(int dx, int dy);
    void iniciarMovimientoPoderGoku();
    void movimientoPoderGoku();
    void verificarSuperCargado();
    void lanzarPoderGoku();

signals:
    void bonificacionRecolectada(Bonificacion* bonificacion);
};

#endif // JUGADOR_H
