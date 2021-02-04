#pragma once
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkCamera.h>


class MainAxes
{
public:
	MainAxes();
	MainAxes(int* sizewin, double parallelScale);
	void RebuildAxes(vtkSmartPointer<vtkCamera> camera, int* sizewin);
	vtkSmartPointer<vtkActor> GetActor();

private:
	vtkSmartPointer<vtkActor> actor;
	void build(int* sizewin, double parallelScale);
};

