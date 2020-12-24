#include "Grid.h"


Grid::Grid(double x, double y) {
	x_ = x;
	y_ = y;

	init();
}


int Grid::size() {
	return grid.size();
}

vtkSmartPointer<vtkActor> Grid::doActor(double s_x, double s_y, double x, double y, double d_x, double d_y) {
	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();

	double p0[3] = { x,  y, 0.0 };
	double p1[3] = { x + s_x, y + s_y, 0.0 };

	vtkSmartPointer<vtkLineSource> lineSource =
		vtkSmartPointer<vtkLineSource>::New();
	lineSource->SetPoint1(p0);
	lineSource->SetPoint2(p1);

	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(lineSource->GetOutputPort());

	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(0.824, 0.824, 0.824);


	actor->SetPosition(d_x, d_y, 0);
	return actor;
}

void Grid::doGrid(double d_x, double d_y) {

	for (double i = 0; i < 0.99; i += 0.01)
	{
		grid.push_back(doActor(0.0, 1.0, i, 0.0, d_x, d_y));
		grid.push_back(doActor(1.0, 0.0, 0.0, i, d_x, d_y));

	}
	
}

void Grid::init() {
	doGrid(x_, y_);
}

double* Grid::GetPosition() {
	return new double[2] {grid[0]->GetPosition()[0], grid[1]->GetPosition()[1]};
}

void Grid::rebuild() {

	/*for (int i = 0; i < grid.size(); i++) {
		vtkPolyDataMapper::SafeDownCast(grid[i]->GetMapper())->SetInputData(polydata);
	}*/
}

void Grid::SetPosition(double x, double y) {
	//cout << "change position" << endl;
	for (int i = 0; i < grid.size(); i++)
	{
		grid[i]->SetPosition(x, y, 0);
	}
}

vtkSmartPointer<vtkActor> Grid::getActorByIndex(int i) {
	return grid[i];
}