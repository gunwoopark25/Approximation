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
    int POC_Size;

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
    Point2D* CP;
    Point2D* Coordinate;
    Point2D* All_POC;

    /*--- Matrix ---*/
    double **Matrix;
    double **BernsteinMatrix;
    double** Result_Matrix;
    

    /*--- 함수 ---*/
    void inputData();
    void fileLoad();
    void ChordLength();
    void Normalization();
    void makeBernsteinMatrix();
    void calculateApproximation();
    void setCP();
    void BezierCurve();
    void solveNormalization();
};
