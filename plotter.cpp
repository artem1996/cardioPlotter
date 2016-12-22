#include "plotter.h"
#define LEGEND_WIDTH 10
#define GRID_STEP 10
#define MARGIN_LEFT 15
#define MARGIN_RIGHT 5
#define MARGIN_TOP 10
#define MARGIN_BOTTOM 10
#include "stdio.h"
#include "iostream"
#include <QMouseEvent>
#include <cmath>
#include <QWheelEvent>

Plotter::Plotter(QWidget *parent) :
    QWidget(parent),
    paintBuffer(size())
{
    xMax = 11; yMax = 100; xMin = 1; yMin = -100;
}

double Plotter::mm2px(double mm) {
    return mm * (this->physicalDpiY() / 25.4);
}

void Plotter::paintEvent(QPaintEvent *event)
{
    drawGrid();
    drawGraphs();
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

void Plotter::drawGraphs() {
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));

    for(int i = 0; i < graphs.size(); i++) {
        DataGraph tempGraph = graphs[i];

        //Ищем левую границу
        double koef = (xMin - tempGraph.getKeys()[0]) / (tempGraph.getKeys()[tempGraph.getKeys().size() - 1] - tempGraph.getKeys()[0]);
        int xLeft;
        if(koef < 0) { xLeft = 0; } else {
            if(koef > 1) { continue; } else {
                xLeft = tempGraph.getKeys().size() * koef;
                while(tempGraph.getKeys()[xLeft] < xMin)
                    xLeft++;
                while(tempGraph.getKeys()[xLeft - 1] > xMin)
                    xLeft--;
            }
        }

        std::cerr << koef << " " << xLeft << " " << tempGraph.getKeys()[xLeft] << tempGraph.getValues()[xLeft] << "\n";

        //Ищем правую границу
        koef = (xMax - tempGraph.getKeys()[0]) / (tempGraph.getKeys()[tempGraph.getKeys().size() - 1] - tempGraph.getKeys()[0]);
        int xRight;
        if(koef < 0) { continue; } else {
            if(koef > 1) { xRight = tempGraph.getKeys().size() - 1; } else {
                xRight = tempGraph.getKeys().size() * koef;
                while(tempGraph.getKeys()[xRight] > xMax)
                    xRight--;
                while(tempGraph.getKeys()[xRight + 1] < xMax)
                    xRight++;
            }
        }

        std::cerr << koef << " " << xRight << " " << tempGraph.getKeys()[xRight] << tempGraph.getValues()[xRight] << "\n";

        int pxLeft = mm2px(MARGIN_LEFT);
        int pxWidth = width() - mm2px(MARGIN_RIGHT + MARGIN_LEFT);
        int pxBottom = height() - mm2px(MARGIN_BOTTOM);
        int pxHeight = height() - mm2px(MARGIN_TOP + MARGIN_BOTTOM);

        double oneXWeigth;
        int backX = (tempGraph.getKeys()[xLeft] - xMin) / (xMax - xMin) * pxWidth + pxLeft;
        int backY = pxBottom - (tempGraph.getValues()[xLeft] - yMin) / (yMax - yMin) * pxHeight;
        for(int j = xLeft + 1; j <= xRight; j++) {
            int X = (tempGraph.getKeys()[j] - xMin) / (xMax - xMin) * pxWidth + pxLeft;
            int Y = pxBottom - (tempGraph.getValues()[j] - yMin) / (yMax - yMin) * pxHeight;
            painter.drawLine(backX, backY, X, Y);
            backX = X;
            backY = Y;
        }


    }


    painter.end();
}

void Plotter::addGraph(DataGraph *graph) {
    graphs.push_back(*graph);
}

void Plotter::mouseMoveEvent(QMouseEvent *event) {
    static int i = 0;
    if(fabs(oldX - event->x()) < mm2px(5)) return;
    double cost = (xMax - xMin) / (width() - mm2px(MARGIN_LEFT + MARGIN_RIGHT)) * (oldX - event->x());
    xMax += cost;
    xMin += cost;
    repaint();
    //std::cerr << event->x() << " " << event->y() << " " << i++ << "\n";
    oldX = event->x();
}

void Plotter::mousePressEvent(QMouseEvent *event) {
    oldX = event->x();
    oldY = event->y();
}

void Plotter::wheelEvent(QWheelEvent *event) {
    int numDegrees = event -> delta() / 8;
    int numTicks = numDegrees / 15;
    if(numTicks == 0) return;
    int x = event->x();
    int y = event->y();
    if(x <= mm2px(MARGIN_LEFT + 1) || x >= width() - mm2px(MARGIN_RIGHT + 1) || y < mm2px(MARGIN_TOP + 1) || y > height() - mm2px(MARGIN_BOTTOM + 1))
        return;
    x -= mm2px(MARGIN_LEFT);
    int widthGraph = width() - mm2px(MARGIN_LEFT + MARGIN_RIGHT);
    double inc = (xMax - xMin) * 0.1 * numTicks;
    if(numTicks < 0) { //+++
        xMin -= inc * x / widthGraph;
        xMax += inc * (widthGraph - x) / widthGraph;
    } else { //---
        xMin -= inc * x / widthGraph;
        xMax += inc * (widthGraph - x) / widthGraph;
    }

    repaint();
}
