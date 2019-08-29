/*
 * BIBLIOTECA DE OPERAÇÕES MATRICIAIS
 */

#ifndef MATRIX_H
#define MATRIX_H

#include "constants.h"

class Matrix
{
public:
    //DEFAULT CONSTRUCTOR
    Matrix();
    //OVERLOAD CONSTRUCTOR
    Matrix(int nR, int nC);
    //DESTRUCTOR
    ~Matrix();

    //--------------------------------
    //THE DETERMINANT OF A MATRIX
    float det() const;

    //--------------------------------
    //VARIABLES
    int sizeR; //the number of rows
    int sizeC; //the number of columns
    float e[MAX][MAX];
};

#endif // MATRIX_H
