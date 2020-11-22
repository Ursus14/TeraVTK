#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkInteractionStyle);

#include "displayGridStyle.h"

#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkcallbackcommand.h>
#include <vtkCoordinate.h>
#include <vtkRendererCollection.h>

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
	vtkSmartPointer<vtkActor> axesY_ = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> axesX_ = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
	vtkSmartPointer<vtkActor> axesActor_ = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();

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

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkPolyDataMapper::New();
	mapper->SetInputData(polydata);
	axesActor_->SetMapper(mapper);
	axesActor_->GetProperty()->SetColor(0, 0, 0);
	//// Create X lines

	double viewportSize[2] = { 1 ,1 }, grid_CellX = 1 / 20.0;

	int halfLinesNum = 40;

	vtkSmartPointer<vtkPoints> points1 = vtkPoints::New();
	points1->Allocate(2 * halfLinesNum);

	vtkSmartPointer<vtkCellArray> lines1 = vtkCellArray::New();

	points1->InsertNextPoint(-1, 0, 0);
	points1->InsertNextPoint(1, 0, 0);
	vtkIdType cell1[] = { 0, 1 };
	lines1->InsertNextCell(2, cell1);
	int num = 1;
	for (int i = 1; i <= halfLinesNum; i++)
	{
		for (int j = -1; j <= 1; j += 2)
		{
			points1->InsertNextPoint(-1, j * i * grid_CellX, 0);
			points1->InsertNextPoint(1, j * i * grid_CellX, 0);

			vtkIdType cell1[] = { 2 * num, 2 * num + 1 };
			lines1->InsertNextCell(2, cell1);
			num++;
		}
	}

	vtkSmartPointer<vtkPolyData> polydata1 = vtkPolyData::New();
	polydata1->SetPoints(points1);
	polydata1->SetLines(lines1);

	vtkSmartPointer<vtkPolyDataMapper> mapper1 = vtkPolyDataMapper::New();
	mapper1->SetInputData(polydata1);
	axesY_->SetMapper(mapper1);
	axesY_->GetProperty()->SetColor(0.8, 0.8, 0.8);
	//// Create Y lines

	vtkSmartPointer<vtkPoints> points2 = vtkPoints::New();
	points2->Allocate(2 * halfLinesNum);

	vtkSmartPointer<vtkCellArray> lines2 = vtkCellArray::New();

	points2->InsertNextPoint(0, -1, 0);
	points2->InsertNextPoint(0, 1, 0);
	vtkIdType cell2[] = { 0, 1 };
	lines2->InsertNextCell(2, cell2);
	num = 1;
	for (int i = 1; i <= halfLinesNum; i++)
	{
		for (int j = -1; j <= 1; j += 2)
		{
			points2->InsertNextPoint(j * i * grid_CellX, -1, 0);
			points2->InsertNextPoint(j * i * grid_CellX, 1, 0);

			vtkIdType cell2[] = { 2 * num, 2 * num + 1 };
			lines2->InsertNextCell(2, cell2);
			num++;
		}
	}

	vtkSmartPointer<vtkPolyData> polydata2 = vtkPolyData::New();
	polydata2->SetPoints(points2);
	polydata2->SetLines(lines2);

	vtkSmartPointer<vtkPolyDataMapper> mapper2 = vtkPolyDataMapper::New();
	mapper2->SetInputData(polydata2);
	axesX_->SetMapper(mapper2);
	axesX_->GetProperty()->SetColor(0.8, 0.8, 0.8);

	// add the actor to the scene

	renderer->AddActor(axesY_);
	renderer->AddActor(axesX_);
	renderer->AddActor(axesActor_);
	renderer->SetBackground(1, 1, 1);


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
	//renderer->ResetCamera();
	renderer->GetActiveCamera()->AddObserver(vtkCommand::ModifiedEvent, modifiedcallback);
	renderer->AutomaticLightCreationOff();
	renderer->GradientEnvironmentalBGOff();

	cout << camera->GetPosition()[2] << endl;

	displayGridStyle* grid = new displayGridStyle(axesX_, axesY_, axesActor_, camera, renderer);
	vtkSmartPointer<displayGridStyle> style =
		vtkSmartPointer<displayGridStyle>::Take(grid);

	renderWindowInteractor->SetInteractorStyle(style);

	// render and interact
	renderwindow->SetSize(width, height);
	renderwindow->Render();

	renderWindowInteractor->Initialize();
	renderWindowInteractor->EnableRenderOn();
	renderWindowInteractor->Start();
}
