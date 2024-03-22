#ifndef vector_h
#define vector_h

#include "../types/types_of_numbers.h"
#include <stdio.h>

typedef enum {
    SET_SIZE_ERROR,
    TYPE_CONFLICT_ERROR,
    DISCREPACY_SIZES_ERROR
} errorsTypes;

typedef enum {
  INT_VEC = 4,
  DOUBLE_VEC = 8,
  COML_VEC = 16
} Type;

typedef struct vector {
  int size;
  Type type;
  size_t elemSize;
  void *elems;
  char name[15];
  size_t (*getElemSize)(Type type);
  void* (*setValue)();
  void* (*getValue)();
  void* (*additionElem)();
  void* (*multiplyElem)();
  void* (*additionVec)();
  void* (*scalarMultiply)();
  char* (*toString)();
} vector;

size_t getElemSize(Type type);
void *setValue(vector *currVector, void *value, int pos, Type type);
void *getValue(vector *currVector, int pos, Type type);
vector *createVector(int size, Type type, void *arrayOfValues, int fieldByHand, char* name);
void *additionElem(vector *first, vector *second, int pos);
void *multiplyElem(vector *first, vector *second, int pos);
void *additionVec(vector *first, vector *second, errorsTypes *error);
void *scalarMultiply(vector *first, vector *second, errorsTypes *error);
char *toString(vector *vector);
//---------------------------------Получение размера типа в байтах-------------------------------
// type - тип
size_t getElemSize(Type type) {
  return (int) type;
};

// --------------------------------Установление значения в векторе--------------------------------
// currVector - адрес вектора в памяти
// value - адрес значения, которое нужно установить в ячейке вектора
// pos - адрес ячейки, в которую помещается значение
// type - тип помещаемого значения
void *setValue(vector *currVector, void *value, int pos, Type type) {
  size_t sizeElem = getElemSize(type);
  memcpy((char*)currVector->elems + pos * sizeElem, value, sizeElem);
};

// --------------------------------Излечение значения в векторе--------------------------------
// currVector - адрес вектора в памяти
// value - адрес значения, которое нужно извлечь в ячейке вектора
// pos - адрес извлекаемой ячейки
// type - тип извлекаемого значения
void *getValue(vector *currVector, int pos, Type type) {
  size_t sizeElem = getElemSize(type);
  void *value = malloc(sizeElem);
  memcpy(value, (char*)currVector->elems + pos * sizeElem, sizeElem);
  return value;
};

// -----------------------------------Создание вектора--------------------------------------------
// size - размерность
// type - тип, поддерживаемый вектором (INT_VEC, DOUBLE_VEC, COML_VEC)
// arrayOfValues - адрес массива, на основе которого мы создаётся вектор 
vector *createVector(int size, Type type, void *arrayOfValues, int fieldByHand, char* name) {
  size_t elemSize = getElemSize(type);
  vector *new_vector = malloc(sizeof(vector));
  strcpy(new_vector->name, name);
  new_vector->elemSize = elemSize;
  new_vector->elems = malloc(size * elemSize);
  new_vector->size = size;
  new_vector->type = type;
  new_vector->setValue = setValue;
  new_vector->getValue = getValue;
  new_vector->getElemSize = getElemSize;
  new_vector->additionElem = additionElem;
  new_vector->additionVec = additionVec;
  new_vector->multiplyElem = multiplyElem;
  new_vector->scalarMultiply = scalarMultiply;
  new_vector->toString = toString;
  for (int i = 0; i < size; ++i) {
    void *value = malloc((int)type);
    int tmp = 0;
    memcpy(value, (fieldByHand == 0) ? &tmp : ((char*)arrayOfValues + i * (int)type), (int)type);
    setValue(new_vector, value, i, type);
  }
  return new_vector;
};


void *additionElem(vector *first, vector *second, int pos) {
    void *sumAddress;
    switch (first->type)
    {
        case INT_VEC:
            int sumI;
            sumI = *((int*) getValue(first, pos, INT_VEC)) + \
            *((int*) getValue(second, pos, INT_VEC));
            sumAddress = &sumI;
            break;
        case DOUBLE_VEC:
            double sumD;
            sumD = *((double*) getValue(first, pos, DOUBLE_VEC)) + \
            *((double*) getValue(second, pos, DOUBLE_VEC));
            sumAddress = &sumD;
            break;
        case COML_VEC:
            ComplexNumber sumC;
            sumC.re = *((double*) getValue(first, pos * 2, DOUBLE_VEC)) + \
            *((double*) getValue(second, pos * 2, DOUBLE_VEC));
            sumC.im = *((double*) getValue(first, pos * 2 + 1, DOUBLE_VEC)) + \
            *((double*) getValue(second, pos * 2 + 1, DOUBLE_VEC));
            sumAddress = &sumC;
            break;
    }
    setValue(first, sumAddress, pos, first->type);
};

