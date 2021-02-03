#pragma once
#include "Grid.h"
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>
#include <cmath>

class PlaneGrid
{
public:
	PlaneGrid();
	PlaneGrid(double* cell, int* sizewin, double parallelScale);
	void RecountBorderline(); 
	void RebuildPlane(vtkCamera* camera, int* sizewin);
	void HitTestingAtBorder(vtkCamera* camera);
	void SetCell(double cellX, double cellY);
	double* GetCell();
	
	vtkSmartPointer<vtkActor> GetActor(int idx);

private:
	void FlippingGrids(int borderNumber);

	Grid pGrids[4];					// four elements of the coordinate plane
	double* borderline;				// boundaries where the camera can be located
	int sequence[4] = { 0, 1, 2, 3 };	// to indicate the position of each grid
};

