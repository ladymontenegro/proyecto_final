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

private slots:
    void mover();

private:
    QTimer *timerMovimiento;
    unsigned short velocidad;
    unsigned short direccion; // 0: derecha, 1: izquierda
    QPixmap spriteAtaque;
};

#endif // ATAQUE_H
