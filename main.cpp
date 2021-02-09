#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkInteractionStyle);


#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkCamera.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkCallbackCommand.h>
#include <vtkPlotPoints.h>
#include <vtkTable.h>
#include <vtkDataObjectToTable.h>
#include <vtkFloatArray.h>
#include <vtkMapper.h>

#include "Grid.h"
#include "PlaneGrid.h"
#include "InteractorStyle.h"
#include "MainAxes.h"
#include "Point.h"


static void CameraModifiedCallback(vtkObject* caller,
	long unsigned int vtkNotUsed(eventId),
	void* vtkNotUsed(clientData),
	void* vtkNotUsed(callData))
{
	vtkCamera* camera = static_cast<vtkCamera*>(caller);
	camera->SetFocalPoint(camera->GetPosition()[0], camera->GetPosition()[1], 0);
}

int main(int, char* [])
{
	vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	
<<<<<<< HEAD
	double cell[2] = { 1,1 };
	double parallelScale = 4;
	int sizewin[2] = { 600,600 };

>>>>>>> Ursus14

	PlaneGrid* plane = new PlaneGrid(cell,sizewin,parallelScale);
	MainAxes* axes = new MainAxes(sizewin,parallelScale);
	Point* marker = new Point(0.03*cell[0]);
	marker->VisibilityOff();
	std::vector<Point> drawPoints;
	
	renderer->AddActor(plane->GetActor(0));
	renderer->AddActor(plane->GetActor(1));
	renderer->AddActor(plane->GetActor(2));
	renderer->AddActor(plane->GetActor(3));
	renderer->AddActor(axes->GetActor());
	renderer->AddActor(marker->GetActor());
	renderer->SetBackground(1, 1, 1);

	vtkSmartPointer<vtkRenderWindow> renderwindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderwindow->AddRenderer(renderer);

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderwindow);

	camera->ParallelProjectionOn();
	camera->SetParallelScale(parallelScale);

	vtkSmartPointer<vtkCallbackCommand> modifiedcallback =
		vtkSmartPointer<vtkCallbackCommand>::New();
	modifiedcallback->SetCallback(CameraModifiedCallback);

	renderer->SetActiveCamera(camera);
	renderer->GetActiveCamera()->AddObserver(vtkCommand::ModifiedEvent, modifiedcallback);
	
	renderer->AutomaticLightCreationOff();
	renderer->GradientEnvironmentalBGOff();
	
	InteractorStyle* newGrid = new InteractorStyle(plane, axes,marker, drawPoints, renderer);

	vtkSmartPointer<InteractorStyle> style =
		vtkSmartPointer<InteractorStyle>::Take(newGrid);
	renderWindowInteractor->SetInteractorStyle(style);

	renderwindow->SetSize(sizewin);
	renderwindow->Render();
	renderWindowInteractor->Initialize();
	renderWindowInteractor->EnableRenderOn();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}