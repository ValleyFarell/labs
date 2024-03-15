#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types_of_numbers.h"

typedef enum {
    SET_SIZE_ERROR,
    TYPE_CONFLICT_ERROR,
    DISCREPACY_SIZES_ERROR
} errorsTypes;

typedef enum {
  INT_VEC = 4,
  DOUBLE_VEC = 8,
  COML_VEC = 16
} vectorType;

typedef struct vector {
  int size;
  vectorType type;
  size_t elemSize;
  void *elems;
} vector;

size_t getElemSize(vectorType type) {
  size_t elemSize;
  switch (type)
    {
    case INT_VEC:
      elemSize = sizeof(int);
      break;
    case DOUBLE_VEC:
      elemSize = sizeof(double);
      break;
    case COML_VEC:
      elemSize = sizeof(ComplexNumber);
      break;
    }
  return elemSize;
}

void *setValue(vector *currVector, void *value, int pos, vectorType type) {
  size_t sizeElem = getElemSize(type);
  memcpy((char*)currVector->elems + pos * sizeElem, value, sizeElem);
}

void *getValue(vector *currVector, int pos, vectorType type) {
  size_t sizeElem = getElemSize(type);
  void *value = malloc(sizeElem);
  memcpy(value, (char*)currVector->elems + pos * sizeElem, sizeElem);
  return value;
}

vector *createVector(int size, vectorType type, void *arrayOfValues) {
  size_t elemSize = getElemSize(type);
  vector *new_vector = malloc(sizeof(vector));
  new_vector->elemSize = elemSize;
  new_vector->elems = malloc(size * elemSize);
  new_vector->size = size;
  new_vector->type = type;
  for (int i = 0; i < size; ++i) {
    void *value = malloc((int)type);
    memcpy(value, (char*)arrayOfValues + i * (int)type, (int)type);
    setValue(new_vector, value, i, type);
  }
  return new_vector;
}



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
            /* ... */
            break;
    }
    setValue(first, sumAddress, pos, first->type);
}

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
}

