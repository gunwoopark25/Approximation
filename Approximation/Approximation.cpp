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
    Parameter = 1000;
    R = 5;

    NumberOfConstraint = Degree + 2;

    POC_Size = Degree + 1;
    for (int i = Degree; i > 0; i--)
    {
        POC_Size = POC_Size + i;
    }
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
        u[i] = u[i - 1] + l[i-1];
    }
}

void Approximation::Normalization()
{
    Min_x = InputPoint[0].x;
    Min_y = InputPoint[0].y;
    Max_x = InputPoint[0].x;
    Max_y = InputPoint[0].y;
    Max_u = u[0];

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

void Approximation::makeBernsteinMatrix()
{
    /*--- Bernstein Matrix 동적 할당 ---*/
    BernsteinMatrix = new double* [NumberOfConstraint];
    for (int i = 0; i < NumberOfConstraint; i++)
    {
        BernsteinMatrix[i] = new double[Degree + 1];
    }

    /*--- Bernstein Matrix value 대입 ---*/
    for (int k = 0; k < NumberOfConstraint; k++)
    {
        for (int i = 0; i <= Degree; i++)
        {
            for (int j = 0; j <= Degree; j++)
            {
                int Degreep = 1;
                int jp = 1;
                int Degree_jp = 1;

                for (int k = 1; k <= Degree; k++)
                {
                    Degreep *= k;
                }
                for (int k = 1; k <= j; k++)
                {
                    jp *= k;
                }

                int Degree_j = Degree - j;
                for (int k = 1; k <= Degree_j; k++)
                {
                    Degree_jp *= k;
                }
                double nCr = Degreep / (jp * Degree_jp);

                /*--- Bernstein Polynomial ---*/
                BernsteinMatrix[k][j] = nCr * pow(1 - u[k], Degree - j) * pow(u[k], j);
            }
        }
    }
}

void Approximation::calculateApproximation()
{
    /*--- A Matrix 세팅 ---*/
    double **A_Matrix = new double*[NumberOfConstraint - 2];
    for (int i = 0; i < NumberOfConstraint - 2; i++)
    {
        A_Matrix[i] = new double[Degree - 1];
    }

    for (int i = 0; i < NumberOfConstraint - 2; i++)
    {
        for (int j = 0; j < Degree - 1; j++)
        {
            A_Matrix[i][j] = BernsteinMatrix[i + 1][j + 1];
        }
    }

    /*--- b Matrix 세팅 ---*/
    double **b_Matrix = new double* [NumberOfConstraint - 2];
    for (int i = 0; i < NumberOfConstraint - 2; i++)
    {
        b_Matrix[i] = new double[2];
    }

    for (int i = 0; i < NumberOfConstraint - 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            b_Matrix[i][j] = Matrix[i + 1][j] - Matrix[0][j]*BernsteinMatrix[i + 1][0] - Matrix[NumberOfConstraint - 1][j] * BernsteinMatrix[i + 1][Degree];
        }
    }

    /*--- A_T X A ---*/
    double **ATA_Matrix = new double* [Degree - 1];
    for (int i = 0; i < Degree - 1; i++)
    {
        ATA_Matrix[i] = new double[Degree - 1];
    }

    /*--- 행렬 곱 계산 ---*/
    for (int i = 0; i < Degree - 1; i++)        
    {
        for (int j = 0; j < Degree - 1; j++)   
        {
            double Sum = 0;

            for (int k = 0; k < NumberOfConstraint - 2; k++)  
            {
                Sum += A_Matrix[k][i] * A_Matrix[k][j];
            }

            ATA_Matrix[i][j] = Sum;
        }
    }
    

    /*--- A_T X b ---*/
    double **ATb_Matrix = new double* [Degree - 1];
    for (int i = 0; i < Degree - 1; i++)
    {
        ATb_Matrix[i] = new double[2];
    }

    for (int i = 0; i < Degree - 1; i++) 
    {
        for (int j = 0; j < 2; j++)             
        {
            double Sum = 0;

            for (int k = 0; k < NumberOfConstraint - 2; k++) 
            {
                Sum += A_Matrix[k][i] * b_Matrix[k][j];
            }

            ATb_Matrix[i][j] = Sum;
        }
    }

    /*--- 단위행렬 생성 ---*/
    double** IdentityMatrix = new double* [Degree - 1];
    for (int i = 0; i < Degree - 1; i++)
    {
        IdentityMatrix[i] = new double[Degree - 1];
    }

    for (int i = 0; i < Degree - 1; i++)
    {
        for (int j = 0; j < Degree - 1; j++)
        {
            if (i == j)
            {
                IdentityMatrix[i][j] = 1;
            }
            else
            {
                IdentityMatrix[i][j] = 0;
            }
        }
    }

    double** GaussJordonMatrix = new double* [Degree - 1];
    for (int i = 0; i < Degree - 1; i++)
    {
        GaussJordonMatrix[i] = new double[Degree - 1 + Degree - 1];
    }

    for (int i = 0; i < Degree - 1; i++)
    {
        for (int j = 0; j < Degree - 1; j++)
        {
            GaussJordonMatrix[i][j] = ATA_Matrix[i][j];

            GaussJordonMatrix[i][j + Degree - 1] = IdentityMatrix[i][j];
        }
    }

    cout << "Matrix for GaussJordon" << endl;
    for (int i = 0; i < Degree - 1; i++)
    {
        for (int j = 0; j < Degree - 1 + Degree - 1; j++)
        {
            cout << GaussJordonMatrix[i][j] << " ";
        }
        cout << endl;
    }

    for (int i = 0; i < Degree - 1; i++)
    {
        double pivot = GaussJordonMatrix[i][i];
        for (int j = 0; j < 2 * (Degree - 1); j++)
        {
            GaussJordonMatrix[i][j] /= pivot;
        }

        for (int k = 0; k < Degree - 1; k++)
        {
            if (k != i)
            {
                double factor = GaussJordonMatrix[k][i];
                if (factor != 0)
                {
                    for (int j = 0; j < 2 * (Degree - 1); j++)
                    {
                        GaussJordonMatrix[k][j] -= GaussJordonMatrix[i][j] * factor;
                    }
                }
            }
        }
    }

    cout << " GaussJordon Matrix" << endl;
    for (int i = 0; i < Degree - 1; i++)
    {
        for (int j = 0; j < Degree - 1 + Degree - 1; j++)
        {
            cout << GaussJordonMatrix[i][j] << " ";
        }
        cout << endl;
    }

    double** ATA_Inverse_Matrix = new double* [Degree - 1];
    for (int i = 0; i < Degree - 1; i++)
    {
        ATA_Inverse_Matrix[i] = new double[Degree - 1];
    }

    for (int i = 0; i < Degree - 1; i++)
    {
        for (int j = 0; j < Degree - 1; j++)
        {
            ATA_Inverse_Matrix[i][j] = GaussJordonMatrix[i][j + Degree - 1];
        }
    }

    cout << "Matrix" << endl;
    for (int i = 0; i < Degree - 1; i++)
    {
        for (int j = 0; j < Degree - 1; j++)
        {
            cout << ATA_Inverse_Matrix[i][j] << " ";
        }
        cout << endl;
    }

    /*--- ATA_Inverse_Matrix X ATb_Matrix ---*/
    Result_Matrix = new double* [Degree - 1];
    for (int i = 0; i < Degree - 1; i++)
    {
        Result_Matrix[i] = new double[2];
    }

    for (int i = 0; i < Degree - 1; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            double Sum = 0;

            for (int k = 0; k < Degree - 1; k++)
            {
                Sum += ATA_Inverse_Matrix[i][k] * ATb_Matrix[k][j];
            }

            Result_Matrix[i][j] = Sum;
        }
    }

    cout << "Result Matrix" << endl;
    for (int i = 0; i < Degree - 1; i++)
    {
        for (int j = 0; j < Degree - 1; j++)
        {
            cout << Result_Matrix[i][j] << " ";
        }
        cout << endl;
    }
}

