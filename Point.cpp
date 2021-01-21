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

void Point::SetRadius(double radius) {
	this->radius = radius;
}

void Point::build(vtkSmartPointer<vtkRenderer> renderer) {
	vtkSmartPointer<vtkRegularPolygonSource> point = vtkSmartPointer<vtkRegularPolygonSource>::New();
	point->SetNumberOfSides(50);
	point->SetRadius(radius);
	point->SetCenter(0.0, 0.0, 0.0);
	vtkSmartPointer<vtkPolyDataMapper> pointMapper = vtkSmartPointer<vtkPolyDataMapper>::New();

	pointMapper->SetInputConnection(point->GetOutputPort());

	vtkSmartPointer<vtkActor> pointActor = vtkSmartPointer<vtkActor>::New();
	pointActor->SetMapper(pointMapper);

	pointActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
	pointActor->SetPosition(coordinate[0], coordinate[1], 0.0);


	renderer->AddActor(pointActor);
}
