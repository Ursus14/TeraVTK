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
	Line(vtkSmartPointer<vtkActor> lineActor, double* coordinateB, double* coordinateE);

public:
	void SetEndPosition(double* coordinate);
	double* GetEndPosition();

	void SetBeginPosition(double* coordinate);
	double* GetBeginPosition();

	double GetLengthSquare();

	void build(double* coordinateE, vtkSmartPointer<vtkRenderer> renderer, double radius);
	void rebuild(double* coordinateE, vtkSmartPointer<vtkRenderer> renderer);
	void Remove(vtkSmartPointer<vtkRenderer> renderer);

private:
	double beginPosition[2];
	double endPosition[2];
	Point points[2];
	vtkSmartPointer<vtkActor> lineActor_;
};

