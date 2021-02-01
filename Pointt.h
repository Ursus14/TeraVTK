#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRegularPolygonSource.h>

class Pointt
{
public:
	Pointt(double* coordinate);


public:
	void SetPosition(double* coordinate);
	double* GetPosition();
	void SetRadius(double radius);

	void build(vtkSmartPointer<vtkRenderer> renderer);

private:
	double* coordinate;
	double radius = 0.025;
};

