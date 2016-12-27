#include "datagraph.h"

DataGraph::DataGraph() {}
DataGraph::DataGraph(QVector<double> &values)
{
    this->values = values;
    min = 999999;
    max = -999999;
    for(int i = 0; i < values.size(); i++) {
        if(min > values[i]) min = values[i];
        if(max < values[i]) max = values[i];
    }
}

void DataGraph::setValues(QVector<double> &values) {
    this->values = values;
    min = 999999;
    max = -999999;
    for(int i = 0; i < values.size(); i++) {
        if(min > values[i]) min = values[i];
        if(max < values[i]) max = values[i];
    }
}

double DataGraph::getMax() {
    return this->max;
}

QVector<double>& DataGraph::getValues() {
    return this->values;
}

double DataGraph::getMin() {
    return this->min;

}
