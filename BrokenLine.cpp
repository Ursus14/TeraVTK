#include "BrokenLine.h"

BrokenLine::BrokenLine() {};

vtkPoints* BrokenLine::GetPoints() {
	return this->points;
}

void BrokenLine::SetPoints(vtkPoints* points) {
	this->points = points;
}

void BrokenLine::build(vtkPoints* points, vtkSmartPointer<vtkRenderer> renderer) {
	vtkSmartPointer<vtkLineSource> lineSource =
		vtkSmartPointer<vtkLineSource>::New();

	lineSource->SetPoints(points);

	vtkSmartPointer<vtkPolyDataMapper> brokenLineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();

	brokenLineMapper->SetInputConnection(lineSource->GetOutputPort());

	vtkSmartPointer<vtkActor> brokenLineActor = vtkSmartPointer<vtkActor>::New();
	brokenLineActor->SetMapper(brokenLineMapper);

	brokenLineActor->GetProperty()->SetColor(0.32, 0.21, 0.123);

	renderer->AddActor(brokenLineActor);
}