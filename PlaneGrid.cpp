#include "PlaneGrid.h"

PlaneGrid::PlaneGrid() {
	double* pos = new double[3]{ -6.0, 6.0, 0.0 };
	double* cell = new double[2]{ 1.0, 1.0 };
	double* size = new double[2]{ 12.0, 12.0 };
	pGrids[0] =  Grid(pos, size, cell);
	pos[0] = 6.0;
	pGrids[1] =  Grid(pos, size, cell);
	pos[1] = -6.0;
	pGrids[2] =  Grid(pos, size, cell);
	pos[0] = -6.0;
	pGrids[3] =  Grid(pos, size, cell);

	borderline = new double[4]{ pGrids[0].GetPosition()[0],pGrids[1].GetPosition()[1] ,pGrids[2].GetPosition()[0] , pGrids[3].GetPosition()[1]};
}


void PlaneGrid::RecountBorderline() {
	borderline[0] = pGrids[sequence[0]].GetPosition()[0];
	borderline[1] = pGrids[sequence[1]].GetPosition()[1];
	borderline[2] = pGrids[sequence[2]].GetPosition()[0];
	borderline[3] = pGrids[sequence[3]].GetPosition()[1];
}

void PlaneGrid::RebuildPlane(vtkCamera* camera, int* sizewin) {
	double zoom = camera->GetParallelScale();
	double* newsize = new double[2]{zoom*4.0, zoom*4.0};

	// in case the window is not square
	if (sizewin[0] / sizewin[1] > 1)
		newsize[0] *= (sizewin[0] * 1.0) / sizewin[1];
	else
		newsize[1] *= (sizewin[1] * 1.0) / sizewin[0];

	// checking the conditions for the grid size
	for (int i = 0; i < 2; i++) {
		if (fmod(newsize[i], pGrids->GetCell()[i]) != 0)
			newsize[i] = floor(newsize[i] / pGrids->GetCell()[i]) + pGrids->GetCell()[i];

		if (fmod(newsize[i] / pGrids->GetCell()[i], 2.0) != 0)
			newsize[i] += pGrids->GetCell()[i];
	}

	for (int i = 0; i < 4; i++) {
		pGrids[i].SetSize(newsize);
	}

	pGrids[sequence[0]].SetPosition(floor((camera->GetPosition()[0] - newsize[0] / 2.0) / pGrids->GetCell()[0]) * pGrids->GetCell()[0],
									floor((camera->GetPosition()[1] + newsize[1] / 2.0) / pGrids->GetCell()[1]) * pGrids->GetCell()[1]);
	pGrids[sequence[1]].SetPosition(floor((camera->GetPosition()[0] + newsize[0] / 2.0) / pGrids->GetCell()[0]) * pGrids->GetCell()[0],
									floor((camera->GetPosition()[1] + newsize[1] / 2.0) / pGrids->GetCell()[1]) * pGrids->GetCell()[1]);
	pGrids[sequence[2]].SetPosition(floor((camera->GetPosition()[0] + newsize[0] / 2.0) / pGrids->GetCell()[0]) * pGrids->GetCell()[0],
									floor((camera->GetPosition()[1] - newsize[1] / 2.0) / pGrids->GetCell()[1]) * pGrids->GetCell()[1]);
	pGrids[sequence[3]].SetPosition(floor((camera->GetPosition()[0] - newsize[0] / 2.0) / pGrids->GetCell()[0]) * pGrids->GetCell()[0],
									floor((camera->GetPosition()[1] - newsize[1] / 2.0) / pGrids->GetCell()[1]) * pGrids->GetCell()[1]);
	RecountBorderline();
}

void PlaneGrid::HitTestingAtBorder(vtkCamera* camera) {
	int borderNumber = -1;

	if (camera->GetPosition()[0] < borderline[0])
		borderNumber = 0;

	if (camera->GetPosition()[0] > borderline[2])
		borderNumber = 2;

	if (camera->GetPosition()[1] < borderline[3])
		borderNumber = 3;

	if (camera->GetPosition()[1] > borderline[1])
		borderNumber = 1;

	if (borderNumber != -1)
		FlippingGrids(borderNumber);

}

void PlaneGrid::SetCell(double cellX, double cellY)
{
	for (int i = 0; i < 4; i++) {
		pGrids[i].SetCell(cellX, cellY);
	}
}

double* PlaneGrid::GetCell()
{
	return pGrids[0].GetCell();
}

void PlaneGrid::FlippingGrids(int borderNumber)
{
	int temp = 0;
	if (borderNumber == 0) {
		pGrids[sequence[1]].SetPosition(pGrids[sequence[1]].GetPosition()[0] - 2 * pGrids[sequence[1]].GetSize()[0], pGrids[sequence[1]].GetPosition()[1]);
		pGrids[sequence[2]].SetPosition(pGrids[sequence[2]].GetPosition()[0] - 2 * pGrids[sequence[2]].GetSize()[0], pGrids[sequence[2]].GetPosition()[1]);
		
		temp = sequence[0];
		sequence[0] = sequence[1];
		sequence[1] = temp;

		temp = sequence[3];
		sequence[3] = sequence[2];
		sequence[2] = temp;
	}

	if (borderNumber == 1) {
		pGrids[sequence[2]].SetPosition(pGrids[sequence[2]].GetPosition()[0], pGrids[sequence[2]].GetPosition()[1] + 2 * pGrids[sequence[2]].GetSize()[1]);
		pGrids[sequence[3]].SetPosition(pGrids[sequence[3]].GetPosition()[0], pGrids[sequence[3]].GetPosition()[1] + 2 * pGrids[sequence[3]].GetSize()[1]);

		temp = sequence[0];
		sequence[0] = sequence[3];
		sequence[3] = temp;

		temp = sequence[1];
		sequence[1] = sequence[2];
		sequence[2] = temp;
	}

	if (borderNumber == 2) {
		pGrids[sequence[0]].SetPosition(pGrids[sequence[0]].GetPosition()[0] + 2 * pGrids[sequence[0]].GetSize()[0], pGrids[sequence[0]].GetPosition()[1]);
		pGrids[sequence[3]].SetPosition(pGrids[sequence[3]].GetPosition()[0] + 2 * pGrids[sequence[3]].GetSize()[0], pGrids[sequence[3]].GetPosition()[1]);

		temp = sequence[0];
		sequence[0] = sequence[1];
		sequence[1] = temp;

		temp = sequence[3];
		sequence[3] = sequence[2];
		sequence[2] = temp;
	}

	if (borderNumber == 3) {
		pGrids[sequence[0]].SetPosition(pGrids[sequence[0]].GetPosition()[0], pGrids[sequence[0]].GetPosition()[1] - 2 * pGrids[sequence[0]].GetSize()[1]);
		pGrids[sequence[1]].SetPosition(pGrids[sequence[1]].GetPosition()[0], pGrids[sequence[1]].GetPosition()[1] - 2 * pGrids[sequence[1]].GetSize()[1]);

		temp = sequence[0];
		sequence[0] = sequence[3];
		sequence[3] = temp;

		temp = sequence[1];
		sequence[1] = sequence[2];
		sequence[2] = temp;
	}
	RecountBorderline();

}

vtkSmartPointer<vtkActor> PlaneGrid::GetActor(int idx)
{
	return pGrids[idx].GetActor();
}


