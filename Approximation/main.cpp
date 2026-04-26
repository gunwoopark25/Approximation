#include "Approximation.h"

int main()
{
    Approximation A;

    A.inputData();
    A.fileLoad();
    A.ChordLength();
    A.Normalization();
    A.Axb();

    return 0;
}