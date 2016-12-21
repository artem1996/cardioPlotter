#include "datagraph.h"

DataGraph::DataGraph() {}
DataGraph::DataGraph(QVector<double> &keys, QVector<double> &values)
{
    this->keys = keys;
    this->values = values;
}

void DataGraph::setKeys(QVector<double> &keys) {
    this->keys = keys;
}

void DataGraph::setValues(QVector<double> &values) {
    this->values = values;
}

QVector<double>& DataGraph::getKeys() {
    return this->keys;
}

QVector<double>& DataGraph::getValues() {
    return this->values;
}
