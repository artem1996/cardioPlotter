#ifndef DATAGRAPH_H
#define DATAGRAPH_H
#include <QVector>

class DataGraph
{
    QVector<double> values;
    double min;
    double max;
public:
    DataGraph();
    DataGraph(QVector<double> &values);
    void setValues(QVector<double> &values);
    double getMin();
    QVector<double>& getValues();
    double getMax();
};

#endif // DATAGRAPH_H
