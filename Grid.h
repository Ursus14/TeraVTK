#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkcallbackcommand.h>
#include <vtkRendererCollection.h>
#include <vtkLineSource.h>
#include <vtkPolyDataMapper.h>

class Grid 
{
public:
	Grid(
		double x, 
		double y
	);


public:
	vtkSmartPointer<vtkActor> getActorByIndex(int i);

	double* GetPosition();
	void SetPosition(double x, double y);
	int size();
	void rebuild();
	
private:
	vtkSmartPointer<vtkActor> doActor(double s_x, double s_y, double x, double y, double d_x, double d_y);
	void doGrid(double d_x, double d_y);
	void init();

private:
	std::vector<vtkSmartPointer<vtkActor>> grid;
	double x_ = 0.0;
	double y_ = 0.0;
};