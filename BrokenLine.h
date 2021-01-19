#include <vtkPoints.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>

class BrokenLine
{
	BrokenLine();
public:

	void SetPoints(vtkPoints* points);

	vtkPoints* GetPoints();

	void build(vtkPoints* points, vtkSmartPointer<vtkRenderer> renderer);

};

