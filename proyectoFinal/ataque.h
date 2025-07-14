#ifndef ATAQUE_H
#define ATAQUE_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QPointF>
#include "personaje.h"  // Asegúrate de incluir la clase base correctamente

class Ataque : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    // Constructor rectilíneo
    Ataque(Personaje* propietario,
           QPixmap _spriteAtaque,
           QPointF inicioPosicion,
           unsigned short _direccion);

    // Constructor parabólico
    Ataque(Personaje* propietario,
           QPixmap _spriteAtaque,
           QPointF inicioPosicion,
           QPointF _destino,
           unsigned short _direccion);

    virtual ~Ataque();
    Personaje* getPropietario() const;

    void usarMovimientoParabolico();
    void moverParabolico();

signals:
    void destruido();

private slots:
    void mover();

private:
    Personaje* propietario;
    QPixmap spriteAtaque;
    QTimer* timerMovimiento;

    // Movimiento y fisica
    double velocidad = 15.0;
    double velocidadX = 0.0;
    double velocidadYInicial = 0.0;
    double gravedad = 0.0;
    double tiempo = 0.0;

    // 0 = derecha, 1 = izquierda
    unsigned short direccion = 0;

    // Posiciones
    QPointF posicionInicial;
    QPointF destinoFinal;
};

#endif // ATAQUE_H

