#include "GridInteractorStyle.h"

vtkStandardNewMacro(GridInteractorStyle);

GridInteractorStyle::GridInteractorStyle()
{
}

GridInteractorStyle::GridInteractorStyle(PlaneGrid* _plane, MainAxes* _axes, vtkSmartPointer<vtkCamera> _camera, vtkSmartPointer<vtkRenderer> _renderer)
{
	plane = _plane;
	camera = _camera;
	renderer = _renderer;
	axes = _axes;
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

}

void GridInteractorStyle::OnMouseMove()
{
	vtkInteractorStyleTrackballCamera::OnMouseMove();
	plane->HitTestingAtBorder(camera);
	axes->RebuildAxes(camera, Interactor->GetRenderWindow()->GetSize());
}

