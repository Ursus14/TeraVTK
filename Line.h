#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkPolyDataMapper.h>
#include <vtkLineSource.h>
#include <vtkProperty.h>


class Line
{
public:
	Line(vtkSmartPointer<vtkActor> lineActor);

public:
	void SetEndPosition(double* coordinate);
	double* GetEndPosition();

	void SetBeginPosition(double* coordinate);
	double* GetBeginPosition();

	void build(double* coordinateE, vtkSmartPointer<vtkRenderer> renderer);
	void rebuild(double* coordinateE, vtkSmartPointer<vtkRenderer> renderer);

private:
	double beginPosition[2];
	double endPosition[2];
	vtkSmartPointer<vtkActor> lineActor_;
};

