#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkInteractionStyle);

#include <vtkSmartPointer.h>
#include <vtkIdList.h>
#include <vtkProperty.h>
#include <vtkStructuredGrid.h>
#include <vtkXmlStructuredGridWriter.h>
#include <vtkMath.h>
#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkStructuredGridGeometryFilter.h>
#include <vtkXmlPolyDataWriter.h>
#include <vtkInteractorStylesWitch.h>
#include <vtkPlaneSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkCamera.h>
#include <vtkInteractorStyleRubberBand2D.h>
#include <vtkcallbackcommand.h>
#include <vtkAbstractPicker.h>
#include <vtkActor2D.h>
#include <vtkCaptionActor2D.h>
#include <vtkCoordinate.h>
#include <vtkFollower.h>
#include <vtkImageActor.h>
#include <vtkImageMapper3D.h>
#include <vtkImageCanvasSource2D.h>
#include <vtkCoordinate.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkPolyDataMapper2D.h>
#include <vtkObjectFactory.h>
#include <vtkPolyData.h>
#include <vtkProperty2D.h>
#include <vtkRendererCollection.h>
#include <vtkSphereSource.h>
#include <vtkSmartPointer.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkVectorText.h>

vtkSmartPointer<vtkActor> axesY_ = vtkSmartPointer<vtkActor>::New();
vtkSmartPointer<vtkActor> axesX_ = vtkSmartPointer<vtkActor>::New();
vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
vtkSmartPointer<vtkActor> axesActor_ = vtkSmartPointer<vtkActor>::New();
vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
double  lastCameraScale_ = 0;
bool isPaneOnly_ = false;

class MouseMovemenetStyle : public vtkInteractorStyleTrackballCamera
{
public:
	static MouseMovemenetStyle* New();
	vtkTypeMacro(MouseMovemenetStyle, vtkInteractorStyleTrackballCamera);
	bool flg = false;
	int countBack = 1, countForw = 1;
	double grid_CellX = 0.05;
	double grid_CellY = 0.05;
	double viewportSize[2];
	int cellScreenWidth_ = 0;
	double worldToScreenCoeff_ = 0;
	double mult = 1;
	virtual void OnLeftButtonDown()
	{
		// forward events
		SetTimerDuration(10);
		UseTimersOn();

		vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
		StartTimer();

	};

	virtual void OnRightButtonDown()
	{
		//cout << "Pressed right mouse button." << endl;
		vtkInteractorStyleTrackballCamera::OnRightButtonDown();
	}

	virtual void OnMiddleButtonDown()
	{
		//cout << "Pressed middle mouse button." << endl;
		vtkInteractorStyleTrackballCamera::OnMiddleButtonDown();
	}

	virtual void OnLeftButtonUp()
	{
		EndTimer();
		//cout << "Unpressed left mouse button." << endl;
		vtkInteractorStyleTrackballCamera::OnLeftButtonUp();

	}


	
	virtual void OnMouseWheelBackward() {
		vtkInteractorStyleTrackballCamera::OnMouseWheelBackward();
		countBack++;
		if (countBack % 5 == 0) {
			countBack = 1;
			grid_CellX *= 2;
		}
		OnTimer();
		flg = true;
	}
	virtual void OnMouseWheelForward() {
		vtkInteractorStyleTrackballCamera::OnMouseWheelForward();
		countForw++;
		if (countForw % 5 == 0) {
			countForw = 1;
			grid_CellX /= 2;
		}
		OnTimer();
		flg = true;
	}
	

