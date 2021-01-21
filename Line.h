#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkPolyDataMapper.h>
#include <vtkLineSource.h>
#include <vtkProperty.h>


class Line
{
public:
	Line();

public:
	void SetEndPosition(double* coordinate);
	double* GetEndPosition();

	void SetBeginPosition(double* coordinate);
	double* GetBeginPosition();


	void build(double* coordinateE, vtkSmartPointer<vtkActor> lineActor, vtkSmartPointer<vtkRenderer> renderer);

private:
	double* beginPosition;
	double* endPosition;
};

