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

    /*--- 2D ---*/
    Point2D *InputPoint;

    /*--- Matrix ---*/
    double **Matrix;

    /*--- 함수 ---*/
    void inputData();
    void fileLoad();
    void ChordLength();
};
