#pragma once
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkSmartPointer.h>
//#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkCamera.h>
//#include <vtkPropPicker.h>
#include <vtkRenderer.h>
//#include <vtkSetGet.h>
#include <vtkObjectFactory.h>
//#include <vtkCoordinate.h>
#include <vtkRenderWindow.h>
#include <vtkActor.h>
//#include <vtkPointPicker.h>
//#include <vtkRendererCollection.h>
//#include <vtkRegularPolygonSource.h>
#include "Grid.h"
#include "PlaneGrid.h"
#include "MainAxes.h"


class GridInteractorStyle: public vtkInteractorStyleTrackballCamera
{
public:
	static GridInteractorStyle* New();
	vtkTypeMacro(GridInteractorStyle, vtkInteractorStyleTrackballCamera);
	GridInteractorStyle();
	GridInteractorStyle(PlaneGrid* _plane, MainAxes* _axes , vtkSmartPointer<vtkCamera> _camera, vtkSmartPointer<vtkRenderer> _renderer);

	virtual void OnMouseWheelBackward();
	virtual void OnMouseWheelForward();
	virtual void OnRightButtonDown();
	virtual void OnRightButtonUp();
	virtual void OnLeftButtonDown();
	virtual void OnMouseMove();

private:
/*		
	virtual void OnLeave();
	virtual void OnTimer();
*/

	PlaneGrid* plane;
	MainAxes* axes;
	vtkSmartPointer<vtkCamera> camera;
	vtkSmartPointer<vtkRenderer> renderer;
};

