#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPropPicker.h>
#include <vtkRenderer.h>

#include <vtkObjectFactory.h>
#include <vtkCoordinate.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkActor.h>
#include <vtkPointPicker.h>
#include <vtkRendererCollection.h>
#include <vtkRegularPolygonSource.h>
#include <vtkPointPicker.h>
#include <vtkDataSetMapper.h>
#include <vtkSphereSource.h>
#include <vtkLineSource.h>
#include <vtkProperty.h>
#include <chrono>
#include <ctime>

class Figures {
public:
	Figures();

public:
	void buildLine(double* coordinateB, double* coordinateE,  vtkSmartPointer<vtkActor> lineActor, vtkSmartPointer<vtkRenderer> renderer);
	void buildBrokenLine(vtkPoints* points, vtkSmartPointer<vtkRenderer> renderer);
	void buildPoint(double* coordinate, vtkSmartPointer<vtkRenderer> renderer);
};

