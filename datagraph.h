#ifndef DATAGRAPH_H
#define DATAGRAPH_H
#include <QVector>

class DataGraph
{
    QVector<double> keys;
    QVector<double> values;
    QVector<int> nodes;
public:
    DataGraph();
    DataGraph(QVector<double> &keys, QVector<double> &values, QVector<int> &nodes);
    void setKeys(QVector<double> &keys);
    void setValues(QVector<double> &values);
    QVector<double>& getKeys();
    QVector<double>& getValues();
    QVector<int>& getNodes();
};

#endif // DATAGRAPH_H
