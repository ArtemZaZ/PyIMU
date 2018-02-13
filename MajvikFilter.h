//
// Created by artem on 09.02.18.
// NONE OPTIMIZATION
//

#ifndef PYTHONMODULE_MAJVIKFILTER_H
#define PYTHONMODULE_MAJVIKFILTER_H


#include "IMUlib.h"
#include <thread>

class MajvikFilter
{
private:
    Quaternion q;
    float beta;
    float J[4][3];  // Якобиан
    Vector f;       // целевая ф-ия     https://habrahabr.ru/post/255661/
    Quaternion quatHatDot;  // Ошибка
    Quaternion quatDotOmega;    // угловая скорость
    std::thread thr();

public:
    MajvikFilter();             // конструктор

    MajvikFilter(float b);      // конструктор

    void updateFilterIterator(Quaternion a, Quaternion w, float time);  // обновления значения

    Vector getEulerAngle();     // получение углов

    void setBeta(float b);      // установка коэффициента
};


#endif //PYTHONMODULE_MAJVIKFILTER_H
