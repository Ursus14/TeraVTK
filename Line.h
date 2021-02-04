#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkPolyDataMapper.h>
#include <vtkLineSource.h>
#include <vtkProperty.h>
#include "Point.h"

class Line
{
public:
	Line(vtkSmartPointer<vtkActor> lineActor);

public:
	void SetEndPosition(double* coordinate);
	double* GetEndPosition();

	void SetBeginPosition(double* coordinate);
	double* GetBeginPosition();

	double GetLength();

	void build(double* coordinateE, vtkSmartPointer<vtkRenderer> renderer, double radius);
	void rebuild(double* coordinateE, vtkSmartPointer<vtkRenderer> renderer);
	void Remove(vtkSmartPointer<vtkRenderer> renderer);

private:
	double beginPosition[3];
	double endPosition[3];
	Point points[2];
	vtkSmartPointer<vtkActor> lineActor_;
};

