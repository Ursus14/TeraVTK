vtkStandardNewMacro(GridInteractorStyle);

GridInteractorStyle::GridInteractorStyle()
{
}

GridInteractorStyle::GridInteractorStyle(PlaneGrid* _plane, MainAxes* _axes, Point* _marker, std::vector<Point> _drawPoints, vtkSmartPointer<vtkCamera> _camera, vtkSmartPointer<vtkRenderer> _renderer)
{
	plane = _plane;
	camera = _camera;
	renderer = _renderer;
	axes = _axes;
	marker = _marker;
	drawPoints = _drawPoints;
}

void GridInteractorStyle::OnMouseWheelBackward()
{
	vtkInteractorStyleTrackballCamera::OnMouseWheelBackward();
	plane->RebuildPlane(camera, Interactor->GetRenderWindow()->GetSize());
	axes->RebuildAxes(camera, Interactor->GetRenderWindow()->GetSize());
}

void GridInteractorStyle::OnMouseWheelForward()
{
	vtkInteractorStyleTrackballCamera::OnMouseWheelForward();
	plane->RebuildPlane(camera, Interactor->GetRenderWindow()->GetSize());
	axes->RebuildAxes(camera, Interactor->GetRenderWindow()->GetSize());
}

void GridInteractorStyle::OnRightButtonDown()
{
	vtkInteractorStyleTrackballCamera::OnMiddleButtonDown();
}

void GridInteractorStyle::OnRightButtonUp()
{
	vtkInteractorStyleTrackballCamera::OnMiddleButtonUp();
}
void GridInteractorStyle::OnLeftButtonDown()
{
	InteractorDoubleClick::OnLeftButtonDown();

		prevPosition[0] = GetCurrentMousePosition()[0];
		prevPosition[1] = GetCurrentMousePosition()[1];
		line_->SetBeginPosition(prevPosition);
		line_->build(GetCurrentMousePosition(), renderer);
		isAddLine = true;
		Point* point = new Point(GetCurrentMousePosition());
		renderer->AddActor(point->GetActor());

	
	Interactor->GetRenderWindow()->Render();
}

void GridInteractorStyle::OnLeftButtonUp()
{
		line_->SetBeginPosition(prevPosition);
		line_->build(GetCurrentMousePosition(), renderer);
		Point* point = new Point(GetCurrentMousePosition());
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

void GridInteractorStyle::OnMouseMove()
{
	vtkInteractorStyleTrackballCamera::OnMouseMove();
	plane->HitTestingAtBorder(camera);
	axes->RebuildAxes(camera, Interactor->GetRenderWindow()->GetSize());
	if (isAddLine) {
		line_->rebuild(GetCurrentMousePosition(), renderer);
	}
	//calculating the new position for the marker
	int* sizeWin = Interactor->GetRenderWindow()->GetSize();
	marker->SetPosition(GetCurrentMousePosition());
	marker->VisibilityOn();
	renderer->RemoveActor(marker->GetActor());
	renderer->AddActor(marker->GetActor());
	Interactor->GetRenderWindow()->Render();

}

void GridInteractorStyle::OnLeave()
{
	vtkInteractorStyleTrackballCamera::OnLeave();
	marker->VisibilityOff();
	Interactor->GetRenderWindow()->Render();
}

void GridInteractorStyle::OnLeftDoubleClick()
{
	InteractorDoubleClick::OnLeftDoubleClick();
	drawPoints.push_back(Point(GetCurrentMousePosition(),0.05));
	drawPoints[drawPoints.size() - 1].SetColor(0,0,0);
	renderer->AddActor(drawPoints[drawPoints.size()-1].GetActor());
	
	Interactor->GetRenderWindow()->Render();
}

double* GridInteractorStyle::GetCurrentMousePosition() {
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
