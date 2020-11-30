#pragma once
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkCamera.h>
#include <vtkPropPicker.h>
#include <vtkRenderer.h>
#include <vtkSetGet.h>
#include <vtkObjectFactory.h>



class gridStyle : public vtkInteractorStyleTrackballCamera
{
public:
	static gridStyle* New();
	vtkTypeMacro(gridStyle, vtkInteractorStyleTrackballCamera);
	gridStyle();
	gridStyle(
		vtkSmartPointer<vtkActor> axesX,
		vtkSmartPointer<vtkActor> axesY,
		vtkSmartPointer<vtkActor> axesMain,
		vtkSmartPointer<vtkRenderer> renderer);

private:
	virtual void OnLeftButtonDown();
	virtual void OnLeftButtonUp();
	virtual void OnRightButtonDown();
	virtual void OnMouseWheelBackward();
	virtual void OnMouseWheelForward();
	virtual void OnTimer();
	virtual void OnMouseMove();
	void rebuildYlines();
	void rebuildXlines();

private:

	bool flg = false;
	int countBack = 1, countForw = 1;
	double grid_CellX = 0.01;
	double grid_CellY = 0.05;
	double viewportSize[2];
	int cellScreenWidth_ = 0;
	double worldToScreenCoeff_ = 0;
	double  lastCameraScale_ = 0;
	bool isPaneOnly_ = false;
	double zPosition;

	vtkSmartPointer<vtkActor> axesY_;
	vtkSmartPointer<vtkActor> axesX_;
	vtkSmartPointer<vtkCamera> camera_;
	vtkSmartPointer<vtkActor> axesMain_;
	vtkSmartPointer<vtkRenderer> renderer_;
};



