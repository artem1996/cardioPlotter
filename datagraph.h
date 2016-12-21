#ifndef DATAGRAPH_H
#define DATAGRAPH_H
#include <QVector>

class DataGraph
{
    QVector<double> keys;
    QVector<double> values;
public:
    DataGraph();
    DataGraph(QVector<double> &keys, QVector<double> &values);
    void setKeys(QVector<double> &keys);
    void setValues(QVector<double> &values);
    QVector<double>& getKeys();
    QVector<double>& getValues();
};

#endif // DATAGRAPH_H
