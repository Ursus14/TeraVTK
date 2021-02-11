
#pragma once
#include "UserEvents.h"
#include "Grid.h"
#include "PlaneGrid.h"
#include "MainAxes.h"
#include "Point.h"
#include "Lines.h"
#include "BrokenLine.h"
#include <array>


class VTKINTERACTIONSTYLE_EXPORT InteractorStyle: public UserEvents
{
public:
	static InteractorStyle* New();
	vtkTypeMacro(InteractorStyle, UserEvents);
	InteractorStyle();
	InteractorStyle(PlaneGrid* _plane, 
						MainAxes* _axes,
						Point* _marker, 
						std::vector<Point> _drawPoints, 
						vtkSmartPointer<vtkRenderer> _renderer);

	double* GetCurrentMousePosition();
	double* GetViewportBorder();

private:
	virtual void OnMouseWheelBackward();
	virtual void OnMouseWheelForward();
	virtual void OnRightButtonDown();
	virtual void OnRightButtonUp();
	virtual void OnLeftButtonDown();
	virtual void OnLeftButtonUp();
	virtual void OnMouseMove();
	virtual void OnLeave();
	virtual void OnLeftDoubleClick();
	virtual void Scrolling();

	//---------------------------------------
	bool isAddLine = false;
	double dx = 0;
	double dy = 0;
	double prevPosition[3] = { 0.0, 0.0, 0.0 };

	std::vector<Line*> lines_;
	Line* line_ = new Line(vtkSmartPointer<vtkActor>::New());
	Lines* lines = new Lines();
	BrokenLine* brokenLine_ = new BrokenLine();
	//--------------------------------------
	PlaneGrid* plane;
	MainAxes* axes;
	Point* marker;
	std::vector<Point> drawPoints;
	vtkSmartPointer<vtkRenderer> renderer;
};