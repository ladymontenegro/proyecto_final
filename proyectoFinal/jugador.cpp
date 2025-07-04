#include "jugador.h"
#include <QDebug>
#include <QGraphicsItem>
#include "ataque.h"
#include "bonificacion.h"
#include "qgraphicsscene.h"

Jugador::Jugador(QPixmap _hojaSprite)
    : Personaje(_hojaSprite)
{
    x = 267;
    y = 271;
    superCargado = false;
    setFlag(QGraphicsItem::ItemIsFocusable);
    hojaMovimientoPoderGoku.load(":/multimedia/movimientoPoderGoku.png");
    timerMovimientoPoderGoku = new QTimer(this);
    connect(timerMovimientoPoderGoku, &QTimer::timeout, this, &Jugador::movimientoPoderGoku);
    timerMovimientoPoderGoku->setInterval(100);
}

//***************** MOVIMIENTO *****************

void Jugador::keyPressEvent(QKeyEvent *event)
{
    if (!teclasPresionadas.contains(event->key())) {
        teclasPresionadas.append(event->key()); //para que guarde la tecla, si no esta repetida
    }

    switch (event->key()) {
    case Qt::Key_A:
        movimiento(-5, 0);
        movimientoSprite(2496);
        ultimaDireccion = 1;
        break;
    case Qt::Key_W:
        movimiento(0, -5);
        movimientoSprite(2432);
        ultimaDireccion = 2;
        break;
    case Qt::Key_S:
        movimiento(0, 5);
        movimientoSprite(2560);
        ultimaDireccion = 3;
        break;
    case Qt::Key_D:
        movimiento(5, 0);
        movimientoSprite(2624);
        ultimaDireccion = 0;
        break;
    case Qt::Key_Space:
        iniciarMovimientoPoderGoku();
        break;
    default:
        QGraphicsItem::keyPressEvent(event);
        break;
    }
}

void Jugador::keyReleaseEvent(QKeyEvent *event)
{
    teclasPresionadas.removeAll(event->key());

    //solo procesar teclas de movimiento
    int key = event->key();
    if (key != Qt::Key_A && key != Qt::Key_D && key != Qt::Key_W && key != Qt::Key_S) {
        return;
    }

    if (teclasPresionadas.isEmpty()) {
        mostrarSpriteQuieto();
    }
}

void Jugador::mostrarSpriteQuieto()
{
    //guardar el conteo actual de sprites
    int conteoAnterior = conteoSprite;

    switch(ultimaDireccion) {
    case 0:
        movimientoSprite(2368);
        break;
    case 1:
        movimientoSprite(2240);
        break;
    case 2:
        movimientoSprite(2176);
        break;
    case 3:
        movimientoSprite(2304);
        break;
    }
    //restaurar el conteo original y mantenerlo fijo
    conteoSprite = conteoAnterior;

    //crear el sprite estatico (primer frame)
    sprite = hojaSprite.copy(0, posicionY, anchoSprite, altoSprite);
    QPixmap spriteEscalado = sprite.scaled(22, 22);
    setPixmap(spriteEscalado);
}

void Jugador::movimiento(int dx, int dy)
{
    //posicion actual antes de intentar el movimiento
    qreal oldX = x;
    qreal oldY = y;

    //intenta mover al jugador
    setPos(x + dx, y + dy);

    //actualiza la nueva posicion
    x = pos().x();
    y = pos().y();

    QList<QGraphicsItem *> itemsChocados = collidingItems(); //obtiene los elementos con los que colisiona

    for (QGraphicsItem *item : itemsChocados) {
        if (item->type() == QGraphicsRectItem::Type) {
            //deshace el movimiento y pone la posicion anterior
            setPos(oldX, oldY);
            x = oldX; //actualiza variables miembro
            y = oldY;
            break;
        } else if (item->type() == QGraphicsPixmapItem::Type) {
            Bonificacion *bonificacion = dynamic_cast<Bonificacion *>(item);
            if (bonificacion) {
                scene()->removeItem(bonificacion);
                cargaSuper += bonificacion->getValorCarga();
                delete bonificacion;
            }
        }
    }
}

//***************** PODER *****************

void Jugador::iniciarMovimientoPoderGoku()
{
    contadorspriteMovimientoPoderGoku = 0; // Reinicia la animacion al inicio
    movimientoPoderGoku();                 // Muestra el primer frame inmediatamente
    timerMovimientoPoderGoku->start();     // Inicia el timer para el resto de la animacion
}

void Jugador::movimientoPoderGoku()
{
    if (contadorspriteMovimientoPoderGoku <= 6) {
        posicionXPoderGoku = 0;
        anchoSpriteMovimientoPoderGoku = 66; // Ancho por defecto
        posicionXPoderGoku = contadorspriteMovimientoPoderGoku * anchoSpriteMovimientoPoderGoku;
        spriteMovimientoPoderGoku = hojaMovimientoPoderGoku.copy(posicionXPoderGoku,
                                                                 0,
                                                                 anchoSpriteMovimientoPoderGoku,
                                                                 128);
        QPixmap spriteEscalado = spriteMovimientoPoderGoku.scaled(22, 32);

        //espejar el sprite
        if (ultimaDireccion == 1) {
            spriteEscalado = spriteEscalado.transformed(QTransform().scale(-1, 1));
        }

        setPixmap(spriteEscalado);
        contadorspriteMovimientoPoderGoku++;

    } else {
        timerMovimientoPoderGoku->stop();
        lanzarPoderGoku();
        mostrarSpriteQuieto();
    }
}

void Jugador::lanzarPoderGoku()
{
    QPointF inicio = {ultimaDireccion != 1 ? x + 20 : x - 20, y + 4};
    QPixmap sprite(":/multimedia/poderGoku.png");
    QPixmap spriteEscalado = sprite.scaled(30, 20);

    Ataque *ataqueGoku = new Ataque(spriteEscalado, inicio, ultimaDireccion);
    scene()->addItem(ataqueGoku);
}
