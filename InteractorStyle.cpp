#include "InteractorStyle.h"
vtkStandardNewMacro(InteractorStyle);

InteractorStyle::InteractorStyle(){
}

InteractorStyle::InteractorStyle(PlaneGrid* _plane, MainAxes* _axes, Point* _marker, std::vector<Point> _drawPoints, vtkSmartPointer<vtkRenderer> _renderer){
	plane = _plane;
	renderer = _renderer;
	axes = _axes;
	marker = _marker;
	drawPoints = _drawPoints;
}


void InteractorStyle::OnMouseWheelBackward(){
	vtkInteractorStyleTrackballCamera::OnMouseWheelBackward();
	plane->RebuildPlane(renderer->GetActiveCamera(), Interactor->GetRenderWindow()->GetSize());
	axes->RebuildAxes(renderer->GetActiveCamera(), Interactor->GetRenderWindow()->GetSize());
}


void InteractorStyle::OnMouseWheelForward(){
	vtkInteractorStyleTrackballCamera::OnMouseWheelForward();
	plane->RebuildPlane(renderer->GetActiveCamera(), Interactor->GetRenderWindow()->GetSize());
	axes->RebuildAxes(renderer->GetActiveCamera(), Interactor->GetRenderWindow()->GetSize());
}

void InteractorStyle::OnRightButtonDown(){
	vtkInteractorStyleTrackballCamera::OnMiddleButtonDown();
}

void InteractorStyle::OnRightButtonUp(){
	vtkInteractorStyleTrackballCamera::OnMiddleButtonUp();
}
void InteractorStyle::OnLeftButtonDown() {
	UserEvents::OnLeftButtonDown();
	prevPosition[0] = GetCurrentMousePosition()[0];
	prevPosition[1] = GetCurrentMousePosition()[1];
	line_->SetBeginPosition(prevPosition);
	line_->SetEndPosition(prevPosition);
	isAddLine = true;
	line_->build(prevPosition, renderer, 0.025 * plane->GetCell()[0]);
	Interactor->GetRenderWindow()->Render();
}

void InteractorStyle::OnLeftButtonUp(){
	UserEvents::OnLeftButtonUp();

	line_->rebuild(GetCurrentMousePosition(), renderer);

	if (line_->GetLengthSquare() == 0) {
		line_->Remove(renderer);
	}
	else {
		for (auto line : lines_) {
			if (lines->intersect(line, line_)) {
				Point* point_ = new Point(lines->getIntersectionPoint(line, line_), 0.025 * plane->GetCell()[0]);
				renderer->AddActor(point_->GetActor());
			}
		}
		lines_.push_back(line_);
		Interactor->GetRenderWindow()->Render();
		line_ = new Line(vtkSmartPointer<vtkActor>::New());
	}
	isAddLine = false;
} 
void InteractorStyle::OnMouseMove(){
	vtkInteractorStyleTrackballCamera::OnMouseMove();
	plane->HitTestingAtBorder(renderer->GetActiveCamera());
	axes->RebuildAxes(renderer->GetActiveCamera(), Interactor->GetRenderWindow()->GetSize());

	double coordinate[3];

	coordinate[0] = GetCurrentMousePosition()[0];
	coordinate[1] = GetCurrentMousePosition()[1];
	coordinate[2] = 0.0;

	if (isAddLine) {
		line_->rebuild(coordinate, renderer);
	}
	//calculating the new position for the marker
	int* sizeWin = Interactor->GetRenderWindow()->GetSize();
	marker->SetPosition(GetCurrentMousePosition());
	marker->VisibilityOn();
	renderer->RemoveActor(marker->GetActor());
	renderer->AddActor(marker->GetActor());
	Interactor->GetRenderWindow()->Render();

}

void InteractorStyle::OnLeave(){
	vtkInteractorStyleTrackballCamera::OnLeave();
	marker->VisibilityOff();
	Interactor->GetRenderWindow()->Render();
}

void InteractorStyle::OnLeftDoubleClick(){
	UserEvents::OnLeftDoubleClick();
	drawPoints.push_back(Point(GetCurrentMousePosition(), 0.025 * plane->GetCell()[0]));
	drawPoints[drawPoints.size() - 1].SetColor(0, 0, 0);
	renderer->AddActor(drawPoints[drawPoints.size() - 1].GetActor());
}

void InteractorStyle::Scrolling(){
	if (isAddLine) {
		int* mousePos = Interactor->GetEventPosition();
		int* sizeWin = Interactor->GetRenderWindow()->GetSize();
		int delta = 20;
		double mult = renderer->GetActiveCamera()->GetParallelScale() / 8;

		if (mousePos[0] < delta) {
			mult = renderer->GetActiveCamera()->GetParallelScale() * abs(atan((mousePos[0] - delta) / 100.0)) / 12.0;
			renderer->GetActiveCamera()->SetPosition(renderer->GetActiveCamera()->GetPosition()[0] - mult, renderer->GetActiveCamera()->GetPosition()[1], renderer->GetActiveCamera()->GetPosition()[2]);
		}
		if (mousePos[0] > sizeWin[0] - delta) {
			mult = renderer->GetActiveCamera()->GetParallelScale() * abs(atan((mousePos[0] - (sizeWin[0] - delta)) / 100.0)) / 12.0;
			renderer->GetActiveCamera()->SetPosition(renderer->GetActiveCamera()->GetPosition()[0] + mult, renderer->GetActiveCamera()->GetPosition()[1], renderer->GetActiveCamera()->GetPosition()[2]);
		}
		if (mousePos[1] < delta) {
			mult = renderer->GetActiveCamera()->GetParallelScale() * abs(atan((mousePos[1] - delta) / 100.0)) / 12.0;
			renderer->GetActiveCamera()->SetPosition(renderer->GetActiveCamera()->GetPosition()[0], renderer->GetActiveCamera()->GetPosition()[1] - mult, renderer->GetActiveCamera()->GetPosition()[2]);
		}
		if (mousePos[1] > sizeWin[1] - delta) {
			mult = renderer->GetActiveCamera()->GetParallelScale() * abs(atan((mousePos[1] - (sizeWin[1] - delta)) / 100.0)) / 12.0;
			renderer->GetActiveCamera()->SetPosition(renderer->GetActiveCamera()->GetPosition()[0], renderer->GetActiveCamera()->GetPosition()[1] + mult, renderer->GetActiveCamera()->GetPosition()[2]);
		}
		OnMouseMove();
	}
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

double* InteractorStyle::GetViewportBorder(){
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