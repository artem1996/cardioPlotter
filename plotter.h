#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>
#include <QPainter>
#include <QVector>
#include <datagraph.h>

class Plotter : public QWidget
{
    Q_OBJECT

    double px2dp();
    double dp2px();
    double mm2px(double mm);
    double yMin, yMax, xMin, xMax, realYMin = 99999;
    int oldX, oldY;
    double scale;
    bool isMove = false;
    QVector<DataGraph> graphs;

    QPixmap paintBuffer;

    void drawGrid();
    void drawGraphs();
    void rescale();

public:
    explicit Plotter(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);
    void addGraph(DataGraph *graph);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void setScale(int scale);
signals:

public slots:
};

#endif // PLOTTER_H