	virtual void OnTimer() {
		int* sizes = renderer->GetSize();
		
		auto cameraScale = camera->GetParallelScale();
		isPaneOnly_ = (cameraScale == lastCameraScale_);
		lastCameraScale_ = cameraScale;
		auto height = 2 * cameraScale;
		worldToScreenCoeff_ = height / sizes[1];
		auto width = worldToScreenCoeff_ * sizes[0];
		viewportSize[0] = width  * camera->GetPosition()[2];
		viewportSize[1] = height  * camera->GetPosition()[2];
		
		auto scaleX = std::abs(camera->GetFocalPoint()[0]) + viewportSize[0] / 2.0;
		auto scaleY = std::abs(camera->GetFocalPoint()[1]) + viewportSize[1] / 2.0;

		double scale[3] = { scaleX, scaleY, 0 };
		axesActor_->SetScale(scale);

		if (!isPaneOnly_)
		{
			rebuildXlines();
			rebuildYlines();
		}
		if (flg) {
			rebuildXlines();
			rebuildYlines();
			flg = false;
		}

		//cout << "Pressed left mouse button." << viewportSize[0] << "===" << viewportSize[1] << endl;

		double xScale[3] = { viewportSize[0] / 2 + grid_CellX, 1, 0 };				// Так мы задаем длины линий решетки (поскольку сначала они были от -1 до 1). Прибавляем grid_.cell.x чтобы было немного с запасом, пототму что решетка рисуется не точно по центрн
		axesY_->SetScale(xScale);

		double yScale[3] = { 1, viewportSize[1] / 2 + grid_CellX, 0 };
		axesX_->SetScale(yScale);
		
		double xmove = floor(camera->GetFocalPoint()[0] / grid_CellX) * grid_CellX;
		double ymove = floor(camera->GetFocalPoint()[1] / grid_CellX) * grid_CellX;

		xmove += fmod(grid_CellX, grid_CellX);
		ymove += fmod(grid_CellX, grid_CellX);

		axesX_->SetPosition(xmove, camera->GetFocalPoint()[1], 0);
		axesY_->SetPosition(camera->GetFocalPoint()[0], ymove, 0);
	}

	void rebuildYlines() {
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
	void rebuildXlines() {
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

};



vtkStandardNewMacro(MouseMovemenetStyle);



static void CameraModifiedCallback(vtkObject* caller,
	long unsigned int vtkNotUsed(eventId),
	void* vtkNotUsed(clientData),
	void* vtkNotUsed(callData))
{
	//std::cout << caller->GetClassName() << " modified" << std::endl;

	vtkCamera* camera = static_cast<vtkCamera*>(caller);

	camera->SetFocalPoint(camera->GetPosition()[0], camera->GetPosition()[1], 0);

	// print the interesting stuff
	/*
	std::cout << "\tPosition: "
		<< camera->GetPosition()[0] << ", "
		<< camera->GetPosition()[1] << ", "
		<< camera->GetPosition()[2] << std::endl;
	std::cout << "\tFocal point: "
		<< camera->GetFocalPoint()[0] << ", "
		<< camera->GetFocalPoint()[1] << ", "
		<< camera->GetFocalPoint()[2] << std::endl;
	*/
}



vtkSmartPointer<vtkStructuredGrid> creategrid(unsigned int gridsize, double q = 1.0) {
	gridsize++;
	vtkSmartPointer<vtkStructuredGrid> structuredgrid =
		vtkSmartPointer<vtkStructuredGrid>::New();

	vtkSmartPointer<vtkPoints> points =
		vtkSmartPointer<vtkPoints>::New();

	for (unsigned int j = 0; j < gridsize; j++)
	{
		for (unsigned int i = 0; i < gridsize; i++)
		{
			points->InsertNextPoint(i * q, j * q, 0);
		}
	}

	structuredgrid->SetDimensions(gridsize, gridsize, 1);
	structuredgrid->SetPoints(points);
	structuredgrid->Modified();

	return structuredgrid;
}

int main() {



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
	////

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
	////

	//halfLinesNum = 10;

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
	renderer->SetBackground(1, 1, 1);//background color

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
	// Обсудить это. Ибо меняет картинку. Становится немного лучше.
	renderer->AutomaticLightCreationOff();
	//renderer->GradientEnvironmentalBGOff();

	cout << camera->GetPosition()[2] << endl;


	vtkSmartPointer<MouseMovemenetStyle> style =
		vtkSmartPointer<MouseMovemenetStyle>::New();
	renderWindowInteractor->SetInteractorStyle(style);
	// render and interact
	renderwindow->SetSize(800, 800);
	renderwindow->Render();

	renderWindowInteractor->Initialize();
	renderWindowInteractor->EnableRenderOn();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}
