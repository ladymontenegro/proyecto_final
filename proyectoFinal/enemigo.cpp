#include "enemigo.h"
#include "ataque.h"
#include <QGraphicsScene>
#include <QDebug>

// -------------------- Constructor y Destructor --------------------
Enemigo::Enemigo(QPixmap _hojaSprite,
                 unsigned short _x,
                 unsigned short _y,
                 unsigned short _anchoSprite,
                 unsigned short _altoSprite,
                 unsigned short _anchoSpriteEscalar,
                 unsigned short _altoSpriteEscalar)
    : Personaje(_hojaSprite,
                _x,
                _y,
                _anchoSprite,
                _altoSprite,
                _anchoSpriteEscalar,
                _altoSpriteEscalar),
    jugadorObjetivo(nullptr),
    contadorspriteMovimientoGolpe(0),
    posicionGolpeX(0),
    tiempoSalto(0),
    plataformaActual(0),
    piedraActiva(false)
{
    // Timer para moverse entre plataformas
    timerMovimiento = new QTimer(this);
    connect(timerMovimiento, &QTimer::timeout, this, &Enemigo::moverEntrePlataformas);
    timerMovimiento->start(80);  // Ajusta según velocidad deseada

    // Timer para disparar piedras
    timerDisparo = new QTimer(this);
    connect(timerDisparo, &QTimer::timeout, this, &Enemigo::disparar);
    timerDisparo->start(2000);  // Dispara cada 2 segundos

    // Timer para animar golpe
    timerMovimientoGolpe = new QTimer(this);
    connect(timerMovimientoGolpe, &QTimer::timeout, this, &Enemigo::movimientoGolpe);

    // Timer para animar salto
    timerSalto = new QTimer(this);
    connect(timerSalto, &QTimer::timeout, this, &Enemigo::animarSalto);
}

Enemigo::~Enemigo() {
    qDebug() << "Destructor de Enemigo";
    // Los QTimers se destruyen automáticamente porque tienen como padre a Enemigo
}

// -------------------- Métodos públicos --------------------

void Enemigo::perderVida() {
    cargaVida--;
    emit vidaCambiada(cargaVida);

    if (cargaVida <= 0) {
        emit enemigoMurio();
    }
}

void Enemigo::setJugadorObjetivo(Jugador *jugador) {
    jugadorObjetivo = jugador;
    timerDisparo->start(1200);
}

void Enemigo::setPlataformas(const QVector<QRectF> &listaPlataformas) {
    plataformas = listaPlataformas;

    if (!plataformas.isEmpty()) {
        setPos(plataformas[0].x() + 20, plataformas[0].y() - pixmap().height());
        plataformaActual = 0;
        puedeSaltarEntrePlataformas = true;

        if (!timerMovimiento) {
            timerMovimiento = new QTimer(this);
            connect(timerMovimiento, &QTimer::timeout, this, &Enemigo::moverEntrePlataformas);
        }
        timerMovimiento->start(1000);
    }
}

// -------------------- Movimiento entre plataformas --------------------

void Enemigo::moverEntrePlataformas() {
    if (plataformas.isEmpty() || !puedeSaltarEntrePlataformas)
        return;

    plataformaActual = (plataformaActual + 1) % plataformas.size();
    destinoSalto = QPointF(plataformas[plataformaActual].x() + 20,
                           plataformas[plataformaActual].y() - pixmap().height());
    origenSalto = pos();
    tiempoSalto = 0;

    if (!timerSalto) {
        timerSalto = new QTimer(this);
        connect(timerSalto, &QTimer::timeout, this, &Enemigo::animarSalto);
    }

    timerSalto->start(40);
}

void Enemigo::animarSalto() {
    const float duracionTotal = 20.0f;
    tiempoSalto++;

    float t = tiempoSalto / duracionTotal;

    if (t > 1.0f) {
        setPos(destinoSalto);
        timerSalto->stop();
        return;
    }

    float nuevoX = origenSalto.x() + t * (destinoSalto.x() - origenSalto.x());
    float altura = -50 * sin(M_PI * t);  // Pico en la mitad
    float nuevoY = origenSalto.y() + t * (destinoSalto.y() - origenSalto.y()) + altura;

    setPos(nuevoX, nuevoY);

    movimientoSprite(1728, 5); // Animación mientras salta
}

// -------------------- Ataque --------------------

void Enemigo::disparar() {
    if (piedraActiva || !jugadorObjetivo)
        return;

    piedraActiva = true;
    puedeSaltarEntrePlataformas = false;

    iniciarMovimientoGolpe();

    QPointF posicionJugador = jugadorObjetivo->sceneBoundingRect().center();
    QPointF posicionEnemigo = pos();
    unsigned short direccion = (posicionJugador.x() < posicionEnemigo.x()) ? 1 : 0;

    Ataque* piedra = new Ataque(this,
                                QPixmap(":/multimedia/piedra.png"),
                                posicionEnemigo,
                                posicionJugador,
                                direccion);
    piedra->usarMovimientoParabolico();

    connect(piedra, &Ataque::destruido, this, [this, piedra]() {
        // Solo reactivar si esta piedra fue la que se destruyó
        if (this->piedraActiva) {
            piedraActiva = false;
            puedeSaltarEntrePlataformas = true;
        }
        piedra->deleteLater();  // Asegurar eliminación
    });

    if (scene()) {
        scene()->addItem(piedra);
    }
}

// -------------------- Golpe cuerpo a cuerpo --------------------

void Enemigo::iniciarMovimientoGolpe() {
    contadorspriteMovimientoGolpe = 0;
    movimientoGolpe();
    timerMovimientoGolpe->start();
}

void Enemigo::movimientoGolpe() {
    if (contadorspriteMovimientoGolpe <= 5) {
        posicionGolpeX = contadorspriteMovimientoGolpe * anchoSprite;

        QPixmap spriteMovimientoGolpe = hojaSprite.copy(posicionGolpeX, 3264, anchoSprite, altoSprite);
        QPixmap spriteEscalado = spriteMovimientoGolpe.scaled(50, 50);

        setPixmap(spriteEscalado);
        contadorspriteMovimientoGolpe++;
    } else {
        timerMovimientoGolpe->stop();
    }
}

void Enemigo::detenerAtaques() {
    if (timerDisparo && timerDisparo->isActive())
        timerDisparo->stop();

    if (timerMovimientoGolpe && timerMovimientoGolpe->isActive())
        timerMovimientoGolpe->stop();

    if (timerSalto && timerSalto->isActive())
        timerSalto->stop();

    piedraActiva = false;

    qDebug() << "Ataques del enemigo detenidos.";
}

void Enemigo::detenerMovimiento() {
    if (timerMovimiento && timerMovimiento->isActive())
        timerMovimiento->stop();

    qDebug() << "Movimiento del enemigo detenido.";
}


