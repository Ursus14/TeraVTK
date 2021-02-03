#include "Grid.h"

Grid::Grid()
{
	position = new double[3]{ 0.0, 0.0, 0.0 };
	size = new double[2]{ 10.0, 10.0 };
	cell = new double[2]{ 1.0, 1.0 };
	actor = vtkSmartPointer<vtkActor>::New();
	BuildActor();
	actor->SetPosition(position);
}

Grid::Grid(double* _position, double* _size, double* _cell)
{
	position = new double[3]{ _position[0], _position[1], 0.0 };
	size = new double[2]{ _size[0], _size[1] };
	cell = new double[2]{ _cell[0], _cell[1] };
	actor = vtkSmartPointer<vtkActor>::New();
	BuildActor();
	actor->SetPosition(position);
}

void Grid::SetPosition(double* newpos)
{
	position[0] = newpos[0];
	position[1] = newpos[1];
	actor->SetPosition(position);
}

void Grid::SetPosition(double newX, double newY, double newZ)
{
	position[0] = newX;
	position[1] = newY;
	position[2] = newZ;
	actor->SetPosition(position);
}

void Grid::SetSize(double* newsize, bool rebuild)
{
	size[0] = newsize[0];
	size[1] = newsize[1];
	if (rebuild)
		BuildActor();
}

void Grid::SetCell(double cellX, double cellY, bool rebuild)
{
	cell[0] = cellX;
	cell[1] = cellY;
	if (rebuild)
		BuildActor();
}

double* Grid::GetPosition()
{
	double* getpos = new double[2]{ position[0],position[1] };
	return getpos;
}

double* Grid::GetSize()
{
	double* getsize = new double[2]{ size[0],size[1] };
	return getsize;
}

double* Grid::GetCell()
{
	double* getcell = new double[2]{ cell[0],cell[1] };
	return getcell;
}

vtkSmartPointer<vtkActor> Grid::GetActor()
{
	return actor;
}

void Grid::BuildActor()
{
	vtkSmartPointer<vtkPoints> points = vtkPoints::New();
	vtkSmartPointer<vtkCellArray> lines = vtkCellArray::New();

	int num = 0;
	for (double i = -size[1]/2.0; i <= size[1]/2.0; i+=cell[1]) {
		points->InsertNextPoint(-size[0]/2, i , 0);
		points->InsertNextPoint(size[0]/2, i , 0);

		vtkIdType tcell[] = { 2 * num, 2 * num + 1 };
		lines->InsertNextCell(2, tcell);
		num++;
	}

	for (double i = -size[0]/2.0; i <= size[0]/2.0; i+=cell[0]) {
		points->InsertNextPoint(i, -size[1]/2, 0);
		points->InsertNextPoint(i , size[1]/2, 0);

		vtkIdType tcell[] = { 2 * num, 2 * num + 1 };
		lines->InsertNextCell(2, tcell);
		num++;
	}

	vtkSmartPointer<vtkPolyData> polydata = vtkPolyData::New();
	polydata->SetPoints(points);
	polydata->SetLines(lines);

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkPolyDataMapper::New();
	mapper->SetInputData(polydata);
	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(0.8, 0.8, 0.8);
}
