#include "Approximation.h"

void main()
{
    Approximation A;
    A.inputData();
    A.fileLoad();
    A.ChordLength();
    A.Normalization();
    A.makeBernsteinMatrix();
    A.calculateApproximation();
    A.setCP();
    A.BezierCurve();
    A.solveNormalization();
}