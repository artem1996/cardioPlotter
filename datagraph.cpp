#include "datagraph.h"

DataGraph::DataGraph() {}
DataGraph::DataGraph(QVector<double> &keys, QVector<double> &values, QVector<int> &nodes)
{
    this->keys = keys;
    this->values = values;
    this->nodes = nodes;
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

QVector<int>& DataGraph::getNodes() {
    return this->nodes;
}