void Approximation::setCP()
{
    CP = new Point2D[Degree + 1];
    for (int i = 0; i < Degree + 1; i++)
    {
        if (i == 0)
        {
            CP[i].x = Matrix[0][0];
            CP[i].y = Matrix[0][1];
        }
        else if (i == Degree)
        {
            CP[i].x = Matrix[NumberOfConstraint - 1][0];
            CP[i].y = Matrix[NumberOfConstraint - 1][1];
        }
        else
        {
            CP[i].x = Result_Matrix[i-1][0];
            CP[i].y = Result_Matrix[i-1][1];
        }
    }

    cout << "=== CP ===" << endl;
    for (int i = 0; i < Degree + 1; i++)
    {
        cout << CP[i].x << " " << CP[i].y << endl;
    }
}

void Approximation::BezierCurve()
{
    Coordinate = new Point2D[POC_Size];
    All_POC = new Point2D[Parameter + 1];
    for (int k = 0; k <= Parameter; k++)
    {
        double t = (double)k / Parameter;

        for (int i = 0; i < Degree + 1; i++)
        {
            Coordinate[i].x = CP[i].x;
            Coordinate[i].y = CP[i].y;
        }

        int boundary = 0;

        for (int i = 1; i <= Degree; i++)
        {
            int n = Degree - i + 1;
            int start_X = boundary + 1;
            int end_X = boundary + n + 1;

            for (int X = start_X; X < end_X; X++)
            {
                Coordinate[X + n].x = (1 - t) * Coordinate[X - 1].x + t * Coordinate[X].x;
                Coordinate[X + n].y = (1 - t) * Coordinate[X - 1].y + t * Coordinate[X].y;
            }

            boundary += (Degree + 1 - i + 1);
        }

        int last_X = POC_Size - 1;
        All_POC[k].x = Coordinate[last_X].x;
        All_POC[k].y = Coordinate[last_X].y;
    }

    cout << "All POC Coordinate" << endl;
    for (int i = 0; i <= Parameter; i++)
    {
        cout << All_POC[i].x << " " << All_POC[i].y << endl;
    }
}

