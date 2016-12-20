#include "plotter.h"
#define LEGEND_WIDTH 10
#define GRID_STEP 10
#include "stdio.h"

Plotter::Plotter(QWidget *parent) :
    QWidget(parent),
    paintBuffer(size())
{
}

double Plotter::mm2px(double mm) {
    return mm * (this->physicalDpiY() / 25.4);
}

void Plotter::paintEvent(QPaintEvent *event)
{
    paintBuffer.fill(Qt::white);
    QPainter painter(this);
    painter.drawPixmap(0, 0, this->width(), this->height(), paintBuffer);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    int mm = LEGEND_WIDTH;
    int offset = mm2px(mm);
    painter.drawLine(offset, 0, offset, height());
    painter.drawLine(offset, 0, offset - 5, 20);
    painter.drawLine(offset, 0, offset + 5, 20);

    painter.drawLine(0, height() - offset, width(), height() - offset);
    painter.drawLine(width(), height() - offset, width() - 20, height() - offset + 5);
    painter.drawLine(width(), height() - offset, width() - 20, height() - offset - 5);

    painter.setPen(QPen(QColor(170, 170, 170), 1, Qt::DashLine, Qt::FlatCap));
    int xSteps = 0;
    offset = mm2px(mm += GRID_STEP);
    while (offset < height()) {

        painter.drawLine(mm2px(LEGEND_WIDTH - 2), height() - offset, width(), height() - offset);
        offset = mm2px(mm += GRID_STEP);
    }
    offset = mm2px(mm = LEGEND_WIDTH + GRID_STEP);
    while (offset < width()) {
        xSteps++;
        painter.drawLine(offset, 0, offset, height() - mm2px(LEGEND_WIDTH - 2));
        offset = mm2px(mm += GRID_STEP);
    }

    xMin = 0;
    xMax = 10;
    double xStep = (xMax - xMin) / xSteps;
    double xTemp = xMin;
    offset = mm2px(mm = LEGEND_WIDTH + GRID_STEP);
    while (offset < width()) {
        char str[24];
        xTemp += xStep;
        sprintf(str, "%2.2f", xTemp);
        painter.drawText(offset - mm2px(GRID_STEP) / 2, height() - mm2px(LEGEND_WIDTH - 2), mm2px(GRID_STEP), mm2px(LEGEND_WIDTH - 2), Qt::AlignHCenter | Qt::AlignVCenter, QString(str));
        offset = mm2px(mm += GRID_STEP);
    }


    painter.end();
}
