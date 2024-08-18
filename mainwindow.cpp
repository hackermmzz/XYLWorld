#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("XYL-World");
    setWindowIcon(QIcon("src/Icon/Icon.png"));
    //resize(800,800);
    showFullScreen();
    GameInfo=ui->GameInfo;
    ExposureAdjust=ui->horizontalSlider;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    ui->palette->move(0,0);
    ui->palette->resize(width(),height());
   ////////////////////////
    auto*GameInfo=ui->GameInfo;
    GameInfo->move(0,0);
    GameInfo->resize(width(),height());
    GameInfo->hide();
    GameInfo->setAttribute(Qt::WA_TransparentForMouseEvents);
    GameInfo->setStyleSheet("background-color: rgba(255,255,255,120);");
    //////////////////////
    auto&ExposureAdjust=ui->horizontalSlider;
    ExposureAdjust->move(0,0);
    ExposureAdjust->resize(width(),20);
    ExposureAdjust->hide();
    QApplication::connect(ExposureAdjust, &QSlider::valueChanged, [&](){
        Exposure = ExposureAdjust->value()/10.0;
        });
}
