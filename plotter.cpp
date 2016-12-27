#include "plotter.h"
#define LEGEND_WIDTH 10
#define GRID_STEP 10
#define MARGIN_LEFT 5
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
    int graphHeight = height() - mm2px(MARGIN_TOP + MARGIN_BOTTOM);
    int xSteps = graphWidth / mm2px(GRID_STEP);
    int ySteps = graphHeight / mm2px(GRID_STEP);
    xMax = (double) graphWidth / mm2px(GRID_STEP) * timeScale + xMin;
    double xStep = (xMax - xMin) / graphWidth * mm2px(GRID_STEP);

    char str[24];
    int offset;

    //Горизонтальные второстепенные линии и вертикальная легенда
    int mm = MARGIN_BOTTOM;
    offset = mm2px(mm);
    //sprintf(str, "%2.2f", yMin);
    //painter.drawText(0, height() - offset - mm2px(GRID_STEP) / 2, mm2px(MARGIN_LEFT), mm2px(GRID_STEP + MARGIN_BOTTOM) / 2, Qt::AlignHCenter | Qt::AlignVCenter, QString(str));
    for (int i = 0; i < ySteps; i++) {
        offset = mm2px(mm += GRID_STEP);
        painter.drawLine(mm2px(MARGIN_LEFT), height() - offset, width() - mm2px(MARGIN_RIGHT), height() - offset);
        //sprintf(str, "%2.2f", yMin + ++i * yStep);
        //painter.drawText(0, height() - offset - mm2px(GRID_STEP) / 2, mm2px(MARGIN_LEFT), mm2px(GRID_STEP), Qt::AlignHCenter | Qt::AlignVCenter, QString(str));
    }

    //Вертикальные второстепенные линии и горизонтальная легенда
    mm = MARGIN_LEFT;
    offset = mm2px(mm);
    sprintf(str, "%3g", xMin);
    painter.drawText(offset / 2, height() - mm2px(MARGIN_BOTTOM), mm2px(GRID_STEP + MARGIN_LEFT) / 2, mm2px(MARGIN_BOTTOM), Qt::AlignHCenter | Qt::AlignVCenter, QString(str));
    for (int i = 0; i < xSteps;) {
        offset = mm2px(mm += GRID_STEP);
        painter.drawLine(offset, mm2px(MARGIN_TOP), offset, height() - mm2px(MARGIN_BOTTOM));
        sprintf(str, "%3.2f", xMin + ++i * xStep);
        painter.drawText(offset - mm2px(GRID_STEP) / 2, height() - mm2px(MARGIN_BOTTOM), mm2px(GRID_STEP), mm2px(MARGIN_BOTTOM), Qt::AlignHCenter | Qt::AlignVCenter, QString(str));
    }
    painter.end();
}

