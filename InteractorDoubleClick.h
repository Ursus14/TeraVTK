#pragma once
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkCamera.h>
#include <vtkPropPicker.h>
#include <vtkSetGet.h>
#include <vtkCoordinate.h>
#include <vtkPointPicker.h>
#include <vtkRendererCollection.h>
#include <vtkRegularPolygonSource.h>

#include "vtkActor.h"
#include "vtkActor2D.h"
#include "vtkAssemblyNode.h"
#include "vtkAssemblyPath.h"
#include "vtkCallbackCommand.h"
#include "vtkCellPicker.h"
#include "vtkCommand.h"
#include "vtkEventForwarderCommand.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkOutlineSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkProperty2D.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkTDxInteractorStyleCamera.h"


#include <chrono>
#include <ctime>
#include <array>
#include "vtkCallbackCommand.h"
#include "vtkCamera.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkRenderWindowInteractor.h"

class VTKINTERACTIONSTYLE_EXPORT InteractorDoubleClick : public vtkInteractorStyleTrackballCamera
{
public:
	static InteractorDoubleClick* New();
	vtkTypeMacro(InteractorDoubleClick, vtkInteractorStyleTrackballCamera);
	InteractorDoubleClick();

	virtual void OnLeftDoubleClick();
	virtual void OnLeftButtonDown();
	

private:
	double doubleClickTimeLimit = 0.5;
	std::array<int, 2> PreviousPosition = { 0,0 };
	int ResetPixelDistance = 5;
	unsigned int NumberOfClicks = 0;
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
};


