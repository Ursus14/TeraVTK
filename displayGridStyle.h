#pragma once
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkSetGet.h>
#include <vtkObjectFactory.h>



class displayGridStyle : public vtkInteractorStyleTrackballCamera
{
public:
	static displayGridStyle* New();
	vtkTypeMacro(displayGridStyle, vtkInteractorStyleTrackballCamera);
	displayGridStyle();
	displayGridStyle(
		vtkSmartPointer<vtkActor> axesX, 
		vtkSmartPointer<vtkActor> axesY, 
		vtkSmartPointer<vtkActor> axesMain, 
		vtkSmartPointer<vtkCamera> camera, 
		vtkSmartPointer<vtkRenderer> renderer);

private:
	virtual void OnLeftButtonDown();
	virtual void OnLeftButtonUp();
	virtual void OnMouseWheelBackward();
	virtual void OnMouseWheelForward();
	virtual void OnTimer();
	void rebuildYlines();
	void rebuildXlines();

private:
	
	bool flg = false;
	int countBack = 1, countForw = 1;
	double grid_CellX = 0.05;
	double grid_CellY = 0.05;
	double viewportSize[2];
	int cellScreenWidth_ = 0;
	double worldToScreenCoeff_ = 0;
	double  lastCameraScale_ = 0;
	bool isPaneOnly_ = false;
	double zPosition;

	vtkSmartPointer<vtkActor> axesY_ = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> axesX_ = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkCamera> camera_ = vtkSmartPointer<vtkCamera>::New();
	vtkSmartPointer<vtkActor> axesMain_ = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkRenderer> renderer_ = vtkSmartPointer<vtkRenderer>::New();
};



