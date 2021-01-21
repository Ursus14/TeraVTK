#include "Line.h"

Line::Line() {
	beginPosition = new double[2]{ 0.0, 0.0 };
	endPosition = new double[2]{ 0.0, 0.0 };
};

void Line::SetEndPosition(double* coordinate) {
	this->endPosition = coordinate;
}

void Line::SetBeginPosition(double* coordinate) {
	this->beginPosition = coordinate;
}

double* Line::GetBeginPosition() {
	return this->beginPosition;
}

double* Line::GetEndPosition() {
	return this->endPosition;
} 

void Line::build(double* coordinateE, vtkSmartPointer<vtkActor> lineActor, vtkSmartPointer<vtkRenderer> renderer) {
	vtkSmartPointer<vtkLineSource> lineSource =
		vtkSmartPointer<vtkLineSource>::New();

	lineSource->SetPoint1(beginPosition[0], beginPosition[1], 0.0);
	lineSource->SetPoint2(coordinateE[0], coordinateE[1], 0.0);

	cout << " First point:     " << beginPosition[0] << "    " << beginPosition[1] << endl;
	cout << " Second point:     " << coordinateE[0] << "    " << coordinateE[1] << endl;
	endPosition = coordinateE;


	vtkSmartPointer<vtkPolyDataMapper> lineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();

	lineMapper->SetInputConnection(lineSource->GetOutputPort());

	lineActor->SetMapper(lineMapper);
	lineActor->GetProperty()->SetColor(0.0, 1.0, 0.0);


	renderer->AddActor(lineActor);
}