#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <plotter.h>
#include <iostream>
#include <datagraph.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Plotter* plotter;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setData(int numb, int diskr, QVector<double> *vect, QVector<int> *nodes);

private slots:
    void on_scale10_clicked();

    void on_scale20_clicked();

    void on_scale30_clicked();

    void on_scaleTime1_clicked();

    void on_scaleTime5_clicked();

    void on_scaleTime10_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
