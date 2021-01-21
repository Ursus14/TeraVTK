#include <vtkPoints.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkLineSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>

class BrokenLine
{
public:
	BrokenLine();
public:

	void SetPoints(vtkPoints* points);

	vtkPoints* GetPoints();

	void build(vtkPoints* points, vtkSmartPointer<vtkRenderer> renderer);

private:
	vtkPoints* points;

};

