#include "Lines.h"

Lines::Lines() {
	eps = 1E-9;
};

double Lines::findDeterminant(double a, double b, double c, double d) {
	return d * a - b * c;
}

bool Lines::between(double a, double b, double c) {
	return std::min(a, b) <= c + eps && c <= std::max(a, b) + eps;
}

bool Lines::intersect1(double a, double b, double c, double d) {
	if (a > b) std::swap(a, b);
	if (c > d) std::swap(c, d);
	return std::max(a, c) <= std::min(b, d);
}

bool Lines::intersect(Line* that, Line* other) {
    double* a = that->GetBeginPosition();
    double* b = that->GetEndPosition();

    double* c = other->GetBeginPosition();
    double* d = other->GetEndPosition();

    double A1 = a[1] - b[1];
    double B1 = b[0] - a[0];

    double C1 = -A1 * a[0] - B1 * a[1];
    double A2 = c[1] - d[1];
    double B2 = d[0] - c[0];

    double C2 = -A2 * c[0] - B2 * c[1];

    double zn = findDeterminant(A1, B1, A2, B2);
    if (zn != 0) {
        double x = -findDeterminant(C1, B1, C2, B2) * 1. / zn;
        double y = -findDeterminant(A1, C1, A2, C2) * 1. / zn;
        
        return between(a[0], b[0], x) && between(a[1], b[1], y) && between(c[0], d[0], x) && between(c[1], d[1], y);
    }
    else {
        return findDeterminant(A1, C1, A2, C2) == 0 && findDeterminant(B1, C1, B2, C2) == 0 &&
            intersect1(a[0], b[0], c[0], d[0]) && intersect1(a[1], b[1], c[1], d[1]);
    }
}


double* Lines::getIntersectionPoint(Line* that, Line* other) {
    double* a = that->GetBeginPosition();
    double* b = that->GetEndPosition();

    double* c = other->GetBeginPosition();
    double* d = other->GetEndPosition();

    double A1 = a[1] - b[1];
    double B1 = b[0] - a[0];

    double C1 = -A1 * a[0] - B1 * a[1];
    double A2 = c[1] - d[1];
    double B2 = d[0] - c[0];

    double C2 = -A2 * c[0] - B2 * c[1];
    double zn = findDeterminant(A1, B1, A2, B2);
    double x = -findDeterminant(C1, B1, C2, B2) * 1. / zn;
    double y = -findDeterminant(A1, C1, A2, C2) * 1. / zn;

    return new double[2]{ x, y };
}

void Lines::add(Line* line) {
    
}