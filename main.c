#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"


int main() {
  vector newVector;
  printf("%ld", sizeof(ComplexNumber));
  errorsTypes ER;
  int array[5] = {1, 5, 9, -7, 0};
  newVector = *createVector(5, INT_VEC, &array[0]);
  void *v = getValue(&newVector, 4, INT_VEC);
  printf("%d", *((int *)v));
  return 0;
}