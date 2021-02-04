#pragma once
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <array>

class Grid
{
public:
	Grid();
	Grid(double* _position, double* _size, double* _cell);

	void SetPosition(double* newpos);
	void SetPosition(double newX, double newY, double newZ = 0);
	void SetSize(double* newsize, bool rebuild = true);
	void SetCell(double cellX, double cellY, bool rebuild = true);

	double* GetPosition();
	double* GetSize();
	double* GetCell();
	vtkSmartPointer<vtkActor> GetActor();

	void BuildActor(); 

private:
	vtkSmartPointer<vtkActor> actor;
	std::array<double, 3> position;	// coordinates in space
	std::array<double, 2> size;		// the sizes of the rectangle grid
	std::array<double, 2> cell;		// the cell size
};

