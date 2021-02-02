#include "InteractorStyle.h"

vtkStandardNewMacro(InteractorStyle);

InteractorStyle::InteractorStyle()
{
}

InteractorStyle::InteractorStyle(PlaneGrid* _plane, MainAxes* _axes, Point* _marker, std::vector<Point> _drawPoints, vtkSmartPointer<vtkRenderer> _renderer) {
	plane = _plane;
	camera = _renderer->GetActiveCamera();
	renderer = _renderer;
	axes = _axes;
	marker = _marker;
	drawPoints = _drawPoints;
}

void InteractorStyle::OnMouseWheelBackward() {
	vtkInteractorStyleTrackballCamera::OnMouseWheelBackward();
	plane->RebuildPlane(camera, Interactor->GetRenderWindow()->GetSize());
	axes->RebuildAxes(camera, Interactor->GetRenderWindow()->GetSize());
}

void InteractorStyle::OnMouseWheelForward() {
	vtkInteractorStyleTrackballCamera::OnMouseWheelForward();
	plane->RebuildPlane(camera, Interactor->GetRenderWindow()->GetSize());
	axes->RebuildAxes(camera, Interactor->GetRenderWindow()->GetSize());
}

void InteractorStyle::OnRightButtonDown() {
	vtkInteractorStyleTrackballCamera::OnMiddleButtonDown();
}

void InteractorStyle::OnRightButtonUp() {
	vtkInteractorStyleTrackballCamera::OnMiddleButtonUp();
}
void InteractorStyle::OnLeftButtonDown() {
	DoubleClickMouse::OnLeftButtonDown();
	prevPosition[0] = GetCurrentMousePosition()[0];
	prevPosition[1] = GetCurrentMousePosition()[1];
	line_->SetBeginPosition(prevPosition);
	isAddLine = true;
	Point* point = new Point(GetCurrentMousePosition());
	renderer->AddActor(point->GetActor());
	line_->build(GetCurrentMousePosition(), renderer);
	Interactor->GetRenderWindow()->Render();
}

void InteractorStyle::OnLeftButtonUp() {
	double* world = GetCurrentMousePosition();
	if (prevPosition[0] == world[0] && prevPosition[1] == world[1]) {

	}
	line_->SetBeginPosition(prevPosition);
	line_->build(world, renderer);
	Point* point = new Point(world);
	renderer->AddActor(point->GetActor());

	for (auto line : lines_) {
		if (lines->intersect(line, line_)) {
			Point* point_ = new Point(lines->getIntersectionPoint(line, line_));
			renderer->AddActor(point_->GetActor());
		}
	}
	lines_.push_back(line_);

	Interactor->GetRenderWindow()->Render();
	line_ = new Line(vtkSmartPointer<vtkActor>::New());
	isAddLine = false;
}

void InteractorStyle::OnMouseMove() {
	vtkInteractorStyleTrackballCamera::OnMouseMove();
	plane->HitTestingAtBorder(camera);
	axes->RebuildAxes(camera, Interactor->GetRenderWindow()->GetSize());

	double coordinate[2];

	coordinate[0] = GetCurrentMousePosition()[0];
	coordinate[1] = GetCurrentMousePosition()[1];

	if (isAddLine) {
		line_->rebuild(coordinate, renderer);
		followToLine(coordinate);
	}
	marker->SetPosition(coordinate);
	marker->VisibilityOn();
	renderer->RemoveActor(marker->GetActor());
	renderer->AddActor(marker->GetActor());
	Interactor->GetRenderWindow()->Render();

}

void InteractorStyle::OnLeave() {
	vtkInteractorStyleTrackballCamera::OnLeave();
	marker->VisibilityOff();
	Interactor->GetRenderWindow()->Render();
}

void InteractorStyle::OnLeftDoubleClick() {
	DoubleClickMouse::OnLeftDoubleClick();
	double* world = GetCurrentMousePosition();
	drawPoints.push_back(Point(world));
	renderer->AddActor(drawPoints[drawPoints.size() - 1].GetActor());
}

double* InteractorStyle::GetCurrentMousePosition() {
	double x = Interactor->GetEventPosition()[0];
	double y = Interactor->GetEventPosition()[1];

	vtkSmartPointer<vtkCoordinate> coordinate =
		vtkSmartPointer<vtkCoordinate>::New();
	coordinate->SetCoordinateSystemToDisplay();
	coordinate->SetValue(x, y, 0);
	double* world = coordinate->GetComputedWorldValue(Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
	world[0] = floor(world[0] / plane->GetCell()[0] + 0.5) * plane->GetCell()[0];
	world[1] = floor(world[1] / plane->GetCell()[1] + 0.5) * plane->GetCell()[1];
	return world;
}

double* InteractorStyle::GetViewportBorder() {
	int xmax = Interactor->GetRenderWindow()->GetSize()[0];
	int ymax = Interactor->GetRenderWindow()->GetSize()[1];

	int xmin = 0;
	int ymin = 0;

	vtkSmartPointer<vtkCoordinate> coordinateMax =
		vtkSmartPointer<vtkCoordinate>::New();
	coordinateMax->SetCoordinateSystemToDisplay();
	coordinateMax->SetValue(xmax, ymax, 0);
	double* worldMax = coordinateMax->GetComputedWorldValue(Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());


	vtkSmartPointer<vtkCoordinate> coordinateMin =
		vtkSmartPointer<vtkCoordinate>::New();
	coordinateMin->SetCoordinateSystemToDisplay();
	coordinateMin->SetValue(xmin, ymin, 0);
	double* worldMin = coordinateMin->GetComputedWorldValue(Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
	return new double[4]{ worldMin[0], worldMax[0], worldMin[1], worldMax[1] };
}

void InteractorStyle::followToLine(double* mousePosition) {
	double* borders = GetViewportBorder();

	double xmin = borders[0];
	double xmax = borders[1];
	double ymin = borders[2];
	double ymax = borders[3];

	double x = mousePosition[0];
	double y = mousePosition[1];

	double xCam = camera->GetPosition()[0];
	double yCam = camera->GetPosition()[1];
	double zCam = camera->GetPosition()[2];

	if (abs(xmin - x) <  2.0 && x >  xmin) {
		camera->SetPosition(xCam - 0.1, yCam, zCam);
	}
	else if (abs(xmax - x) < 2.0 && x < xmax) {
		camera->SetPosition(xCam + 0.1, yCam, zCam);
	}
	else if (abs(ymin - y) < 2.0 && y > ymin) {
		camera->SetPosition(xCam, yCam - 0.1, zCam);
	}
	else if (abs(ymax - y) < 2.0 && y < ymax) {
		camera->SetPosition(xCam, yCam + 0.1, zCam);
	}

}