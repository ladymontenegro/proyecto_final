#include "obstaculo.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QRandomGenerator>
#include <QTimer>

Obstaculo::Obstaculo(QRectF posicion, QGraphicsItem *parent)
    : QObject()
    , QGraphicsRectItem(posicion, parent)
{}

void Obstaculo::destruirConResiduos(QGraphicsScene *scene)
{
    //Calcula el punto central del cuadrado respecto a la escena
    QPointF origen = mapToScene(boundingRect().center());

    // Para crear varios residuos circulares y su fisica
    for (int i = 0; i < 10; ++i) {
        QGraphicsEllipseItem *residuo = new QGraphicsEllipseItem(0, 0, 3, 3);
        residuo->setBrush(QColor("#053270"));
        residuo->setPen(QColor("#053270"));
        residuo->setZValue(2); //Para poder que se muestre encima de otros objetos
        residuo->setPos(origen);
        scene->addItem(residuo);

        // Velocidad inicial aleatoria
        QPointF velocidad(QRandomGenerator::global()->bounded(-4, 5),
                          QRandomGenerator::global()->bounded(-25, -4));

        int *rebotes = new int(3); // rebotes permitidos
        QTimer *timer = new QTimer();

        QObject::connect(timer, &QTimer::timeout, [=]() mutable {
            velocidad.setY(velocidad.y() + 1.2); // "Gravedad"; es esa para que se vea bien
            residuo->moveBy(velocidad.x(), velocidad.y()); //Mueve en ambas direcciones

            if (residuo->y() >= 50) {
                if (*rebotes > 0) {
                    //rebote normal
                    velocidad.setY(-velocidad.y() * 0.5);
                    velocidad.setX(velocidad.x() * 0.7);
                    (*rebotes)--;
                }
            } else {
                if (residuo->scene()) {
                    residuo->scene()->removeItem(residuo);
                }
                delete residuo;
                delete rebotes;
                timer->stop();
                delete timer;
            }
        });

        timer->start(30); // 30 fps
    }

    // Finalmente, elimina el obstáculo
    scene->removeItem(this);
    deleteLater();
}
