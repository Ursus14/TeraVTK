#include "MainAxes.h"

MainAxes::MainAxes()
{
	actor = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkPoints> points = vtkPoints::New();
	points->Allocate(4);
	points->InsertNextPoint(0, -12, 0);
	points->InsertNextPoint(0, 12, 0);
	points->InsertNextPoint(-12, 0, 0);
	points->InsertNextPoint(12, 0, 0);

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
	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(0, 0, 0);
}

MainAxes::MainAxes(int* sizewin, double parallelScale)
{
	actor = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkPoints> points = vtkPoints::New();
	points->Allocate(4);

	double* scale = new double[2]{ 4 * parallelScale ,4 * parallelScale };
	if (sizewin[0] / sizewin[1] > 1)
		scale[0] *= (sizewin[0] * 1.0) / sizewin[1];
	else
		scale[1] *= (sizewin[1] * 1.0) / sizewin[0];

	points->InsertNextPoint(0, -scale[1], 0);
	points->InsertNextPoint(0, scale[1], 0);
	points->InsertNextPoint(-scale[0], 0, 0);
	points->InsertNextPoint(scale[0], 0, 0);

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
	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(0, 0, 0);

}

void MainAxes::RebuildAxes(vtkSmartPointer<vtkCamera> camera, int* sizewin)
{

	auto scaleX = (abs(camera->GetPosition()[0]) + 4.0 * camera->GetParallelScale());
	auto scaleY = (abs(camera->GetPosition()[1]) + 4.0 * camera->GetParallelScale());

	if (sizewin[0] / sizewin[1] > 1)
		scaleX *= (sizewin[0] * 1.0) / sizewin[1];
	else
		scaleY *= (sizewin[1] * 1.0) / sizewin[0];

	double scale[3] = { scaleX, scaleY, 0 };
	actor->SetScale(scale);
}

vtkSmartPointer<vtkActor> MainAxes::GetActor()
{
	return actor;
}
