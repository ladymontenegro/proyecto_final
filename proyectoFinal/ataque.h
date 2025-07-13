#ifndef ATAQUE_H
#define ATAQUE_H
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QPixmap>
#include <QTimer>
#include <QTransform>

class Ataque : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Ataque(QPixmap _spriteAtaque, QPointF inicioPosicion, unsigned short _direccion);
    Ataque(QPixmap _spriteAtaque, QPointF inicioPosicion, QPointF destino, unsigned short _direccion);

    virtual ~Ataque();
    void usarMovimientoParabolico();

private slots:
    void mover();
    void moverParabolico();

private:
    qreal tiempo;
    qreal velocidadX;
    qreal velocidadYInicial;
    qreal gravedad;
    QPointF posicionInicial;
    QPointF destinoFinal;
    QTimer *timerMovimiento;
    unsigned short velocidad;
    unsigned short direccion; // 0: derecha, 1: izquierda
    QPixmap spriteAtaque;

signals:
    void destruido();
};

#endif // ATAQUE_H
