#include "DoubleClickMouse.h"

DoubleClickMouse::DoubleClickMouse() {
	PreviousPosition = new int[2]{ 0, 0 };
	CurrentPosition = new int[2]{ 0, 0 };
};

void DoubleClickMouse::SetPreviousPosition(int* position) {
	this->PreviousPosition = position;
}

void DoubleClickMouse::SetCurrentPosition(int* position) {
	this->CurrentPosition = position;
}

int* DoubleClickMouse::GetCurrentPosition() {
	return this->CurrentPosition;
}

int* DoubleClickMouse::GetPreviousPosition() {
	return this->PreviousPosition;
}


bool DoubleClickMouse::isDoubleClick(int* pickPosition, std::chrono::system_clock::time_point start, std::chrono::system_clock::time_point end) {
	int xdist = pickPosition[0] - this->PreviousPosition[0];
	int ydist = pickPosition[1] - this->PreviousPosition[1];

	std::chrono::duration<double> elapsed_seconds = end - start;
	int moveDistance = xdist * xdist + ydist * ydist;
	bool flag = false;
	if (moveDistance < (this->resetPixelDistance)^2 && (abs(elapsed_seconds.count()) < doubleClickTimeLimit))
	{
		flag = true;
	}
	return flag;
}
