#ifdef _WIN32
    #include <cmath>
#else
    #include <math.h>
#endif
#include "solver.h"
#include <iostream>

void solve(double a, double b, double c) {
    double d = b*b - 4*a*c;
    if (d < 0) {
        std::cout << "No real roots" << std::endl;
        return;
    }
    double x1 = (-b + sqrt(d)) / (2*a);
    double x2 = (-b - sqrt(d)) / (2*a);
    std::cout << "x1 = " << x1 << std::endl;
    std::cout << "x2 = " << x2 << std::endl;
}
