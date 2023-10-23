#include "Complex.h"
#include <iostream>
using MtmMath::Complex;


Complex::Complex(const Complex& complex){
    re=complex.re;
    im=complex.im;
}

Complex& Complex::operator=(const Complex& complex){
    re=complex.re;
    im=complex.im;
    return *this;
}

Complex& Complex::operator+=(const Complex& complex) {
    re += complex.re;
    im += complex.im;
    return *this;
}

Complex& Complex::operator-=(const Complex& complex) {
    return *this += -complex;
}

Complex& Complex::operator*=(const Complex& complex){
    double new_re=(re*complex.re)-(im*complex.im),
    new_im=(im*complex.re)+(re*complex.im);
    re=new_re;
    im=new_im;
    return *this;
}

Complex Complex::operator-() const {
    return Complex(-re, -im);
}

bool MtmMath::operator==(const Complex& complex1, const Complex& complex2) {
    return complex1.re==complex2.re && complex1.im==complex2.im;
}

std::ostream& MtmMath::operator<<(std::ostream& os, const Complex& complex){
    const char* sign = complex.im < 0 ? "" : "+";
    return os << complex.re << sign << complex.im << "i";
}

std::istream& MtmMath::operator>>(std::istream& is, Complex& complex){
    return is >> complex.re >> complex.im;
}

Complex MtmMath::operator+(const Complex& complex1, const Complex& complex2){
    return Complex(complex1)+=complex2;
}

Complex MtmMath::operator-(const Complex& complex1, const Complex& complex2){
    return Complex(complex1)-=complex2;
}

Complex MtmMath::operator*(const Complex& complex1, const Complex& complex2){
    return Complex(complex1)*=complex2;
}

bool MtmMath::operator!=(const Complex& complex1, const Complex& complex2){
    return !(complex1==complex2);
}