void Approximation::solveNormalization()
{
    for (int i = 0; i <= Degree; i++)
    {
        /*--- CP 정규화 해제 ---*/
        CP[i].x = CP[i].x * dx + Min_x;
        CP[i].y = CP[i].y * dy + Min_y;
    }

    for (int i = 0; i <= Parameter; i++)
    {
        /*--- All POC 정규화 해제 ---*/
        All_POC[i].x = All_POC[i].x * dx + Min_x;
        All_POC[i].y = All_POC[i].y * dy + Min_y;
    }

    cout << "--- CP Coordinate ---" << endl;
    for (int i = 0; i <= Degree; i++)
    {
        cout << CP[i].x << " " << CP[i].y << endl;
    }
    cout << "--- All POC Coordinate ---" << endl;
    for (int i = 0; i <= Parameter; i++)
    {
        cout << All_POC[i].x << " " << All_POC[i].y << endl;
    }
}

void Approximation::writePS()
{
    ofstream psFile("Curve.ps");

    if (!psFile)
    {
        std::cout << "You can't load this file" << endl;
        return;
    }

    /*--- ps 헤더 ---*/
    psFile << "%!PS" << endl;

    /*--- CP 라인 그리기 ---*/
    psFile << "newpath" << endl;
    for (int i = 0; i <= Degree; i++)
    {
        if (i == 0)
        {
            psFile << CP[i].x << " " << CP[i].y << " " << "moveto" << endl;
        }
        else
        {
            psFile << CP[i].x << " " << CP[i].y << " " << "lineto" << endl;
        }
    }
    psFile << "stroke" << endl;

    /*--- CP 원 그리기 ---*/
    for (int i = 0; i <= Degree; i++)
    {
        psFile << "newpath" << endl;
        if (i == 0)
        {
            psFile << CP[i].x << " " << CP[i].y << " " << R << " " << "0 360 arc" << endl;
            psFile << "fill" << endl;
        }
        else if (i == Degree)
        {
            psFile << CP[i].x << " " << CP[i].y << " " << R << " " << "0 360 arc" << endl;
            psFile << "fill" << endl;
        }
        else
        {
            psFile << CP[i].x << " " << CP[i].y << " " << R << " " << "0 360 arc" << endl;
            psFile << "gsave" << endl;
            psFile << "1 setgray" << " " << "fill" << " " << "grestore" << " " << "0 setgray" << endl;
            psFile << "stroke" << endl;
        }
    }

    /*--- All POC Line 그리기 ---*/
    psFile << "newpath" << endl;
    for (int i = 0; i <= Parameter; i++)
    {
        if (i == 0)
        {
            psFile << All_POC[i].x << " " << All_POC[i].y << " " << "moveto" << endl;
        }
        else
        {
            psFile << All_POC[i].x << " " << All_POC[i].y << " " << "lineto" << endl;
        }
    }
    psFile << "stroke" << endl;

    /*--- POC 원 그리기 ---*/
    

    for (int i = 0; i <= Degree; i++)
    {
        if (i == 0)
        {
            psFile << "newpath" << endl;
            psFile << InputPoint[i].x << " " << InputPoint[i].y << " " << R << " " << "0 360 arc" << endl;
            psFile << "stroke" << endl;
        }
        else if (i == Degree)
        {
            psFile << "newpath" << endl;
            psFile << InputPoint[i].x << " " << InputPoint[i].y << " " << R << " " << "0 360 arc" << endl;
            psFile << "stroke" << endl;
        }
        else
        {
            psFile << "newpath" << endl;
            psFile << InputPoint[i].x << " " << InputPoint[i].y << " " << R << " " << "0 360 arc" << endl;
            psFile << "gsave" << endl;
            psFile << "1 setgray" << " " << "fill" << " " << "grestore" << " " << "0 setgray" << endl;
            psFile << "stroke" << endl;
        }
    }

    psFile << "showpage" << endl;

    psFile.close();


}