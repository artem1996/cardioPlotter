#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <fstream>
#define BUF_SIZE 32

// Метод для чтения строки из файла. Пропускает пустые строки и возвращает первую не пустую или EOF.
void getNotEmptyLine(std::ifstream& file, char* str) {
    do {
    file.getline(str, BUF_SIZE);
    } while (str[0] == '\n' || str[0] == 0);
}


int main(int argc, char *argv[])
{
    int graphCount = 0; // Количество графиков

    // Открытие файла с графиками
    std::ifstream fil("/home/note/Diplom/cardioPlotter/Signal1_ECG.m", std::ios::in);
    if (!fil.is_open()) { // если файл не открыт
            std::cout << "404 Not Found!\n"; // сообщить об этом
            return -1;
    } else {
        std::cerr << "Otkryto!\n";
    }

    char *buffer = new char[BUF_SIZE]; // Буфер для сложных строк (те, которые впоследствии нужно будет разбирать

    // Разбор первой строки. Получение количества графиков
    getNotEmptyLine(fil, buffer);
    graphCount = atoi(strstr(buffer, "lead_num  =") + 11);

    // Разбор второй строки. Получение частоты дискретизации
    getNotEmptyLine(fil, buffer);
    int descr = atoi(strstr(buffer, "samp_rate =") + 11);


    QVector<double> graphics[graphCount]; // Хранилище графиков. Хранятся только значения. Ключи высчитываются отдельно исходя из частоты дискретизации
    QVector<int> nodes; // Хранилище точек. Хранятся только ключи. Точки общие для всех графиков.

    // Цикл чтения данных графиков из файла
    for(int i = 0; i < graphCount; i++) {
        getNotEmptyLine(fil, buffer);       // Пропустить строку заголовка графика
        double temp;                        // Переменная для записи считанного значения
        while(fil >> temp) {                // Пока не будет обнаружена ошибка чтения (закрывающая скобка), вектор текущего графика будет набиваться значениями
            graphics[i].push_back(temp);
        }
        fil.clear(std::ios_base::goodbit ); // Сброс ошибки чтения
        getNotEmptyLine(fil, buffer);       // Пропустить строку закрытия массива значений (закрывающая скобка)
    }
    fil.close();    // Закрыть файл графиков.

    // Открытие файла точек
    std::ifstream nodesFile("/home/note/Diplom/cardioPlotter/Signal1_Rpeaks.m", std::ios::in);
    if (!nodesFile.is_open()) { // если файл не открыт
            std::cout << "404 Not Found!\n"; // сообщить об этом
            return -1;
    } else {
        std::cerr << "Otkryto!\n";
    }

    /* Файл содержит по набору точек для каждого графика. Однако было решено использовать только один.
    Поэтому здесь происходит считывание только первого набора точек.*/

    getNotEmptyLine(nodesFile, buffer); // Пропутить строку - заголовок набора

    // Считывание точек первого набора
    int temp;
    while(nodesFile >> temp) {
        nodes.push_back(temp);
    }

    // Сбросить ошибки и пропустить закрывающую строку. Не обязательно, т.к. дальнейшего чтения не будет
    nodesFile.clear(std::ios_base::goodbit );
    getNotEmptyLine(nodesFile, buffer);
    nodesFile.close();  // Закрыть файл точек.

    // Запуск графического модуля и передача ему входных данных
    QApplication a(argc, argv);
    MainWindow w;
    w.setData(graphCount, descr, graphics, nodes);
    w.show();

    return a.exec();
}
