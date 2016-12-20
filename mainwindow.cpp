#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    plotter = new Plotter();
    ui->gridLayout->addWidget(plotter, 0,0,1,1);
    std::cout << plotter->width() << " " << plotter->height() << " " << plotter->physicalDpiY() << plotter->logicalDpiY() << "\n";
}

MainWindow::~MainWindow()
{
    delete ui;
}
