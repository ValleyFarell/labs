#include<stdio.h>
#include<stdlib.h>
#include"types_of_numbers.h"

typedef struct Matrix {
    int size;
    ComplexNumber **mat;
} Matrix;

void printMatrix(Matrix *matrix) {
    int size = matrix->size;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            printNumber(&(*matrix).mat[i][j]);
        }
        printf("\n");
    }
}

void createMatrix(Matrix *matrix, int size, ComplexNumber vector[]) {
    matrix->size = size;
    matrix->mat = (ComplexNumber**) malloc(size * sizeof(ComplexNumber*));
    for (int i = 0; i < size; ++i) {
        matrix->mat[i] = (ComplexNumber *) malloc(size * sizeof(ComplexNumber));
        for (int j = 0; j < size; ++j) {
            matrix->mat[i][j] = vector[i * size + j];
        }
    }
};