#include "ataque.h"
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QList>
#include <QGraphicsView>
#include "obstaculo.h"
#include "jugador.h"

Ataque::Ataque(Personaje* propietario,
               QPixmap _spriteAtaque,
               QPointF inicioPosicion,
               unsigned short _direccion)
    : propietario(propietario),
    spriteAtaque(_spriteAtaque),
    timerMovimiento(nullptr),
    velocidad(15),
    velocidadX(0),
    velocidadYInicial(0),
    gravedad(0),
    tiempo(0),
    direccion(_direccion),
    posicionInicial(inicioPosicion),
    destinoFinal(QPointF()) // no se usa en rectilíneo, lo dejamos como (0,0)
{
    // Reflejar sprite si va hacia la izquierda
    if (direccion == 1) {
        spriteAtaque = spriteAtaque.transformed(QTransform().scale(-1, 1));
    }

    setPixmap(spriteAtaque);
    setPos(inicioPosicion);

    // Crear y conectar el timer para movimiento rectilíneo
    timerMovimiento = new QTimer(this);
    connect(timerMovimiento, &QTimer::timeout, this, &Ataque::mover);
    timerMovimiento->start(40);
}


Ataque::Ataque(Personaje* propietario,
               QPixmap _spriteAtaque,
               QPointF inicioPosicion,
               QPointF _destino,
               unsigned short _direccion)
    : propietario(propietario),
    spriteAtaque(_spriteAtaque),
    timerMovimiento(nullptr),
    velocidad(15),
    velocidadX(0),
    velocidadYInicial(0),
    gravedad(0),
    tiempo(0),
    direccion(_direccion),
    posicionInicial(inicioPosicion),
    destinoFinal(_destino)
{
    // Reflejar sprite si va hacia la izquierda
    if (direccion == 1) {
        spriteAtaque = spriteAtaque.transformed(QTransform().scale(-1, 1));
    }

    setPixmap(spriteAtaque);
    setPos(inicioPosicion);

    // Crear y conectar el timer para movimiento parabólico
    timerMovimiento = new QTimer(this);
    connect(timerMovimiento, &QTimer::timeout, this, &Ataque::mover);
    timerMovimiento->start(40);
}


Ataque::~Ataque()
{
    if (timerMovimiento) {
        timerMovimiento->stop();
        delete timerMovimiento;
        timerMovimiento = nullptr;
    }

    qDebug() << "Destructor de Ataque";
}

Personaje* Ataque::getPropietario() const{
    return propietario;
}

void Ataque::mover()
{
    //por si no choco con nada y salio de escena
    if (!scene() || !scene()->views().first()->rect().contains(pos().toPoint())) {
        if (scene()) scene()->removeItem(this);
        deleteLater();
        return;
    }

    //mueve el poder en la direccion correcta
    qreal nuevoX = x() + (direccion == 0 ? velocidad : -velocidad);
    setPos(nuevoX, y());

    //verificar colisiones
    QList<QGraphicsItem *> colisionados = collidingItems();
    for (QGraphicsItem *item : colisionados) {
        //si choca con un cuadrado
        if (Obstaculo *cuadrado = dynamic_cast<Obstaculo *>(item)) {
            cuadrado->destruirConResiduos(scene());
            emit destruido();
            scene()->removeItem(this);
            this->deleteLater();
            return;
        }

        //si choca con un muro
        if (item->type() == QGraphicsRectItem::Type) {
            emit destruido();
            scene()->removeItem(this);
            this->deleteLater();
            return;
        }
    }
}

void Ataque::usarMovimientoParabolico() {
    tiempo = 0;
    posicionInicial = pos();

    direccion = (destinoFinal.x() < posicionInicial.x()) ? 1 : 0;
    if (direccion == 1) {
        setPixmap(spriteAtaque.transformed(QTransform().scale(-1, 1)));
    }

    qreal dx = destinoFinal.x() - posicionInicial.x();
    qreal dy = destinoFinal.y() - posicionInicial.y();

    //estimamos un tiempo de vuelo en segundos
    qreal tiempoVuelo = 2.3;

    //convertimos ese tiempo a pasos de 0.4s
    qreal pasos = tiempoVuelo / 0.4;

    velocidadX = dx / pasos;

    gravedad = 1.8;

    //velocidad inicial vertical calculada
    velocidadYInicial = (dy - 0.5 * gravedad * pasos * pasos) / pasos;

    qDebug() << "Lanzando piedra hacia: " << destinoFinal << " desde: " << posicionInicial;
    qDebug() << "vx:" << velocidadX << ", vyInicial:" << velocidadYInicial;


    // Cambio de conexión
    disconnect(timerMovimiento, &QTimer::timeout, this, &Ataque::mover);
    connect(timerMovimiento, &QTimer::timeout, this, &Ataque::moverParabolico);
}

void Ataque::moverParabolico() {
    if (!scene() || !scene()->views().first()->rect().contains(pos().toPoint())) {
        emit destruido();
        if (scene()) scene()->removeItem(this);
        deleteLater();
        return;
    }

    tiempo += 0.4;

    //movimiento parabolico
    qreal x = posicionInicial.x() + velocidadX * tiempo;
    qreal y = posicionInicial.y() + velocidadYInicial * tiempo + 0.5 * gravedad * tiempo * tiempo;

    setPos(x, y);

    //verificar colisiones
    QList<QGraphicsItem *> colisionados = collidingItems();
    for (QGraphicsItem *item : colisionados) {
        if (Obstaculo *cuadrado = dynamic_cast<Obstaculo *>(item)) {
            cuadrado->destruirConResiduos(scene());
            emit destruido();

            scene()->removeItem(this);
            deleteLater();
            return;
        }

        if (item->type() == QGraphicsRectItem::Type) {
            QRectF plataformaRect = item->boundingRect().translated(item->pos());
            QRectF ataqueRect = boundingRect().translated(pos());

            qreal margen = 6.0; //tolerancia para impactos verticales

            bool colisionDesdeArriba =
                ataqueRect.bottom() >= plataformaRect.top() &&
                ataqueRect.bottom() <= plataformaRect.top() + margen &&
                velocidadYInicial >= 0;

            if (colisionDesdeArriba) {
                emit destruido();
                scene()->removeItem(this);
                deleteLater();
                return;
            }
        }

        if (dynamic_cast<Jugador *>(item)) {
            emit destruido();
            scene()->removeItem(this);
            deleteLater();
            return;
        }
    }
}
