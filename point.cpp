#include "point.h"

point::point()
{
}

point::point(double* pos, vtkSmartPointer<vtkRenderer> renderer)
{
	pos_[0] = pos[0];
	pos_[1] = pos[1];
	renderer_ = renderer;

}

void point::draw()
{
	actorPoint_ = vtkSmartPointer<vtkActor>::New();

	vtkSmartPointer<vtkRegularPolygonSource> point = vtkSmartPointer<vtkRegularPolygonSource>::New();
	point->SetNumberOfSides(50);
	point->SetRadius(0.005);
	point->SetCenter(pos_[0], pos_[1], 0);


	vtkSmartPointer<vtkPolyDataMapper> mapperPoint_ = vtkPolyDataMapper::New();
	mapperPoint_->SetInputConnection(point->GetOutputPort());
	actorPoint_->SetMapper(mapperPoint_);
	actorPoint_->GetProperty()->SetColor(158/255.0, 77/255.0, 30/255.0);
	actorPoint_->SetVisibility(1);
	renderer_->AddActor(actorPoint_);
}
