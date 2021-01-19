#include <vtkSmartPointer.h>
#include <vtkRenderer.h>

class Line
{
	Line();

	void SetEndPosition(double* coordinate);
	double* GetEndPosition();

	void SetBeginPosition(double* coordinate);
	double* GetBeginPosition(double* coordinate);


	void build(double* coordinateB, double* coordinateE, vtkSmartPointer<vtkActor> lineActor, vtkSmartPointer<vtkRenderer> renderer);

private:
	double* beginPosition;
	double* endPosition;
};

