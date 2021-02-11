#include "PlaneGrid.h"

PlaneGrid::PlaneGrid() {
	build(new double[2]{ 1.0, 1.0 }, new int[2]{ 1,1 }, 3.0);
}

PlaneGrid::PlaneGrid(double* cell, int* sizewin, double parallelScale) {
	build(cell, sizewin, parallelScale);
}


void PlaneGrid::build(double* cell, int* sizewin, double parallelScale) {
	double* size = new double[2]{ 4.0 * parallelScale, 4.0 * parallelScale };


	// in case the window is not square
	if (sizewin[0] / sizewin[1] > 1) {
		size[0] *= (sizewin[0] * 1.0) / sizewin[1];
	}
	else {
		size[1] *= (sizewin[1] * 1.0) / sizewin[0];
	}

	// checking the conditions for the grid size
	for (int i = 0; i < 2; i++) {
		if (fmod(size[i], cell[i]) != 0) {
			size[i] = floor(size[i] / cell[i]) * cell[i] + cell[i];
		}

		if (fmod(size[i] / cell[i], 2.0) != 0) {
			size[i] += cell[i];
		}
	}
	double* pos = new double[3]{ -size[0] / 2.0 ,size[1] / 2.0, 0 };

	pGrids[0] = Grid(pos, size, cell);
	pos[0] = size[0] / 2.0;
	pGrids[1] = Grid(pos, size, cell);
	pos[1] = -size[1] / 2.0;
	pGrids[2] = Grid(pos, size, cell);
	pos[0] = -size[0] / 2.0;
	pGrids[3] = Grid(pos, size, cell);

	RecountBorderline();
}

void PlaneGrid::RecountBorderline() {
	borderline[0] = pGrids[sequence[0]].GetPosition()[0];
	borderline[1] = pGrids[sequence[1]].GetPosition()[1];
	borderline[2] = pGrids[sequence[2]].GetPosition()[0];
	borderline[3] = pGrids[sequence[3]].GetPosition()[1];
}

void PlaneGrid::RebuildPlane(vtkCamera* camera, int* sizewin) {
	double zoom = camera->GetParallelScale();
	double* newsize = new double[2]{ zoom * 4.0, zoom * 4.0 };

	// in case the window is not square
	if (sizewin[0] / sizewin[1] > 1) {
		newsize[0] *= (sizewin[0] * 1.0) / sizewin[1];
	}
	else {
		newsize[1] *= (sizewin[1] * 1.0) / sizewin[0];
	}
	// checking the conditions for the grid size
	for (int i = 0; i < 2; i++) {
		if (fmod(newsize[i], pGrids->GetCell()[i]) != 0) {
			newsize[i] = floor(newsize[i] / pGrids->GetCell()[i]) * pGrids->GetCell()[i] + pGrids->GetCell()[i];
		}
		if (fmod(newsize[i] / pGrids->GetCell()[i], 2.0) != 0) {
			newsize[i] += pGrids->GetCell()[i];
		}
	}
	for (int i = 0; i < 4; i++) {
		pGrids[i].SetSize(newsize);
	}
	pGrids[sequence[0]].SetPosition(floor(camera->GetPosition()[0] / pGrids->GetCell()[0]) * pGrids->GetCell()[0] - newsize[0] / 2.0,
									floor(camera->GetPosition()[1] / pGrids->GetCell()[1]) * pGrids->GetCell()[1] + newsize[1] / 2.0);

	pGrids[sequence[1]].SetPosition(floor(camera->GetPosition()[0] / pGrids->GetCell()[0]) * pGrids->GetCell()[0] + newsize[0] / 2.0,
									floor(camera->GetPosition()[1] / pGrids->GetCell()[1]) * pGrids->GetCell()[1] + newsize[1] / 2.0);

	pGrids[sequence[2]].SetPosition(floor(camera->GetPosition()[0] / pGrids->GetCell()[0]) * pGrids->GetCell()[0] + newsize[0] / 2.0,
									floor(camera->GetPosition()[1] / pGrids->GetCell()[1]) * pGrids->GetCell()[1] - newsize[1] / 2.0);

	pGrids[sequence[3]].SetPosition(floor(camera->GetPosition()[0] / pGrids->GetCell()[0]) * pGrids->GetCell()[0] - newsize[0] / 2.0,
									floor(camera->GetPosition()[1] / pGrids->GetCell()[1]) * pGrids->GetCell()[1] - newsize[1] / 2.0);

	RecountBorderline();
}

void PlaneGrid::BorderHitCheck(vtkCamera* camera) {
	int borderNumber = -1;

	if (camera->GetPosition()[0] < borderline[0]) {
		borderNumber = 0;
	}

	if (camera->GetPosition()[0] > borderline[2]) {
		borderNumber = 2;
	}

	if (camera->GetPosition()[1] < borderline[3]) {
		borderNumber = 3;
	}

	if (camera->GetPosition()[1] > borderline[1]) {
		borderNumber = 1;
	}

	if (borderNumber != -1) {
		FlippingGrids(borderNumber);
	}
}


void PlaneGrid::SetCell(double cellX, double cellY) {
	for (int i = 0; i < 4; i++) {
		pGrids[i].SetCell(cellX, cellY);
	}
}

double* PlaneGrid::GetCell() {
	return pGrids[0].GetCell();
}

void PlaneGrid::FlippingGrids(int borderNumber) {
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

vtkSmartPointer<vtkActor> PlaneGrid::GetActor(int idx) {
	return pGrids[idx].GetActor();
}