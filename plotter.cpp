#include "plotter.h"
#define LEGEND_WIDTH 10
#define GRID_STEP 10
#define MARGIN_LEFT 10
#define MARGIN_RIGHT 10
#define MARGIN_TOP 10
#define MARGIN_BOTTOM 10
#include "stdio.h"
#include "iostream"

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
    drawGrid();
}

void Plotter::drawGrid() {
    QPainter painter(this);
    paintBuffer.fill(Qt::white);
    painter.drawPixmap(0, 0, this->width(), this->height(), paintBuffer);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));

    //Вертикальные границы
    painter.drawLine(mm2px(MARGIN_LEFT), mm2px(MARGIN_TOP), mm2px(MARGIN_LEFT), height() - mm2px(MARGIN_BOTTOM));
    painter.drawLine(width() - mm2px(MARGIN_RIGHT), mm2px(MARGIN_TOP), width() - mm2px(MARGIN_RIGHT), height() - mm2px(MARGIN_BOTTOM));
    //Горизонтальные границы
    painter.drawLine(mm2px(MARGIN_LEFT), mm2px(MARGIN_TOP), width() - mm2px(MARGIN_RIGHT), mm2px(MARGIN_TOP));
    painter.drawLine(mm2px(MARGIN_LEFT), height() - mm2px(MARGIN_BOTTOM), width() - mm2px(MARGIN_RIGHT), height() - mm2px(MARGIN_BOTTOM));

    //Задаём цвет второстепернных линий
    painter.setPen(QPen(QColor(170, 170, 170), 1, Qt::DashLine, Qt::FlatCap));

    xMax = 10; yMax = 20; xMin = 0; yMin = -5;

    int graphWidth = width() - mm2px(MARGIN_LEFT + MARGIN_RIGHT);
    int xSteps = graphWidth / mm2px(GRID_STEP);
    double xStep = (xMax - xMin) / graphWidth * mm2px(GRID_STEP);
    int graphHeight = height() - mm2px(MARGIN_TOP + MARGIN_BOTTOM);
    int ySteps = graphHeight / mm2px(GRID_STEP);
    double yStep = (yMax - yMin) / graphHeight * mm2px(GRID_STEP);

    char str[24];
    int offset;

    //Горизонтальные второстепенные линии и вертикальная легенда
    int mm = MARGIN_BOTTOM;
    offset = mm2px(mm);
    sprintf(str, "%2.2f", yMin);
    painter.drawText(0, height() - offset - mm2px(GRID_STEP) / 2, mm2px(MARGIN_LEFT), mm2px(GRID_STEP + MARGIN_BOTTOM) / 2, Qt::AlignHCenter | Qt::AlignVCenter, QString(str));
    for (int i = 0; i < ySteps;) {
        offset = mm2px(mm += GRID_STEP);
        painter.drawLine(mm2px(MARGIN_LEFT), height() - offset, width() - mm2px(MARGIN_RIGHT), height() - offset);
        sprintf(str, "%2.2f", yMin + ++i * yStep);
        painter.drawText(0, height() - offset - mm2px(GRID_STEP) / 2, mm2px(MARGIN_LEFT), mm2px(GRID_STEP), Qt::AlignHCenter | Qt::AlignVCenter, QString(str));
    }

    //Вертикальные второстепенные линии и горизонтальная легенда
    mm = MARGIN_LEFT;
    offset = mm2px(mm);
    sprintf(str, "%2.2f", xMin);
    painter.drawText(offset / 2, height() - mm2px(MARGIN_BOTTOM), mm2px(GRID_STEP + MARGIN_LEFT) / 2, mm2px(MARGIN_BOTTOM), Qt::AlignHCenter | Qt::AlignVCenter, QString(str));
    for (int i = 0; i < xSteps;) {
        offset = mm2px(mm += GRID_STEP);
        painter.drawLine(offset, mm2px(MARGIN_TOP), offset, height() - mm2px(MARGIN_BOTTOM));
        sprintf(str, "%2.2f", xMin + ++i * xStep);
        painter.drawText(offset - mm2px(GRID_STEP) / 2, height() - mm2px(MARGIN_BOTTOM), mm2px(GRID_STEP), mm2px(MARGIN_BOTTOM), Qt::AlignHCenter | Qt::AlignVCenter, QString(str));
    }
    painter.end();
}

void Plotter::addGraph(DataGraph graph) {
    graphs.push_back(graph);
}
