#include "Figures.h"

Figures::Figures()
{
}


void Figures::buildLine(double* coordinateB, double* coordinateE, vtkSmartPointer<vtkActor> lineActor, vtkSmartPointer<vtkRenderer> renderer) {

	vtkSmartPointer<vtkLineSource> lineSource =
		vtkSmartPointer<vtkLineSource>::New();

	lineSource->SetPoint1(coordinateB[0], coordinateB[1], 0.0);
	lineSource->SetPoint2(coordinateE[0], coordinateE[1], 0.0);

	cout << " First point:     " << coordinateB[0] << "    " << coordinateB[1] << endl;
	cout << " Second point:     " << coordinateE[0] << "    " << coordinateE[1] << endl;

	vtkSmartPointer<vtkPolyDataMapper> lineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();

	lineMapper->SetInputConnection(lineSource->GetOutputPort());

	lineActor->SetMapper(lineMapper);
	lineActor->GetProperty()->SetColor(0.0, 0.0, 0.0);


	renderer->AddActor(lineActor);

}

void Figures::buildBrokenLine(vtkPoints* points, vtkSmartPointer<vtkRenderer> renderer) {
	vtkSmartPointer<vtkLineSource> lineSource =
		vtkSmartPointer<vtkLineSource>::New();

	lineSource->SetPoints(points);

	vtkSmartPointer<vtkPolyDataMapper> brokenLineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();

	brokenLineMapper->SetInputConnection(lineSource->GetOutputPort());

	vtkSmartPointer<vtkActor> brokenLineActor = vtkSmartPointer<vtkActor>::New();
	brokenLineActor->SetMapper(brokenLineMapper);

	renderer->AddActor(brokenLineActor);
}

void Figures::buildPoint(double* coordinate, vtkSmartPointer<vtkRenderer> renderer) {
	vtkSmartPointer<vtkSphereSource> point = vtkSmartPointer<vtkSphereSource>::New();
	point->SetThetaResolution(100);
	point->SetPhiResolution(50);
	point->SetRadius(0.001);
	vtkSmartPointer<vtkPolyDataMapper> pointMapper = vtkSmartPointer<vtkPolyDataMapper>::New();

	pointMapper->SetInputConnection(point->GetOutputPort());

	vtkSmartPointer<vtkActor> pointActor = vtkSmartPointer<vtkActor>::New();
	pointActor->SetMapper(pointMapper);

	pointActor->GetProperty()->SetColor(0.0, 0.0, 0.0);
	pointActor->SetPosition(coordinate[0], coordinate[1], 0.0);


	renderer->AddActor(pointActor);
}