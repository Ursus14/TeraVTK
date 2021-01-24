#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkInteractionStyle);

#include "GridStyleNine.h"
#include "GridStyleFour.h"



#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkcallbackcommand.h>
#include <vtkCoordinate.h>
#include <vtkRendererCollection.h>
#include <vtkLineSource.h>
#include <vtkRegularPolygonSource.h>


void InitScene(int width, int height);

static void CameraModifiedCallback(vtkObject* caller,
	long unsigned int vtkNotUsed(eventId),
	void* vtkNotUsed(clientData),
	void* vtkNotUsed(callData))
{
	vtkCamera* camera = static_cast<vtkCamera*>(caller);
	camera->SetFocalPoint(camera->GetPosition()[0], camera->GetPosition()[1], 0);
}



int main() {

	InitScene(800, 800);
	return EXIT_SUCCESS;
}



void InitScene(int width, int height) {
	vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
	vtkSmartPointer<vtkActor> axesActor_ = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkActor> actorMarker_ = vtkSmartPointer<vtkActor>::New();


	// Create axes
	vtkSmartPointer<vtkPoints> points = vtkPoints::New();
	points->Allocate(4);
	points->InsertNextPoint(0, -1, 0);
	points->InsertNextPoint(0, 1, 0);
	points->InsertNextPoint(-1, 0, 0);
	points->InsertNextPoint(1, 0, 0);

	vtkSmartPointer<vtkPolyData> polydata = vtkPolyData::New();
	polydata->SetPoints(points);

	vtkSmartPointer<vtkCellArray> lines = vtkCellArray::New();
	vtkIdType vert[2] = { 0, 1 };
	vtkIdType hor[2] = { 2, 3 };
	lines->InsertNextCell(2, vert);
	lines->InsertNextCell(2, hor);
	polydata->SetLines(lines);


	vtkSmartPointer<vtkPolyDataMapper> axesMapper = vtkSmartPointer<vtkPolyDataMapper>::New();

	axesMapper->SetInputData(polydata);

	axesActor_->SetMapper(axesMapper);

	axesActor_->GetProperty()->SetColor(0.0, 0.0, 0.0);

	// camera ---------------
	 
	camera->ParallelProjectionOn();

	// -------------
	

	// create grid 

	Grid* gridA = new Grid(0.0, 0.0, 1.0);
	for (size_t i = 0; i < gridA->size(); i++) {
		renderer->AddActor(gridA->getActorByIndex(i));
	}
	Grid* gridB = new Grid(-1.0, 0.0, 1.0);
	for (size_t i = 0; i < gridB->size(); i++) {
		renderer->AddActor(gridB->getActorByIndex(i));
	}
	Grid* gridC = new Grid(0.0, -1.0, 1.0);
	for (size_t i = 0; i < gridC->size(); i++) {
		renderer->AddActor(gridC->getActorByIndex(i));
	}
	Grid* gridD = new Grid(-1.0, -1.0, 1.0);
	for (size_t i = 0; i < gridD->size(); i++) {
		renderer->AddActor(gridD->getActorByIndex(i));
	}
	// ------------------------

	renderer->AddActor(axesActor_);

	// -------------
	
		
	renderer->SetBackground(1, 1, 1);
	double b = 1.0;
	camera->SetPosition(0.0, 0.0, b);

	// create Marker and add to renderer
	vtkSmartPointer<vtkRegularPolygonSource> marker = vtkSmartPointer<vtkRegularPolygonSource>::New();
	marker->SetNumberOfSides(50);
	marker->SetRadius(0.0001);
	marker->SetCenter(0, 0, 0);


	vtkSmartPointer<vtkPolyDataMapper> mapperMarker = vtkPolyDataMapper::New();
	mapperMarker->SetInputConnection(marker->GetOutputPort());
	actorMarker_->SetMapper(mapperMarker);
	actorMarker_->GetProperty()->SetColor(0.47, 0.07, 0.07);

	renderer->AddActor(actorMarker_);
	// ---------------------------


	

	renderer->SetActiveCamera(camera);
	

	vtkSmartPointer<vtkRenderWindow> renderwindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderwindow->AddRenderer(renderer);
	renderwindow->SetWindowName("Grid");
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderwindow);

	vtkSmartPointer<vtkCallbackCommand> modifiedcallback =
		vtkSmartPointer<vtkCallbackCommand>::New();
	modifiedcallback->SetCallback(CameraModifiedCallback);


	renderer->SetActiveCamera(camera);
	//renderer->ResetCamera();
	renderer->GetActiveCamera()->AddObserver(vtkCommand::ModifiedEvent, modifiedcallback);
	//renderer->AutomaticLightCreationOff();
	//renderer->GradientEnvironmentalBGOff();



	GridStyleFour* grid = new GridStyleFour(gridA, gridB, gridC, gridD, axesActor_, actorMarker_, renderer);
	vtkSmartPointer<GridStyleFour> style =
		vtkSmartPointer<GridStyleFour>::Take(grid);

	renderWindowInteractor->SetInteractorStyle(style); 

	// render and interact
	renderwindow->SetSize(width, height);
	renderwindow->Render();
	

	renderWindowInteractor->Initialize();
	renderWindowInteractor->EnableRenderOn();
	renderWindowInteractor->Start();
}
