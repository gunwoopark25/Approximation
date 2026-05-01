#include "Approximation.h"

int main()
{
    Approximation A;

    A.fileLoad();
    A.ChordLength();
    A.Normalization();
    A.makeBernsteinMatrix();
    A.calculateApproximation();
    A.setCP();
    A.BezierCurve();
    A.solveNormalization();

    return 0;
}