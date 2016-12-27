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
    double xMin, xMax;
    int oldX;
    double scale;
    double timeScale;
    bool isMove = false;
    int frequency;
    QVector<DataGraph> graphs;
    QVector<double> xValues;
    QVector<int> nodes;

    QPixmap paintBuffer;

    void drawGrid();
    void drawGraphs();

public:
    explicit Plotter(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);
    void addGraph(DataGraph *graph);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void setScale(int scale);
    void setTimeScale(double scale);
    void setFrequency(int frequency);
    void setNodes(QVector<int> &nodes);
signals:

public slots:
};

#endif // PLOTTER_H
