#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    plotter = new Plotter();
    ui->scale10->setChecked(true);
    ui->scaleTime10->setChecked(true);
    plotter->setScale(10);
    plotter->setTimeScale(1);
    ui->gridLayout->addWidget(plotter, 0,0,1,1);
    //std::cout << plotter->width() << " " << plotter->height() << " " << plotter->physicalDpiY() << " " << plotter->logicalDpiY() << "\n";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setData(int numb, int diskr, QVector<double> *vect, QVector<int> &nodes) {
    for(int i = 0; i < numb; i++) {
        plotter->addGraph(new DataGraph(vect[i])); // Устанавливаем данные
    }
    plotter->setFrequency(diskr);
    plotter->setNodes(nodes);
    repaint();
}


void MainWindow::on_scale10_clicked()
{
    plotter->setScale(10);
    repaint();
}

void MainWindow::on_scale20_clicked()
{
    plotter->setScale(30);
    repaint();
}

void MainWindow::on_scale30_clicked()
{
    plotter->setScale(50);
    repaint();
}

void MainWindow::on_scaleTime1_clicked()
{
    plotter->setTimeScale(0.01);
    repaint();
}

void MainWindow::on_scaleTime5_clicked()
{
    plotter->setTimeScale(0.1);
    repaint();
}

void MainWindow::on_scaleTime10_clicked()
{
    plotter->setTimeScale(1.0);
    repaint();
}
