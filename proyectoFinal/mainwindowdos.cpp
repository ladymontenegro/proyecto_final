#include "mainwindowdos.h"
#include <QDesktopServices> ///////////
#include <QFile>            ///////////////////
#include <QGraphicsItem>
#include <QGraphicsProxyWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QVector>
#include "QGraphicsPixmapItem"
#include "enemigo.h"
#include "jugador.h"

MainWindowDos::MainWindowDos(QWidget *parent)
    : QMainWindow(parent)
    , goku(nullptr)
    , roshi(nullptr)
{
    // Crear el espacio central
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Layout principal vertical para toda la ventana
    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Crear escena
    scene = new QGraphicsScene(this);

    // Crear vista
    view = new QGraphicsView(scene, this);

    // Eliminar barras de desplazamiento
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Agregar vista al layout principal
    mainLayout->addWidget(view);

    // Iniciar nivel 2
    nivel2();
}

//----------------- METODOS PARA LA CARGA DEL NIVEL DOS -----------------

void MainWindowDos::nivel2()
{
    //CONFIGURAR LA VISTA
    resize(1000, 600);

    QWidget *lifeBarContainer = new QWidget();
    lifeBarContainer->setFixedHeight(50); //le damos altura
    lifeBarContainer->setStyleSheet("background-color: transparent;");

    QHBoxLayout *lifeBarLayout = new QHBoxLayout(lifeBarContainer);
    lifeBarLayout->setSpacing(450);
    lifeBarLayout->setContentsMargins(10, 5, 40, 5);

    QHBoxLayout *barraGokuLayout = new QHBoxLayout();
    barraGokuLayout->setSpacing(0);
    barraGokuLayout->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *barraRoshiLayout = new QHBoxLayout();
    barraRoshiLayout->setSpacing(0);
    barraRoshiLayout->setContentsMargins(0, 0, 0, 0);

    QPixmap imagenRoshi(":/multimedia/barraRoshi.png");
    for (int i = 0; i < 5; i++) {
        lifeBarRoshiLabels[i] = new QLabel(lifeBarContainer);
        lifeBarRoshiLabels[i]->setFixedSize(250, 50);

        // Obtener el sprite correspondiente
        int y = i * 100;
        QPixmap sprite = imagenRoshi.copy(0, y, 500, 100);
        QPixmap scaled = sprite.scaled(250, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        lifeBarRoshiLabels[i]->setPixmap(scaled);
        lifeBarRoshiLabels[i]->setVisible(false); //inicialmente ocultos
        barraRoshiLayout->addWidget(lifeBarRoshiLabels[i]);
    }

    QPixmap imagenGoku(":/multimedia/barraGoku.png");
    for (int i = 0; i < 5; i++) {
        lifeBarGokuLabels[i] = new QLabel(lifeBarContainer);
        lifeBarGokuLabels[i]->setFixedSize(250, 50);

        // Obtener el sprite correspondiente
        int y = i * 100;
        QPixmap sprite = imagenGoku.copy(0, y, 500, 100);
        QPixmap scaled = sprite.scaled(250, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        lifeBarGokuLabels[i]->setPixmap(scaled);
        lifeBarGokuLabels[i]->setVisible(false); //inicialmente ocultos
        barraGokuLayout->addWidget(lifeBarGokuLabels[i]);
    }

    lifeBarLayout->addStretch();
    lifeBarLayout->addLayout(barraGokuLayout);
    lifeBarLayout->addLayout(barraRoshiLayout);
    lifeBarLayout->addStretch();

    QGraphicsProxyWidget *proxyWidget = new QGraphicsProxyWidget();
    proxyWidget->setWidget(lifeBarContainer);
    scene->addItem(proxyWidget);
    proxyWidget->setPos(0, 10);

    QPixmap imagenCueva(":/multimedia/cueva.png");
    QGraphicsPixmapItem *cueva = new QGraphicsPixmapItem(imagenCueva);
    cueva->setZValue(-100);
    scene->addItem(cueva);
    scene->setSceneRect(cueva->boundingRect());
    mainLayout->addWidget(view);
    crearPlataformas();

    //AGREGAR PERSONAJES
    //agregar a Goku
    QPixmap spriteGoku(":/multimedia/goku.png");
    goku = new Jugador(spriteGoku, 36, 248, 64, 64, 50, 50);
    scene->addItem(goku);
    goku->setFocus();
    goku->setPuntoReinicio(36, 336);
    goku->setPos(goku->x, goku->y);

    connect(goku, &Jugador::vidaCambiada, this, [=](int) { actualizarBarraVidaGoku(); });

    //agregar a Roshi
    QPixmap spriteRoshi(":/multimedia/roshi.png");
    roshi = new Enemigo(spriteRoshi, 848, 248, 64, 64, 50, 50);
    scene->addItem(roshi);
    roshi->setPos(roshi->x, roshi->y);
    roshi->setPlataformas(plataformasDerecha);
    roshi->setJugadorObjetivo(goku);

    //inicializar barras
    actualizarBarraVidaGoku();
    actualizarBarraVidaRoshi();
}

void MainWindowDos::crearPlataformas()
{
    QVector<QRectF> posicionesPlataformas = {{0, 296, 312, 32},
                                             {0, 384, 236, 32},
                                             {0, 468, 400, 32},
                                             {768, 296, 228, 32},
                                             {608, 388, 388, 32},
                                             {500, 468, 496, 32}};

    for (auto &datosPlataforma : posicionesPlataformas) {
        QGraphicsRectItem *plataforma = new QGraphicsRectItem(datosPlataforma);
        plataforma->setBrush(Qt::NoBrush);
        plataforma->setPen(Qt::NoPen);
        //plataforma->setBrush(QColor("#6d2045"));
        //plataforma->setPen(QColor("#FFFFFF"));
        //plataforma->setFlag(QGraphicsItem::ItemIsMovable);
        //plataforma->setOpacity(0.5);
        scene->addItem(plataforma);
        if (plataforma && plataforma->rect().x() > 495) {
            plataformasDerecha.append(plataforma->rect());
        }
    }
}

void MainWindowDos::actualizarBarraVidaGoku()
{
    //ocultar todos los labels primero
    for (int i = 0; i < 5; i++) {
        lifeBarGokuLabels[i]->setVisible(false);
    }

    int estado = 0;

    if (goku->getCargaVida() >= 4)
        estado = 4;
    else if (goku->getCargaVida() >= 3)
        estado = 3;
    else if (goku->getCargaVida() >= 2)
        estado = 2;
    else if (goku->getCargaVida() >= 1)
        estado = 1;
    else
        estado = 0;

    //mostrar solo el label correspondiente
    lifeBarGokuLabels[estado]->setVisible(true);
}

void MainWindowDos::actualizarBarraVidaRoshi()
{
    //ocultar todos los labels primero
    for (int i = 0; i < 5; i++) {
        lifeBarRoshiLabels[i]->setVisible(false);
    }

    int estado = 0;

    if (roshi->getCargaVida() >= 4)
        estado = 4;
    else if (roshi->getCargaVida() >= 3)
        estado = 3;
    else if (roshi->getCargaVida() >= 2)
        estado = 2;
    else if (roshi->getCargaVida() >= 1)
        estado = 1;
    else
        estado = 0;

    //mostrar solo el label correspondiente
    lifeBarRoshiLabels[estado]->setVisible(true);
}
