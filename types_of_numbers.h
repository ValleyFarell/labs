typedef struct  {
    double re;
    double im;
} ComplexNumber;

ComplexNumber addition(ComplexNumber x, ComplexNumber y) {
    ComplexNumber result;
    result.im = x.im + y.im;
    result.re = x.re + y.re;
    return result;
}

ComplexNumber multiplication(ComplexNumber x, ComplexNumber y) {
    ComplexNumber result;
    result.im = x.re * y.re - x.im * y.im;
    result.re = x.re * y.im + x.im * y.re;
    return result;
}
