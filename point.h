#pragma once
#include "GridStyle.h"
#include <vtkNew.h>
#include <vtkProperty.h>

class point
{
public:
	
	point();
	point(double *pos, vtkSmartPointer<vtkRenderer> renderer);
	void draw();

private:
	double pos_[2];
	vtkSmartPointer<vtkPolyDataMapper> mapperPoint_;
	vtkSmartPointer<vtkActor> actorPoint_;
	vtkSmartPointer<vtkRenderer> renderer_;
};

