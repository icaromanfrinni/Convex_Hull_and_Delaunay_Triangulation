#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <cmath>
#include "Matrix.h"

using namespace std;

//DEFAULT CONSTRUCTOR
Matrix::Matrix()
{
    sizeR = 0;
    sizeC = 0;
    for (int i = 0; i < MAX; i++)
        for (int j = 0; j < MAX; j++)
            e[i][j] = 0.0;
}

//OVERLOAD CONSTRUCTOR
Matrix::Matrix(int nR, int nC)
{
    sizeR = nR;
    sizeC = nC;
    for (int i = 0; i < MAX; i++)
        for (int j = 0; j < MAX; j++)
            e[i][j] = 0.0;
}


//DESTRUCTOR
Matrix::~Matrix()
{

}

//------------------------------------------------------------------------------
//THE DETERMINANT OF A MATRIX
//------------------------------------------------------------------------------
float Matrix::det() const
{
    if (sizeR != sizeC) //verifica se a matrix é quadrada
    {
        cout << "\n!!! MATRIX MUST BE SQUARE !!!\n" << endl;
        exit(EXIT_FAILURE);
    }
    else {
        int n = sizeR; //pega a dimensão da matrix
        float det = 0.0;
        if (n == 1)
        {
            det = e[0][0];
            return det;
        }
        else if (n == 2) //se for 2x2, calcula o determinante direto
        {
            det = e[0][0] * e[1][1] - e[0][1] * e[1][0];
            return det;
        }
        else {
            for (int k = 0; k < n; k++)
            {
                Matrix subM(n - 1, n - 1); //Inicializa uma SubMatriz com DIM = n-1 da matriz corrente

                for (int i = 1; i < n; i++) //cada submatriz é formada pelos elementos abaixo da primeira linha
                    for (int j = 0; j < n; j++)
                    {
                        if (j < k) //não pega os elementos da coluna do coeficiente
                            subM.e[i - 1][j] = e[i][j];
                        else if (j > k) subM.e[i - 1][j - 1] = e[i][j];
                    }

                det += pow(-1, k) * e[0][k] * subM.det(); //calcula o determinante de cada submatriz e acumula
            }
            return det;
        }
    }
}
