#pragma once
#include <vtkRegularPolygonSource.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>


class Point
{
public:
	Point();
	Point(double _radius);
	Point(double* _position, double _radius = 0.025);
	void SetRadius(double _radius);
	void SetPosition(double* position);
	void SetColor(int r, int g, int b); // parameter values from 0 to 255
	void VisibilityOn();
	void VisibilityOff();
	
	bool Equals(Point* a);
	double GetRadius();
	double* GetPosition();
	vtkSmartPointer<vtkActor> GetActor();

private:
	void build(double* position, double radius);
	
	vtkSmartPointer<vtkActor> actor;
	double radius;
};