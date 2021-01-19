#include <chrono>
#include <ctime>
#include <Windows.h>

class DoubleClickMouse {
public:
	DoubleClickMouse();

	void SetPreviousPosition(int* position);
	void SetCurrentPosition(int* position);

	int* GetPreviousPosition();
	int* GetCurrentPosition();

	bool isDoubleClick(int* pickPosition, std::chrono::system_clock::time_point start, std::chrono::system_clock::time_point end);


private:
	double* currentPosition;
	double* previousPosition;

	int* PreviousPosition;
	int* CurrentPosition;

	int resetPixelDistance = 3; 
	float doubleClickTimeLimit = 0.25f;

};

