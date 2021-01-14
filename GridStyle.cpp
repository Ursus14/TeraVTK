#include "GridStyle.h"

vtkStandardNewMacro(gridStyle);

gridStyle::gridStyle()
{

}

gridStyle::gridStyle(vtkSmartPointer<vtkActor> axesX, vtkSmartPointer<vtkActor> axesY, vtkSmartPointer<vtkActor> axesMain, vtkSmartPointer<vtkActor> actorMarker, vtkSmartPointer<vtkRenderer> renderer)
{
	axesX_ = axesX;
	axesY_ = axesY;
	axesMain_ = axesMain;
	camera_ = renderer->GetActiveCamera();
	renderer_ = renderer;
	actorMarker_ = actorMarker;
	zPosition = camera_->GetPosition()[2];
}

void gridStyle::OnMouseMove()
{
	vtkInteractorStyleTrackballCamera::OnMouseMove();
    double x = Interactor->GetLastEventPosition()[0];
	double y = Interactor->GetLastEventPosition()[1];
	int* sizeWin = Interactor->GetRenderWindow()->GetSize();

	vtkSmartPointer<vtkCoordinate> coordinate =
		vtkSmartPointer<vtkCoordinate>::New();
	coordinate->SetCoordinateSystemToDisplay();
	coordinate->SetValue(x, y, 0);
	double* world = coordinate->GetComputedWorldValue(Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
	//std::cout << "World coordinate: " << world[0] << ", " << world[1] << ", " << world[2] << std::endl;
	world[0] = floor(world[0] / grid_CellX + 0.5) * grid_CellX;
	world[1] = floor(world[1] / grid_CellX + 0.5) * grid_CellX;
	
	vtkSmartPointer<vtkRegularPolygonSource> marker = vtkSmartPointer<vtkRegularPolygonSource>::New();
	marker->SetNumberOfSides(50);
	marker->SetRadius(0.005);
	marker->SetCenter(world[0], world[1], 0);
	
	vtkPolyDataMapper::SafeDownCast(actorMarker_->GetMapper())->SetInputConnection(marker->GetOutputPort());
	actorMarker_->SetVisibility(1);
	renderer_->RemoveActor(actorMarker_);
	renderer_->AddActor(actorMarker_);
	Interactor->GetRenderWindow()->Render();
}

void gridStyle::OnLeave() {
	vtkInteractorStyle::OnLeave();
	actorMarker_->SetVisibility(0);
	Interactor->GetRenderWindow()->Render();
}

void gridStyle::OnLeftButtonDown()
{
	SetTimerDuration(1);
	UseTimersOn();
	vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
	StartTimer();

	//std::cout << "Pressed left mouse button." << std::endl;
	NumberOfClicks++;
	//std::cout << "NumberOfClicks = " << this->NumberOfClicks << std::endl;
	int pickPosition[2];
	GetInteractor()->GetEventPosition(pickPosition);

	int xdist = pickPosition[0] - this->PreviousPosition[0];
	int ydist = pickPosition[1] - this->PreviousPosition[1];

	this->PreviousPosition[0] = pickPosition[0];
	this->PreviousPosition[1] = pickPosition[1];

	int moveDistance = (int)sqrt((double)(xdist * xdist + ydist * ydist));

	// Reset numClicks - If mouse moved further than resetPixelDistance
	if (moveDistance > this->ResetPixelDistance)
	{
		this->NumberOfClicks = 1;
	}

	if (this->NumberOfClicks == 2)
	{
		double x = Interactor->GetLastEventPosition()[0];
		double y = Interactor->GetLastEventPosition()[1];
		int* sizeWin = Interactor->GetRenderWindow()->GetSize();

		vtkSmartPointer<vtkCoordinate> coordinate =
			vtkSmartPointer<vtkCoordinate>::New();
		coordinate->SetCoordinateSystemToDisplay();
		coordinate->SetValue(x, y, 0);
		double* world = coordinate->GetComputedWorldValue(Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
		//std::cout << "World coordinate: " << world[0] << ", " << world[1] << ", " << world[2] << std::endl;
		world[0] = floor(world[0] / grid_CellX + 0.5) * grid_CellX;
		world[1] = floor(world[1] / grid_CellX + 0.5) * grid_CellX;


		
		point* p = new point(world, renderer_);
		p->draw();
		Interactor->GetRenderWindow()->Render();

		std::cout << "Double clicked." << std::endl;
		this->NumberOfClicks = 0;
	}	
}

void gridStyle::OnLeftButtonUp()
{
	EndTimer();
	vtkInteractorStyleTrackballCamera::OnLeftButtonUp();

}

void gridStyle::OnRightButtonDown()
{
	camera_->SetPosition(camera_->GetPosition()[0], camera_->GetPosition()[1], zPosition);

}
void gridStyle::OnMouseWheelBackward()
{

	vtkInteractorStyleTrackballCamera::OnMouseWheelBackward();
	zPosition = camera_->GetPosition()[2];

	countBack++;
	if (countBack % 5 == 0) {
		countBack = 1;
		
		//grid_CellX *= 2;
	}
	OnTimer();
	flgRebuildLines = true;
	OnMouseMove();
}

void gridStyle::OnMouseWheelForward()
{

	vtkInteractorStyleTrackballCamera::OnMouseWheelForward();
	zPosition = camera_->GetPosition()[2];

	countForw++;
	if (countForw % 5 == 0) {
		countForw = 1;
		
		//grid_CellX /= 2;
	}
	OnTimer();
	flgRebuildLines = true;
	OnMouseMove();
}

void gridStyle::OnTimer()
{
	if (camera_->GetPosition()[2] / zPosition < 0.99) {
		camera_->SetPosition(camera_->GetPosition()[0], camera_->GetPosition()[1], zPosition);
	}

	int* sizes = renderer_->GetSize();

	auto cameraScale = camera_->GetParallelScale();
	isPaneOnly_ = (cameraScale == lastCameraScale_);
	lastCameraScale_ = cameraScale;
	auto height = 2 * cameraScale;
	worldToScreenCoeff_ = height / sizes[1];
	auto width = worldToScreenCoeff_ * sizes[0];
	viewportSize[0] = width * camera_->GetPosition()[2];
	viewportSize[1] = height * camera_->GetPosition()[2];

	auto scaleX = std::abs(camera_->GetFocalPoint()[0]) + viewportSize[0] / 2.0;
	auto scaleY = std::abs(camera_->GetFocalPoint()[1]) + viewportSize[1] / 2.0;

	double scale[3] = { scaleX, scaleY, 0 };
	axesMain_->SetScale(scale);

	if (!isPaneOnly_)
	{
		rebuildXlines();
		rebuildYlines();
	}
	if (flgRebuildLines) {
		rebuildXlines();
		rebuildYlines();
		flgRebuildLines = false;
	}


	double xScale[3] = { viewportSize[0] / 2 + grid_CellX, 1, 0 };
	axesY_->SetScale(xScale);

	double yScale[3] = { 1, viewportSize[1] / 2 + grid_CellX, 0 };
	axesX_->SetScale(yScale);

	double xmove = floor(camera_->GetFocalPoint()[0] / grid_CellX) * grid_CellX;
	double ymove = floor(camera_->GetFocalPoint()[1] / grid_CellX) * grid_CellX;

	xmove += fmod(grid_CellX, grid_CellX);
	ymove += fmod(grid_CellX, grid_CellX);

	// gridActor->SetPosition(xmove, ymove, 0);

	axesX_->SetPosition(xmove, camera_->GetFocalPoint()[1], 0);
	axesY_->SetPosition(camera_->GetFocalPoint()[0], ymove, 0);
}

void gridStyle::rebuildYlines()
{
	const int halfLinesNum = floor((viewportSize[1] / grid_CellX) / 2) + 1;

	vtkSmartPointer<vtkPoints> points = vtkPoints::New();
	points->Allocate(2 * halfLinesNum);

	vtkSmartPointer<vtkCellArray> lines = vtkCellArray::New();

	points->InsertNextPoint(-1, 0, 0);
	points->InsertNextPoint(1, 0, 0);
	vtkIdType cell[] = { 0, 1 };
	lines->InsertNextCell(2, cell);
	int num = 1;
	for (int i = 1; i <= halfLinesNum; i++)
	{
		for (int j = -1; j <= 1; j += 2)
		{
			points->InsertNextPoint(-1, j * i * grid_CellX, 0);
			points->InsertNextPoint(1, j * i * grid_CellX, 0);

			vtkIdType cell[] = { 2 * num, 2 * num + 1 };
			lines->InsertNextCell(2, cell);
			num++;
		}
	}

	vtkSmartPointer<vtkPolyData> polydata = vtkPolyData::New();
	polydata->SetPoints(points);
	polydata->SetLines(lines);
	vtkPolyDataMapper::SafeDownCast(axesY_->GetMapper())->SetInputData(polydata);
}

void gridStyle::rebuildXlines()
{
	const int halfLinesNum = floor((viewportSize[0] / grid_CellX) / 2) + 1;

	vtkSmartPointer<vtkPoints> points = vtkPoints::New();
	points->Allocate(2 * halfLinesNum);

	vtkSmartPointer<vtkCellArray> lines = vtkCellArray::New();

	points->InsertNextPoint(0, -1, 0);
	points->InsertNextPoint(0, 1, 0);
	vtkIdType cell[] = { 0, 1 };
	lines->InsertNextCell(2, cell);
	int num = 1;
	for (int i = 1; i <= halfLinesNum; i++)
	{
		for (int j = -1; j <= 1; j += 2)
		{
			points->InsertNextPoint(j * i * grid_CellX, -1, 0);
			points->InsertNextPoint(j * i * grid_CellX, 1, 0);

			vtkIdType cell[] = { 2 * num, 2 * num + 1 };
			lines->InsertNextCell(2, cell);
			num++;
		}
	}

	vtkSmartPointer<vtkPolyData> polydata = vtkPolyData::New();
	polydata->SetPoints(points);
	polydata->SetLines(lines);
	vtkPolyDataMapper::SafeDownCast(axesX_->GetMapper())->SetInputData(polydata);
}

void gridStyle::rebuildMarker(double* coordinate)
{

}
