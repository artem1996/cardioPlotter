#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>
#include <QPainter>

class Plotter : public QWidget
{
    Q_OBJECT

    double px2dp();
    double dp2px();
    double mm2px(double mm);
    double yMin, yMax, xMin, xMax;

    QPixmap paintBuffer;

    void drawGrid();

public:
    explicit Plotter(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);

signals:

public slots:
};

#endif // PLOTTER_H
