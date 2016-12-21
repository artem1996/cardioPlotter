#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
    int graphCount = 0;
    std::ifstream fil("/home/note/cardio/ECG_4000.m", std::ios::in);
    if (!fil.is_open()) { // если файл не открыт
            std::cout << "404 Not Found!\n"; // сообщить об этом
            return -1;
    } else {
        std::cerr << "Otkryto!\n";
    }
    char *lol = new char[32];
    fil.getline(lol, 32);
    graphCount = atoi(strstr(lol, "lead_num  =") + 11);
    fil.getline(lol, 32);
    std::cerr << lol << std::endl;
    int descr = atoi(strstr(lol, "samp_rate =") + 11);
    fil.getline(lol, 32);
    QVector<double> gr[graphCount];
    for(int i = 0; i < graphCount; i++) {
        fil.getline(lol, 32);
        int temp;
        fil >> temp;
        gr[i].push_back(temp/10);
        while(fil >> temp) {
            gr[i].push_back(temp);
        }
        fil.clear(std::ios_base::goodbit );
        fil.getline(lol, 32);
    }
    std::cout << graphCount << " " << descr << std::endl;
    for(int i = 0; i < graphCount; i++)
        std::cerr << gr[i].back() << std::endl;
    QApplication a(argc, argv);
    MainWindow w;
    w.setData(graphCount, descr, gr);
    w.show();

    return a.exec();
}
