#include "Point.h"

Point::Point(double* coordinate) {
	SetPosition(coordinate);
};

double* Point::GetPosition() {
	return this->coordinate;
}

void Point::SetPosition(double* coordinate){
	this->coordinate = coordinate;
}

void Point::build(vtkSmartPointer<vtkRenderer> renderer) {
	vtkSmartPointer<vtkSphereSource> point = vtkSmartPointer<vtkSphereSource>::New();
	point->SetThetaResolution(100);
	point->SetPhiResolution(50);
	point->SetRadius(0.001);
	vtkSmartPointer<vtkPolyDataMapper> pointMapper = vtkSmartPointer<vtkPolyDataMapper>::New();

	pointMapper->SetInputConnection(point->GetOutputPort());

	vtkSmartPointer<vtkActor> pointActor = vtkSmartPointer<vtkActor>::New();
	pointActor->SetMapper(pointMapper);

	pointActor->GetProperty()->SetColor(0.0, 0.0, 0.0);
	pointActor->SetPosition(coordinate[0], coordinate[1], 0.0);


	renderer->AddActor(pointActor);
}
