#pragma once
#include "InteractorDoubleClick.h"
#include "Grid.h"
#include "PlaneGrid.h"
#include "MainAxes.h"
#include "Point.h"
#include "Lines.h"
#include "BrokenLine.h"


class VTKINTERACTIONSTYLE_EXPORT GridInteractorStyle: public InteractorDoubleClick
{
public:
	static GridInteractorStyle* New();
	vtkTypeMacro(GridInteractorStyle, InteractorDoubleClick);
	GridInteractorStyle();
	GridInteractorStyle(PlaneGrid* _plane, 
						MainAxes* _axes,
						Point* _marker, 
						std::vector<Point> _drawPoints, 
						vtkSmartPointer<vtkCamera> _camera, 
						vtkSmartPointer<vtkRenderer> _renderer);

	double* GetCurrentMousePosition();

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

	//---------------------------------------
	bool isAddLine = false;
	double prevPosition[2]{ 0.0, 0.0 };
	std::vector<Line*> lines_;
	Line* line_ = new Line(vtkSmartPointer<vtkActor>::New());
	Lines* lines = new Lines();
	BrokenLine* brokenLine_ = new BrokenLine();
	//--------------------------------------
	PlaneGrid* plane;
	MainAxes* axes;
	Point* marker;
	std::vector<Point> drawPoints;
	vtkSmartPointer<vtkCamera> camera;
	vtkSmartPointer<vtkRenderer> renderer;
};
