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
}

void Approximation::fileLoad()
{
    /*--- txt 파일 열기*/
    ifstream readFile("ApproximationPoints.txt");
    
    if(!readFile)
    {
        cout << "You can't load this file." << endl;
    }


}