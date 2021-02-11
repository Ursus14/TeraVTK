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

void Lines::add(Line* line, vtkSmartPointer<vtkRenderer> renderer) {
    if (!lines.empty()) {
        std::vector<Line*> intersectedSegments;
        std::vector<std::array<double, 2>> points;
        for (auto &elem: lines) {
            if (intersect(elem, line)) {
                std::array<double, 2> pointCoordinate = { getIntersectionPoint(elem, line)[0], getIntersectionPoint(elem, line)[1]};
                if (pointCoordinate[0] != elem->GetBeginPosition()[0] && 
                    pointCoordinate[0] != elem->GetEndPosition()[0] &&
                    pointCoordinate[1] != elem->GetBeginPosition()[1] &&
                    pointCoordinate[1] != elem->GetEndPosition()[1]) {
                    points.push_back(pointCoordinate);
                    intersectedSegments.push_back(elem);
                }
            }
        }
        
        if (!points.empty()) {
            if (points.size() < 2) {
                Line* lineA = new Line(vtkSmartPointer<vtkActor>::New());
                lineA->SetBeginPosition(line->GetBeginPosition());
                lineA->build(new double[2]{ points[0][0], points[0][1] }, renderer, 0.02);
                Line* lineB = new Line(vtkSmartPointer<vtkActor>::New());
                lineB->SetBeginPosition(new double[2]{ points[0][0], points[0][1] });
                lineB->build(line->GetEndPosition(), renderer, 0.05);

                lines.push_back(lineA);
                lines.push_back(lineB);
             
                Line* intersectedLineA = new Line(vtkSmartPointer<vtkActor>::New());
                intersectedLineA->SetBeginPosition(intersectedSegments[0]->GetBeginPosition());
                intersectedLineA->build(new double[2]{ points[0][0], points[0][1] }, renderer, 0.05);
                Line* intersectedLineB = new Line(vtkSmartPointer<vtkActor>::New());
                intersectedLineB->SetBeginPosition(new double[2]{ points[0][0], points[0][1] });
                intersectedLineB->build(intersectedSegments[0]->GetEndPosition(), renderer, 0.02);

                lines.push_back(intersectedLineA);
                lines.push_back(intersectedLineB);

                for (auto& elem : intersectedSegments) {
                    lines.erase(std::remove(lines.begin(), lines.end(), elem), lines.end());
                    elem->Remove(renderer);
                }
            }
            else {
                std::vector<Line*> segments;
                for (int i = 0; i < points.size() - 1; ++i) {
                    Line* lineA = new Line(vtkSmartPointer<vtkActor>::New());
                    lineA->SetBeginPosition(new double[2]{ points[i][0], points[i][1] });
                    lineA->build(new double[2]{ points[i + 1][0], points[i+1][1] }, renderer, 0.05);
                    lines.push_back(lineA);
                }

                if (line->GetBeginPosition()[0] != points[0][0] && line->GetBeginPosition()[1] != points[0][1]) {
                    Line* lineB = new Line(vtkSmartPointer<vtkActor>::New());
                    lineB->SetBeginPosition(line->GetBeginPosition());
                    lineB->build(new double[2]{ points[0][0], points[0][1] }, renderer, 0.05);
                    lines.push_back(lineB);
                }
                if (line->GetEndPosition()[0] != points[points.size() - 1][0] && line->GetEndPosition()[1] != points[points.size() - 1][1]) {
                    Line* lineC = new Line(vtkSmartPointer<vtkActor>::New());
                    lineC->SetBeginPosition(new double[2]{ points[points.size() - 1][0], points[points.size() - 1][1]});
                    lineC->build(line->GetEndPosition(), renderer, 0.05);
                    lines.push_back(lineC);

                }
                for (int i = 0; i < points.size(); ++i) {
                    Line* lineD = new Line(vtkSmartPointer<vtkActor>::New());
                    lineD->SetBeginPosition(intersectedSegments[i]->GetBeginPosition());
                    lineD->build(new double[2] { points[i][0], points[i][1] }, renderer, 0.05);
                    lines.push_back(lineD);

                    Line* lineF = new Line(vtkSmartPointer<vtkActor>::New());
                    lineF->SetBeginPosition(new double[2]{ points[i][0], points[i][1] });
                    lineF->build(intersectedSegments[i]->GetEndPosition(), renderer, 0.05);
                    lines.push_back(lineF);
                }
                for (auto& elem : intersectedSegments) {
                    lines.erase(std::remove(lines.begin(), lines.end(), elem), lines.end());
                    elem->Remove(renderer);
                }
            }
        } else {
            lines.push_back(line);
        }
    }
    else {
        lines.push_back(line);
    }
    
   

}