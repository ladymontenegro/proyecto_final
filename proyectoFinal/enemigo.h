#ifndef ENEMIGO_H
#define ENEMIGO_H
#include "Personaje.h"
#include "jugador.h"

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
    void setJugadorObjetivo(Jugador *jugador);

public slots:
    void moverEntrePlataformas();
    void movimientoGolpe();
    void setPlataformas(const QVector<QRectF> &listaPlataformas);
    void disparar();

private:
    QVector<QRectF> plataformas;
    QTimer *timerMovimiento;
    QTimer *timerDisparo;
    QTimer *timerMovimientoGolpe;
    QTimer* timerSalto;
    Jugador *jugadorObjetivo;
    void iniciarMovimientoGolpe();
    void animarSalto();
    unsigned short contadorspriteMovimientoGolpe;
    unsigned short posicionGolpeX;
    int tiempoSalto;
    int plataformaActual;
    bool piedraActiva;
    bool puedeSaltarEntrePlataformas = true;
    QPointF origenSalto;
    QPointF destinoSalto;

signals:
    void golpeRecibido();
};

#endif // ENEMIGO_H
