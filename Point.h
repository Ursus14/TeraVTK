#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>

class Point
{
public:
	Point(double* coordinate);


public:
	void SetPosition(double* coordinate);
	double* GetPosition();

	void build(vtkSmartPointer<vtkRenderer> renderer);

private:
	double* coordinate;

};