void *multiplyElem(vector *first, vector *second, int pos) {
    void *sumAddress;
    switch (first->type)
    {
        case INT_VEC:
            int I = *((int*) getValue(first, pos, INT_VEC)) * \
            *((int*) getValue(second, pos, INT_VEC));
            sumAddress = &I;
            break;
        case DOUBLE_VEC:
            double D = *((double*) getValue(first, pos, DOUBLE_VEC)) * \
            *((double*) getValue(second, pos, DOUBLE_VEC));
            sumAddress = &D;
            break;
        case COML_VEC:
            ComplexNumber C1, C2;
            C1.re = *((double*) getValue(first, pos * 2, DOUBLE_VEC));
            C2.re = *((double*) getValue(second, pos * 2, DOUBLE_VEC));
            C1.im = *((double*) getValue(first, pos * 2 + 1, DOUBLE_VEC));
            C2.im = *((double*) getValue(second, pos * 2 + 1, DOUBLE_VEC));
            ComplexNumber C = multiplication(C1, C2);
            sumAddress = &C;
            break;
    }
    return sumAddress;
};

void *additionVec(vector *first, vector *second, errorsTypes *error) {
    if(first->type != second->type) {
        *error = TYPE_CONFLICT_ERROR;
        return NULL;
    }
    if (first->size != second->size) {
        *error = DISCREPACY_SIZES_ERROR;
        return NULL;
    }
    for (int i = 0; i < first->size; ++i) {
        additionElem(first, second, i);
    }
};

void *scalarMultiply(vector *first, vector *second, errorsTypes *error) {
    if(first->type != second->type) {
        *error = TYPE_CONFLICT_ERROR;
        return NULL;
    }
    if (first->size != second->size) {
        *error = DISCREPACY_SIZES_ERROR;
        return NULL;
    }
    void* multAdress;
    switch (first->type)
    {
        case INT_VEC:
            int I = 0;
            for (int i = 0; i < first->size; ++i) {
                multiplyElem(first, second, i);
                I += *((int*) multiplyElem(first, second, i));
            }
            multAdress = (int*) malloc(sizeof(int));
            memcpy(multAdress, &I, sizeof(int));
            break;
        case DOUBLE_VEC:
            double D = 0;
            for (int i = 0; i < first->size; ++i) {
                multiplyElem(first, second, i);
                D += *((int*) multiplyElem(first, second, i));
            }
            multAdress = malloc(sizeof(double));
            memcpy(multAdress, &D, sizeof(double));
            break;
        case COML_VEC:
            ComplexNumber C;
            C.re = 0;
            C.im = 0;
            for (int i = 0; i < first->size; ++i) {
                ComplexNumber * tmp = (ComplexNumber*) multiplyElem(first, second, i);
                C.re += tmp->re;
                C.im += tmp->im;
            }
            multAdress = malloc(sizeof(ComplexNumber));
            memcpy(multAdress, &C, sizeof(ComplexNumber));
            break;
    }
    return multAdress;
};

char *toString(vector *vector) {
    int n = 1;
    char *str = malloc(n);
    switch (vector->type)
    {
    case INT_VEC:
        for (int i = 0; i < vector->size; ++i) {
            char elemCh[100];
            int elem = *((int*)vector->getValue(vector, i, INT_VEC));
            if (i != vector->size - 1) sprintf(elemCh, "%d, ", elem);
            else sprintf(elemCh, "%d", elem);
            str = (char*)realloc(str, n + strlen(elemCh));
            strcpy(str + n - 1, elemCh);
            n += strlen(elemCh);
        }
        break;
    case DOUBLE_VEC:
        
        break;
    case COML_VEC:
        
        break;
    default:
        break;
    }
    char *result = (char*) malloc(n * sizeof(char));
    sprintf(result, "(%s)", str);
    return result;
}

#endif