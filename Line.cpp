#include "Line.h"

Line::Line(vtkSmartPointer<vtkActor> lineActor) {
	this->SetBeginPosition(new double[2]{ 0.0, 0.0 });
	this->SetEndPosition(new double[2]{ 0.0, 0.0 });
	this->lineActor_ = lineActor;
};

void Line::SetEndPosition(double* coordinate) {
	this->endPosition[0] = coordinate[0];
	this->endPosition[1] = coordinate[1];
}

void Line::SetBeginPosition(double* coordinate) {
	this->beginPosition[0] = coordinate[0];
	this->beginPosition[1] = coordinate[1];
}

double* Line::GetBeginPosition() {
	return this->beginPosition;
}

double* Line::GetEndPosition() {
	return this->endPosition;
} 

void Line::build(double* coordinateE, vtkSmartPointer<vtkRenderer> renderer) {
	vtkSmartPointer<vtkLineSource> lineSource =
		vtkSmartPointer<vtkLineSource>::New();

	lineSource->SetPoint1(beginPosition[0], beginPosition[1], 0.0);
	lineSource->SetPoint2(coordinateE[0], coordinateE[1], 0.0);

	endPosition[0] = coordinateE[0];
	endPosition[1] = coordinateE[1];

	vtkSmartPointer<vtkPolyDataMapper> lineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();

	lineMapper->SetInputConnection(lineSource->GetOutputPort());

	this->lineActor_->SetMapper(lineMapper);
	this->lineActor_->GetProperty()->SetColor(0.0, 1.0, 0.0);


	renderer->AddActor(this->lineActor_);

}