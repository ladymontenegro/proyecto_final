#ifndef JUGADOR_H
#define JUGADOR_H
#include <QTimer>
#include "personaje.h"
#include "bonificacion.h"

class Jugador : public Personaje
{
    Q_OBJECT

public:
    Jugador(QPixmap _hojaSprite,
            unsigned short _x,
            unsigned short _y,
            unsigned short _anchoSprite,
            unsigned short _altoSprite,
            unsigned short _anchoSpriteEscalar,
            unsigned short _altoSpriteEscalar);

    virtual ~Jugador() override;

    bool nivelDosActivo = true;
    void setValidoCargarSuper(bool estado);
    void setPuntoReinicio(qreal x, qreal y);
    void setCargaSuper(unsigned short nuevaCarga);
    unsigned short getCargaSuper();
    void perderVida();
    void desactivarControles();

private:
    //general
    qreal puntoReinicioX;
    qreal puntoReinicioY;

    //activar poder
    QPixmap hojaMovimientoPoderGoku;
    QPixmap spriteMovimientoPoderGoku;
    unsigned short contadorspriteMovimientoPoderGoku;
    unsigned short anchoSpriteMovimientoPoderGoku;
    unsigned short posicionXPoderGoku;
    QTimer *timerMovimientoPoderGoku;

    //activar salto
    unsigned short posicionSaltoX;
    unsigned short contadorspriteMovimientoSalto;
    QTimer *timerMovimientoSalto;

    //activar agacho
    unsigned short posicionAgachoX;
    unsigned short contadorspriteMovimientoAgacho;
    QTimer *timerMovimientoAgacho;

    //super del jugador
    unsigned short cargaSuper;
    bool validoCargarSuper;

    //posicion _ general
    unsigned short ultimaDireccion;  // 0: derecha, 1: izquierda, 2: arriba, 3: abajo
    QList<unsigned short> teclasPresionadas;
    QTimer* timerGravedad;

    //poicion - nivel 2
    bool enPlataforma;
    bool porCaida;
    qreal velocidadVertical;

private slots:
    void aplicarGravedad();
    void movimientoAgacho();
    void movimientoPoderGoku();
    void movimientoSalto();

protected:
    //eventos de teclas
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    void mostrarSpriteQuieto();
    void movimiento(int dx, int dy);
    void movimientoNivelDos(int dx, int dy);
    void lanzarPoderGoku();

    //inicialiacion de animciones
    void iniciarMovimientoPoderGoku();
    void iniciarMovimientoSalto();
    void iniciarMovimientoAgacho();

    //verificaciones - actualizaciones
    bool verificarSuperCargado();
    void actualizarEnPlataforma();

signals:
    void bonificacionRecolectada(Bonificacion* bonificacion);
    void poderLanzado();
    void golpeRecibido();
    void jugadorMurio();
};

#endif // JUGADOR_H
