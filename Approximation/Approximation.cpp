#include "Approximation.h"
#include "Point2D.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;

/*--- 함수 정의 ---*/
void Approximation::inputData()
{
    Degree = 3;
    Parameter = 10;
    R = 5;

    NumberOfConstraint = Degree + 2;
}

void Approximation::fileLoad()
{
    /*--- txt 파일 열기*/
    ifstream readFile("ApproximationPoints.txt");

    /*--- 호출이 안됬을 경우 ---*/
    if (!readFile)
    {
        cout << "You can't load this file." << endl;
        return;
    }

    InputPoint = new Point2D[NumberOfConstraint];
    cout << "Input Point" << endl;
    for (int i = 0; i < NumberOfConstraint; i++)
    {
        readFile >> InputPoint[i].x;
        readFile >> InputPoint[i].y;

        cout << InputPoint[i].x << " " << InputPoint[i].y << endl;
    }

    /*--- 계산할 Matrix 동적할당 ---*/
    Matrix = new double *[NumberOfConstraint];

    for (int i = 0; i < NumberOfConstraint; i++)
    {
        Matrix[i] = new double[2];
    }
    /*--- 값 저장 ---*/
    for (int i = 0; i < NumberOfConstraint; i++)
    {
        Matrix[i][0] = InputPoint[i].x;
        Matrix[i][1] = InputPoint[i].y;
    }
}

void Approximation::ChordLength()
{
    double *u = new double[NumberOfConstraint];
    double *l = new double[NumberOfConstraint];

    u[0] = 0;
    /*--- l 계산 ---*/
    for (int i = 0; i < NumberOfConstraint-1; i++)
    {
        l[i] = sqrt(pow(InputPoint[i+1].x - InputPoint[i].x,2)+pow(InputPoint[i+1].y - InputPoint[i].y,2));
    }

    /*--- u 계산 ---*/
    for (int i = 1; i < NumberOfConstraint; i++)
    {
        u[i] = u[i-1]+l[0];
    }
}