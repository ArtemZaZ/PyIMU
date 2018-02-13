//
// Created by artem on 09.02.18.
//

#include "MajvikFilter.h"

void MajvikFilter::updateFilterIterator(Quaternion a, Quaternion w, float time)
{
    a = normalize(a);

    f = {2.f*(q.x*q.z - q.w*q.y) - a.x,
         2.f*(q.w*q.x + q.y*q.z) - a.y,
         2.f*(0.5f - q.x*q.x - q.y*q.y) - a.z};
    J[0][0] = -2.f*q.y;
    J[0][1] = 2.f*q.x;
    J[0][2] = 0.f;
    J[1][0] = 2.f*q.z;
    J[1][1] = 2.f*q.w;
    J[1][2] = -4.f*q.x;
    J[2][0] = -2.f*q.w;
    J[2][1] = 2.f*q.z;
    J[2][2] = -4.f*q.y;
    J[3][0] = 2.f*q.x;
    J[3][1] = 2.f*q.y;
    J[3][2] = 0.f;

    quatHatDot.w = J[0][0]*f.x + J[0][1]*f.y + J[0][2]*f.z;
    quatHatDot.x = J[1][0]*f.x + J[1][1]*f.y + J[1][2]*f.z;
    quatHatDot.y = J[2][0]*f.x + J[2][1]*f.y + J[2][2]*f.z;
    quatHatDot.z = J[3][0]*f.x + J[3][1]*f.y + J[3][2]*f.z;
    quatHatDot = normalize(quatHatDot);

    quatDotOmega = mul(scale(q, 0.5f), w);
    q.w = q.w + (quatDotOmega.w - beta*quatHatDot.w)*time;
    q.x = q.x + (quatDotOmega.x - beta*quatHatDot.x)*time;
    q.y = q.y + (quatDotOmega.y - beta*quatHatDot.y)*time;
    q.z = q.z + (quatDotOmega.z - beta*quatHatDot.z)*time;
    q = normalize(q);
}

MajvikFilter::MajvikFilter()
{
    q = {1.f, 0.f, 0.f, 0.f};
}


MajvikFilter::MajvikFilter(float b)
{
    q = {1.f, 0.f, 0.f, 0.f};
    beta =  b;
}

Vector MajvikFilter::getEulerAngle()
{
    return quatToEulerAngle(q);
}

void MajvikFilter::setBeta(float b)
{
    beta = b;
}
