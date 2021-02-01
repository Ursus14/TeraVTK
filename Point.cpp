#include "Point.h"

Point::Point()
{
	actor = vtkSmartPointer<vtkActor>::New();

	vtkSmartPointer<vtkRegularPolygonSource> point = vtkSmartPointer<vtkRegularPolygonSource>::New();
	point->SetNumberOfSides(50);
	point->SetRadius(0.05);
	point->SetCenter(0, 0, 0);


	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkPolyDataMapper::New();
	mapper->SetInputConnection(point->GetOutputPort());
	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(0.47, 0.07, 0.07);
	actor->SetVisibility(1);
}

Point::Point(double* _position, double _radius)
{
	build(_position, _radius);
}

void Point::build(double* _position, double _radius) {
	actor = vtkSmartPointer<vtkActor>::New();

	vtkSmartPointer<vtkRegularPolygonSource> point = vtkSmartPointer<vtkRegularPolygonSource>::New();
	point->SetNumberOfSides(50);
	point->SetRadius(_radius);
	point->SetCenter(_position);


	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkPolyDataMapper::New();
	mapper->SetInputConnection(point->GetOutputPort());
	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(158 / 255.0, 77 / 255.0, 30 / 255.0);
	actor->SetVisibility(1);

	radius = _radius;
	
}

void Point::SetRadius(double _radius)
{
	actor->SetScale(_radius, _radius, 0);
}

void Point::SetPosition(double* position)
{
	actor->SetPosition(position);
}

void Point::SetColor(int r, int g, int b)
{
	actor->GetProperty()->SetColor(r / 255.0, g / 255.0, b / 255.0);
}

void Point::VisibilityOn()
{
	actor->SetVisibility(1);
}

void Point::VisibilityOff()
{
	actor->SetVisibility(0);
}

double Point::GetRadius()
{
	return radius;
}

double* Point::GetPosition()
{
	return actor->GetPosition();
}

vtkSmartPointer<vtkActor> Point::GetActor()
{
	return actor;
}
