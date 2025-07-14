#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "personaje.h"
#include "jugador.h"
#include <QTimer>
#include <QVector>
#include <QRectF>
#include <QPointF>

class Enemigo : public Personaje {
    Q_OBJECT

public:
    // Constructor y destructor
    Enemigo(QPixmap _hojaSprite,
            unsigned short _x,
            unsigned short _y,
            unsigned short _anchoSprite,
            unsigned short _altoSprite,
            unsigned short _anchoSpriteEscalar,
            unsigned short _altoSpriteEscalar);

    ~Enemigo() override;

    // Métodos públicos
    void setJugadorObjetivo(Jugador *jugador);
    void setPlataformas(const QVector<QRectF> &listaPlataformas);
    void verificarColisiones();
    void perderVida();

public slots:
    void moverEntrePlataformas();
    void movimientoGolpe();
    void disparar();

signals:
    void enemigoMurio();

private:
    // Referencias
    Jugador *jugadorObjetivo = nullptr;
    QVector<QRectF> plataformas;
    bool fueAtacado();

    // Timers
    QTimer *timerMovimiento = nullptr;
    QTimer *timerDisparo = nullptr;
    QTimer *timerMovimientoGolpe = nullptr;
    QTimer *timerSalto = nullptr;
    QTimer *timerColisiones = nullptr;

    // Estado de animaciones y control
    bool piedraActiva = false;
    bool puedeSaltarEntrePlataformas = true;

    // Posiciones de salto
    QPointF origenSalto;
    QPointF destinoSalto;

    // Variables para animacion de golpe
    unsigned short contadorspriteMovimientoGolpe = 0;
    unsigned short posicionGolpeX = 0;

    // Logica de salto
    int tiempoSalto = 0;
    int plataformaActual = 0;

    // Funciones privadas
    void iniciarMovimientoGolpe();
    void animarSalto();
};

#endif // ENEMIGO_H
