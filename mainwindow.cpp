#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m = new meteo();
    connect(m, SIGNAL(gotowe()), this, SLOT(gotowe()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::gotowe()
{
    ui->label->setPixmap(QPixmap::fromImage(m->nowy));
}
