#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkInteractionStyle);


#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkcallbackcommand.h>
#include <vtkCoordinate.h>
#include <vtkDataSetMapper.h> 
#include <vtkRendererCollection.h>

#include "GridStyle.h"
#include <vtkStructuredGrid.h>
#include "InteractionCallback.h"


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

void InitScene(int width, int height) 
{
	vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
	vtkSmartPointer<vtkActor> axisesActor_ = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkActor> gridActor_ = vtkSmartPointer<vtkActor>::New();

	vtkSmartPointer<vtkPoints> axisesPoints = vtkPoints::New();
	axisesPoints->Allocate(4);
	axisesPoints->InsertNextPoint(0, -1, 0);
	axisesPoints->InsertNextPoint(0, 1, 0);
	axisesPoints->InsertNextPoint(-1, 0, 0);
	axisesPoints->InsertNextPoint(1, 0, 0);

	vtkSmartPointer<vtkPolyData> polydata = vtkPolyData::New();
	polydata->SetPoints(axisesPoints);

	vtkSmartPointer<vtkCellArray> lines = vtkCellArray::New();
	vtkIdType vert[2] = { 0, 1 };
	vtkIdType hor[2] = { 2, 3 };
	lines->InsertNextCell(2, vert);
	lines->InsertNextCell(2, hor);
	polydata->SetLines(lines);

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkPolyDataMapper::New();
	mapper->SetInputData(polydata);
	axisesActor_->SetMapper(mapper);
	axisesActor_->GetProperty()->SetColor(0, 0, 0);
	//// Create X lines

	double viewportSize[2] = { 1 ,1 };
	double grid_CellX = 1 / 100.0;

	int halfLinesNum = 100;

	// add the actor to the scene
	double q = 1.0;
	vtkSmartPointer<vtkStructuredGrid> structuredGrid =
		vtkSmartPointer<vtkStructuredGrid>::New();

	vtkSmartPointer<vtkPoints> gridPoints =
		vtkSmartPointer<vtkPoints>::New();

	for (unsigned int j = 0; j < halfLinesNum; j++)
	{
		for (unsigned int i = 0; i < halfLinesNum; i++)
		{
			gridPoints->InsertNextPoint(i * q, j * q, 0);
		}
	}

	structuredGrid->SetDimensions(halfLinesNum, halfLinesNum, 1);
	structuredGrid->SetPoints(gridPoints);
	structuredGrid->Modified();

	vtkSmartPointer<vtkDataSetMapper> gridMapper = vtkSmartPointer<vtkDataSetMapper>::New();
	gridMapper->SetInputData(structuredGrid);
	gridActor_->SetPosition(-halfLinesNum / 2, -halfLinesNum / 2, 0);
	gridActor_->SetMapper(gridMapper);
	gridActor_->GetProperty()->EdgeVisibilityOn();
	//gridActor_->GetProperty()->VertexVisibilityOff();
	gridActor_->GetProperty()->SetEdgeColor(18 / 256, 25 / 256, 16 / 256);
	gridActor_->GetProperty()->SetColor(0.995, 0.995, 0.995);
	

	renderer->AddActor(gridActor_);
	renderer->AddActor(axisesActor_);
	renderer->SetBackground(1, 1, 1);
	 

	vtkSmartPointer<vtkPropPicker> propPicker = 
		vtkSmartPointer<vtkPropPicker>::New(); 


	propPicker->PickFromListOn();
	propPicker->AddPickList(axisesActor_);


	vtkSmartPointer<vtkRenderWindow> renderwindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderwindow->AddRenderer(renderer);

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderwindow);


	vtkSmartPointer<vtkCallbackCommand> modifiedcallback =
		vtkSmartPointer<vtkCallbackCommand>::New();
	modifiedcallback->SetCallback(CameraModifiedCallback);


	renderer->SetActiveCamera(camera);

	camera->SetPosition(0, 0, halfLinesNum * 1.5);

	//renderer->ResetCamera();
	
	renderer->GetActiveCamera()->AddObserver(vtkCommand::ModifiedEvent, modifiedcallback);
	//renderer->AutomaticLightCreationOff();
	renderer->GradientEnvironmentalBGOff();
	
	//set picker to InteractionCallback
	//vtkSmartPointer<InteractionCallback> callback = vtkSmartPointer<InteractionCallback>::New();
	//callback->SetPicker(propPicker);

	gridStyle* grid = new gridStyle(gridActor_, axisesActor_, renderer);
	vtkSmartPointer<gridStyle> style =
		vtkSmartPointer<gridStyle>::Take(grid);
	////style->AddObserver(vtkCommand::MouseMoveEvent, callback);
	renderWindowInteractor->SetInteractorStyle(style);

	// render and interact
	renderwindow->SetSize(width, height);
	renderwindow->Render();

	renderWindowInteractor->Initialize();
	renderWindowInteractor->EnableRenderOn();
	renderWindowInteractor->Start();
}
