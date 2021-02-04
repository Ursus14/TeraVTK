#include "Line.h"

Line::Line(vtkSmartPointer<vtkActor> lineActor) {
	
	points[0] = Point();
	points[1] = Point();
	points[0].SetColor(0, 0, 0);
	points[1].SetColor(0, 0, 0);
	SetBeginPosition(new double[2]{ 0.0, 0.0 });
	SetEndPosition(new double[2]{ 0.0, 0.0 });
	
	lineActor_ = lineActor;
};

void Line::SetEndPosition(double* coordinate) {
	endPosition[0] = coordinate[0];
	endPosition[1] = coordinate[1];
	endPosition[2] = 0;
}

void Line::SetBeginPosition(double* coordinate) {
	beginPosition[0] = coordinate[0];
	beginPosition[1] = coordinate[1];
	beginPosition[2] = 0;
}

double* Line::GetBeginPosition() {
	return beginPosition;
}

double Line::GetLength()
{
	double x = beginPosition[0] - endPosition[0];
	double y = beginPosition[1] - endPosition[1];
	return sqrt(x*x+y*y);
}

double* Line::GetEndPosition() {
	return endPosition;
} 

void Line::build(double* coordinateE, vtkSmartPointer<vtkRenderer> renderer, double radius) {
	
	renderer->AddActor(points[0].GetActor());
	renderer->AddActor(points[1].GetActor());
	vtkSmartPointer<vtkLineSource> lineSource =
		vtkSmartPointer<vtkLineSource>::New();

	lineSource->SetPoint1(beginPosition[0], beginPosition[1], 0.0);
	lineSource->SetPoint2(coordinateE[0], coordinateE[1], 0.0);

	endPosition[0] = coordinateE[0];
	endPosition[1] = coordinateE[1];

	vtkSmartPointer<vtkPolyDataMapper> lineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();

	lineMapper->SetInputConnection(lineSource->GetOutputPort());

	lineActor_->SetMapper(lineMapper);
	lineActor_->GetProperty()->SetColor(0.0, 1.0, 0.0);
	points[0].SetRadius(radius);
	points[1].SetRadius(radius);


	points[0].SetPosition(beginPosition);
	points[1].SetPosition(endPosition);

	renderer->AddActor(lineActor_);
	
}

void Line::rebuild(double* coordinateE, vtkSmartPointer<vtkRenderer> renderer)
{
	vtkSmartPointer<vtkLineSource> lineSource =
		vtkSmartPointer<vtkLineSource>::New();

	lineSource->SetPoint1(beginPosition[0], beginPosition[1], 0.0);
	lineSource->SetPoint2(coordinateE[0], coordinateE[1], 0.0);

	endPosition[0] = coordinateE[0];
	endPosition[1] = coordinateE[1];

	points[1].SetPosition(endPosition);

	vtkPolyDataMapper::SafeDownCast(lineActor_->GetMapper())->SetInputConnection(lineSource->GetOutputPort());
}

void Line::Remove(vtkSmartPointer<vtkRenderer> renderer)
{
	renderer->RemoveActor(points[0].GetActor());
	renderer->RemoveActor(points[1].GetActor());
	renderer->RemoveActor(lineActor_);
	double null[3] = { 0,0,0 };
	points[0].SetPosition(null);
	points[1].SetPosition(null);
}
