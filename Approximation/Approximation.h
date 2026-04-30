#pragma once

#include "Point2D.h"

class Approximation
{
public:
    /*--- 변수 ---*/
    int Degree;
    int Parameter;
    int R;
    int NumberOfConstraint;

    double Min_x;
    double Min_y;
    double Max_x;
    double Max_y;
    double Max_u;

    double dx;
    double dy;

    double *u;

    /*--- 2D ---*/
    Point2D *InputPoint;

    /*--- Matrix ---*/
    double **Matrix;
    double **BernsteinMatrix;


    /*--- 함수 ---*/
    void inputData();
    void fileLoad();
    void ChordLength();
    void Normalization();
    void makeBernsteinMatrix();
};
