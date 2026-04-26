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
    /*--- u, l 동적할당 ---*/
    u = new double[NumberOfConstraint];
    double *l = new double[NumberOfConstraint];

    u[0] = 0;
    /*--- l 계산 ---*/
    for (int i = 0; i < NumberOfConstraint - 1; i++)
    {
        l[i] = sqrt(pow(InputPoint[i + 1].x - InputPoint[i].x, 2) + pow(InputPoint[i + 1].y - InputPoint[i].y, 2));
    }

    /*--- u 계산 ---*/
    for (int i = 1; i < NumberOfConstraint; i++)
    {
        u[i] = u[i - 1] + l[0];
    }
}

void Approximation::Normalization()
{
    Min_x = InputPoint[0].x;
    Min_y = InputPoint[0].y;
    Max_x = InputPoint[0].x;
    Max_y = InputPoint[0].y;

    for (int i = 0; i < NumberOfConstraint; i++)
    {
        /*--- 최솟값 찾기 ---*/
        if (Min_x > InputPoint[i].x)
        {
            Min_x = InputPoint[i].x;
        }
        if (Min_y > InputPoint[i].y)
        {
            Min_y = InputPoint[i].y;
        }
        /*--- 최댓값 찾기 ---*/
        if (Max_x < InputPoint[i].x)
        {
            Max_x = InputPoint[i].x;
        }
        if (Max_y < InputPoint[i].y)
        {
            Max_y = InputPoint[i].y;
        }
        if (Max_u < u[i])
        {
            Max_u = u[i];
        }
    }

    dx = Max_x - Min_x;
    dy = Max_y - Min_y;

    for (int i = 0; i < NumberOfConstraint; i++)
    {
        /*--- Constraint 정규화 ---*/
        Matrix[i][0] = (Matrix[i][0] - Min_x) / dx;
        Matrix[i][1] = (Matrix[i][1] - Min_y) / dy;

        /*--- Chord Length 정규화 ---*/
        u[i] = u[i] / Max_u;
    }
}

void Approximation::Axb()
{
    /*--- 전치행렬 matrix 동적할당 ---*/
    double **TransposedMatrix = new double *[2];
    for (int i = 0; i <= 1; i++)
    {
        TransposedMatrix[i] = new double[NumberOfConstraint];
    }

    /*--- 전치 행렬 값 저장 ---*/
    for (int i = 0; i < NumberOfConstraint; i++)
    {
        TransposedMatrix[0][i] = Matrix[i][0];
        TransposedMatrix[1][i] = Matrix[i][1];
    }

    /*--- 계산된 행렬 동적할당 ---*/
    double **ATA = new double *[2];
    for (int i = 0; i < 2; i++)
    {
        ATA[i] = new double[2];
    }

    /*--- 행렬 계산 ---*/
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            ATA[i][j] = 0;

            for (int k = 0; k < NumberOfConstraint; k++)
            {
                ATA[i][j] += TransposedMatrix[i][k] * Matrix[k][j];
            }
        }
    }
}