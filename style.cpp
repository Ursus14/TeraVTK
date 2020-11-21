#include <vtkInteractorStyleTrackballCamera.h>

#include "style.h"
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkCamera.h>
#include <vtkRenderer.h>


class MouseMovemenetStyle : public vtkInteractorStyleTrackballCamera
{
public:
	static MouseMovemenetStyle* New();
	vtkSmartPointer<vtkActor> axesY_;
	vtkSmartPointer<vtkActor> axesX_;
	vtkSmartPointer<vtkCamera> camera;
	vtkSmartPointer<vtkRenderer> renderer;
	vtkSmartPointer<vtkActor> axesActor_;
	vtkSmartPointer<vtkActor> point_;
	bool isPaneOnly_ = false;
	
	vtkTypeMacro(MouseMovemenetStyle, vtkInteractorStyleTrackballCamera);

	MouseMovemenetStyle(
		vtkSmartPointer<vtkActor> axesY_, 
		vtkSmartPointer<vtkActor> axesX_, 
		vtkSmartPointer<vtkCamera> camera, 
		vtkSmartPointer<vtkRenderer> renderer, 
		vtkSmartPointer<vtkActor> axesActor_,
		vtkSmartPointer<vtkActor> point_)
	{
		this->axesY_ = axesY_;
		this->axesX_ = axesX_;
		this->camera = camera;
		this->renderer = renderer;
		this->axesActor_ = axesActor_;
		this->point_ = point_;
	};
	bool flg = false;
	bool mut = false;
	int countBack = 1, countForw = 1;
	double grid_CellX = 0.05;
	double grid_CellY = 0.05;
	double viewportSize[2];
	int cellScreenWidth_ = 0;
	double worldToScreenCoeff_ = 0;
	double mult = 1;
	double zPosition = camera->GetPosition()[2];
	double  lastCameraScale_ = 0;

	virtual void OnLeftButtonDown()
	{
		mut = true;
		// forward events
		SetTimerDuration(1);
		UseTimersOn();
		vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
		StartTimer();
	};

	virtual void OnLeftButtonUp()
	{
		EndTimer();
		//cout << "Unpressed left mouse button." << endl;
		vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
	}

	virtual void OnMouseWheelBackward() {
		mut = false;
		vtkInteractorStyleTrackballCamera::OnMouseWheelBackward();
		countBack++;
		if (countBack % 5 == 0) {
			countBack = 1;
			//grid_CellX *= 2;
		}
		OnTimer();
		flg = true;
		zPosition = camera->GetPosition()[2];
	}

	virtual void OnMouseWheelForward() {
		mut = false;
		vtkInteractorStyleTrackballCamera::OnMouseWheelForward();
		countForw++;
		if (countForw % 5 == 0) {
			countForw = 1;
			//grid_CellX /= 2;
		}
		OnTimer();
		flg = true;
		zPosition = camera->GetPosition()[2];
	}

	virtual void OnTimer() {
		if (zPosition != camera->GetPosition()[2] && mut)
		{
			camera->SetPosition(camera->GetPosition()[0], camera->GetPosition()[1], zPosition);
		}

		int* sizes = renderer->GetSize();

		auto cameraScale = camera->GetParallelScale();
		isPaneOnly_ = (cameraScale == lastCameraScale_);
		lastCameraScale_ = cameraScale;
		auto height = 2 * cameraScale;
		worldToScreenCoeff_ = height / sizes[1];
		auto width = worldToScreenCoeff_ * sizes[0];
		viewportSize[0] = width * camera->GetPosition()[2];
		viewportSize[1] = height * camera->GetPosition()[2];

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