void Plotter::drawGraphs() {
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));

    //Ищем левую границу
    double koef = (xMin - xValues[0]) / (xValues[xValues.size() - 1] - xValues[0]);
    int xLeft;
    if(koef < 0) { xLeft = 0; } else {
        if(koef <= 1) {
            xLeft = xValues.size() * koef;
            while(xLeft < xValues.size() - 1 && xValues[xLeft] < xMin)
                xLeft++;
            while(xLeft > 0 && xValues[xLeft - 1] > xMin)
                xLeft--;
        }
    }

    //std::cerr << koef << " " << xLeft << " " << xValues[xLeft] << tempGraph.getValues()[xLeft] << "\n";

    //Ищем правую границу
    koef = (xMax - xValues[0]) / (xValues[xValues.size() - 1] - xValues[0]);
    int xRight;
    if(koef >= 0) {
        if(koef > 1) { xRight = xValues.size() - 1; } else {
            xRight = (xValues.size() - 1) * koef;
            while(xRight > 0 && xValues[xRight] > xMax)
                xRight--;
            while(xRight < xValues.size() - 1 && xValues[xRight + 1] < xMax)
                xRight++;
        }
    }

    int pxLeft = mm2px(MARGIN_LEFT);
    int pxWidth = width() - mm2px(MARGIN_RIGHT + MARGIN_LEFT);
    int pxBottom = height() - mm2px(MARGIN_BOTTOM);
    int pxTop = mm2px(MARGIN_TOP);
    int pxHeight = height() - mm2px(MARGIN_TOP + MARGIN_BOTTOM);
    int startNode;
    for(startNode = 0; startNode < nodes.size() && nodes[startNode] < xLeft + 1; startNode++);
    double ratioX = pxWidth / (xMax - xMin);
    double ratioY = mm2px(GRID_STEP) / scale;

    for(int i = 0; i < graphs.size(); i++) {
        painter.setPen(QPen(QColor(i % 3 == 0 ? 255 - i / 3 * 50 : 0,i % 3 == 1 ? 255 - i / 3 * 50 : 0,i % 3 == 2 ? 255 - i / 3 * 50 : 0))); // Устанавливаем цвет графика
        QVector<double> tempGraph = graphs[i].getValues();
        int nullPoint = pxBottom - (double) pxHeight / (graphs.size()) * (i + 0.5);
        double middleLine = (graphs[i].getMax() + graphs[i].getMin()) / 2;

        //std::cerr << graphs[i].getMax() << " " << graphs[i].getMin() << "\n";// << xValues[xRight] << tempGraph.getValues()[xRight] << "\n";

        int backX = ratioX * (xValues[xLeft] - xMin) + pxLeft;
        int backY = nullPoint - ratioY * (tempGraph[xLeft] - middleLine);

        int node = startNode;
        int nodeR = mm2px(3) / 2;
        for(int j = xLeft + 1; j <= xRight; j++) {
            int X = ratioX * (xValues[j] - xMin) + pxLeft;
            int Y = nullPoint - ratioY * (tempGraph[j] - middleLine);
            if(Y < pxBottom && Y > pxTop) painter.drawLine(backX, backY, X, Y); else continue;
            if(node < nodes.size() && nodes[node] == j) {
                painter.drawArc(X - nodeR, Y - nodeR , nodeR*2, nodeR*2, 0, 5760);
                node++;
            }
            backX = X;
            backY = Y;
        }
    }
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    for(; startNode < nodes.size() && nodes[startNode] < xRight; startNode++) {
        int X = ratioX * (xValues[nodes[startNode]] - xMin) + pxLeft;
        painter.drawText(X - mm2px(5), 0, mm2px(10), mm2px(MARGIN_TOP), Qt::AlignHCenter | Qt::AlignVCenter, QString("SMTH\0"));
    }
    painter.end();
}

void Plotter::addGraph(DataGraph *graph) {
    graphs.push_back(*graph);
}

void Plotter::mouseMoveEvent(QMouseEvent *event) {
    if(fabs(oldX - event->x()) < mm2px(5)) return;
    double cost = (xMax - xMin) / (width() - mm2px(MARGIN_LEFT + MARGIN_RIGHT)) * (oldX - event->x());
    xMin += cost;
    oldX = event->x();
    repaint();
    isMove = true;
}

void Plotter::mousePressEvent(QMouseEvent *event) {
    oldX = event->x();
    isMove = false;
}

void Plotter::mouseReleaseEvent(QMouseEvent *event) {
    if(isMove) {
        isMove = false;
        return;
    }
    int X = event->x() - mm2px(MARGIN_LEFT);
    double position = (double) X / mm2px(GRID_STEP) * timeScale + xMin;
    int xPoint = position / (xValues[1] - xValues[0]);
    std::cerr << "xPoint:  " << xPoint << "\n";
    int node;
    for(node = 0; node < nodes.size() && nodes[node] < xPoint; node++);
    if(node >= nodes.size()) node = nodes.size() - 1;
    if (event->button() == Qt::LeftButton) {
        nodes.insert(node, xPoint);
    } else if(event->button() == Qt::RightButton) {
        int diap = (double) mm2px(2) / mm2px(GRID_STEP) * timeScale / (xValues[1] - xValues[0]);
        //std::cerr << node << " " << diap << "\n";
        if(node > 0 && xPoint - nodes[node - 1] < nodes[node] - xPoint) {
            node -= 1;
        }
        if(abs(xPoint - nodes[node]) < diap) {
            nodes.removeAt(node);
        }
    }
    repaint();
}

void Plotter::setScale(int scale) {
    this->scale = scale;
}

void Plotter::setTimeScale(double scale) {
    this->timeScale = scale;
}

void Plotter::setFrequency(int frequency) {
    xValues.clear();
    this->frequency = frequency;
    if(graphs.size() == 0) return;
    double timeStep = 1.0 / frequency;
    double length = graphs[0].getValues().size() * timeStep;
    for(double i = 0; i < length; i += timeStep)
        xValues.push_back(i);
}

void Plotter::setNodes(QVector<int> &nodes) {
    this->nodes = nodes;
}
