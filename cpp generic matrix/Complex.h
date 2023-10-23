#ifndef EX3_COMPLEX_H
#define EX3_COMPLEX_H
#include <iostream>
namespace MtmMath {
    class Complex {
        double re,im;
    public:
        Complex(double r=0.0, double i=0.0):re(r),im(i){}
        Complex(const Complex&);
        Complex& operator=(const Complex&);
        Complex& operator+=(const Complex&);
        Complex& operator-=(const Complex&);
        Complex& operator*=(const Complex&);
        Complex operator-()const;
        friend bool operator==(const Complex& complex1,const Complex& complex2);
        friend std::ostream& operator<<(std::ostream& os, const Complex& c);
        friend std::istream& operator>>(std::istream& is, Complex& c);
    };
    bool operator==(const Complex& complex1,const Complex& complex2);
    bool operator!=(const Complex& complex1, const Complex& complex2);
    std::ostream& operator<<(std::ostream& os, const Complex& complex);
    std::istream& operator>>(std::istream& is, Complex& complex);
    Complex operator+(const Complex& complex1, const Complex& complex2);
    Complex operator-(const Complex& complex1, const Complex& complex2);
    Complex operator*(const Complex& complex1,const Complex& complex2);
}
#endif //EX3_COMPLEX_H
