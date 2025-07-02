#include "jugador.h"
#include <QGraphicsItem>
#include <QDebug>
#include "bonificacion.h"
#include "qgraphicsscene.h"

Jugador::Jugador(QPixmap _hojaSprite)
    : Personaje(_hojaSprite)
{
    x = 187;
    y = 87;
    superCargado = false;
    setFlag(QGraphicsItem::ItemIsFocusable);
    hojaPoderGoku.load(":/multimedia/poderGoku.png");
    timerPoderGoku = new QTimer(this);
    connect(timerPoderGoku, &QTimer::timeout, this, &Jugador::poderGoku);
    timerPoderGoku->setInterval(100);
}

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
        iniciarPoderGoku();
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

void Jugador::iniciarPoderGoku()
{
    contadorSpritePoderGoku = 0; // Reinicia la animacion al inicio
    poderGoku();                 // Muestra el primer frame inmediatamente
    timerPoderGoku->start();     // Inicia el timer para el resto de la animacion
}

void Jugador::poderGoku()
{
    if (contadorSpritePoderGoku <= 7) {
        //determina medidas
        int ancho = 66;
        int posicionPoderX = 0;

        if (contadorSpritePoderGoku == 6) {
            ancho = 283;
            posicionPoderX = 410;
        } else if (contadorSpritePoderGoku == 7) {
            ancho = 283;
            posicionPoderX = 693;
        } else {
            posicionPoderX = contadorSpritePoderGoku * ancho;
        }

        QPointF posicionActual = pos();

        spritePoderGoku = hojaPoderGoku.copy(posicionPoderX, 0, ancho, 128);
        QPixmap spriteEscalado;

        if (ancho == 283) {
            spriteEscalado = spritePoderGoku.scaled(70, 32);
        } else {
            spriteEscalado = spritePoderGoku.scaled(22, 32);
        }

        //espejar el sprite
        if (ultimaDireccion == 1) {
            spriteEscalado = spriteEscalado.transformed(QTransform().scale(-1, 1));
        }
        setPixmap(spriteEscalado);
        if (ultimaDireccion == 1) {
            //calcular diferencia de anchos
            qreal anchoActual = boundingRect().width(); //este es el sprite antes de cambiar al otro
            qreal nuevoAncho = (ancho == 283) ? 70 : 22; //este es el del proximo, si no es de los ultimos, no habra diferencia
            qreal diferencia = nuevoAncho - anchoActual;

            //mover el personaje a la derecha para compensar
            setPos(posicionActual.x() - diferencia, posicionActual.y());
        }
        contadorSpritePoderGoku++;

    } else {
        timerPoderGoku->stop();
        mostrarSpriteQuieto();
    }
}
