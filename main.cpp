#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <fstream>
#define BUF_SIZE 32

void getNotEmptyLine(std::ifstream& file, char* str) {
    do {
    file.getline(str, BUF_SIZE);
    } while (str[0] == '\n' || str[0] == 0);
}


int main(int argc, char *argv[])
{
    int graphCount = 0;
    std::ifstream fil("/home/note/Diplom/cardioPlotter/Signal1_ECG.m", std::ios::in);
    if (!fil.is_open()) { // если файл не открыт
            std::cout << "404 Not Found!\n"; // сообщить об этом
            return -1;
    } else {
        std::cerr << "Otkryto!\n";
    }
    char *lol = new char[BUF_SIZE];
    getNotEmptyLine(fil, lol);
    graphCount = atoi(strstr(lol, "lead_num  =") + 11);
    getNotEmptyLine(fil, lol);
    int descr = atoi(strstr(lol, "samp_rate =") + 11);
    QVector<double> gr[graphCount];
    QVector<int> nodes;
    for(int i = 0; i < graphCount; i++) {
        getNotEmptyLine(fil, lol);
        //std::cerr << lol << std::endl;
        double temp;
        while(fil >> temp) {
            gr[i].push_back(temp);
        }
        fil.clear(std::ios_base::goodbit );
        getNotEmptyLine(fil, lol);
    }
    fil.close();
    std::ifstream nodesFile("/home/note/Diplom/cardioPlotter/Signal1_Rpeaks.m", std::ios::in);
    if (!nodesFile.is_open()) { // если файл не открыт
            std::cout << "404 Not Found!\n"; // сообщить об этом
            return -1;
    } else {
        std::cerr << "Otkryto!\n";
    }
    getNotEmptyLine(nodesFile, lol);
    int temp;
    while(nodesFile >> temp) {
        nodes.push_back(temp);
    }
    nodesFile.clear(std::ios_base::goodbit );
    getNotEmptyLine(nodesFile, lol);
    nodesFile.close();
//    std::cout << graphCount << " " << descr << std::endl;
//    for(int i = 0; i < graphCount; i++)
//        std::cerr << gr[i].back() << std::endl;
    QApplication a(argc, argv);
    MainWindow w;
    w.setData(graphCount, descr, gr, nodes);
    w.show();

    return a.exec();
}
