#ifndef OBSTACULO_H
#define OBSTACULO_H
#include <QGraphicsItem>
#include <QObject>

class Obstaculo : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Obstaculo(QRectF posicion, QGraphicsItem *parent = nullptr);
    void destruirConResiduos(QGraphicsScene *scene);
};

#endif // OBSTACULO_H
