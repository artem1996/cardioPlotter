#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    plotter = new Plotter();
    ui->gridLayout->addWidget(plotter, 0,0,1,1);
    std::cout << plotter->width() << " " << plotter->height() << " " << plotter->physicalDpiY() << " " << plotter->logicalDpiY() << "\n";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setData(int numb, int diskr, QVector<double> *vect) {
    QVector<double> aX;
    for(int i = 0; i < vect[0].size(); i++) {
        aX.push_back(i * (1.0 / diskr));
    }
    plotter->addGraph(new DataGraph(aX, vect[0])); // Устанавливаем данные
    repaint();
}

