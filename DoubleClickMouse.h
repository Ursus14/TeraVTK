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

	int* PreviousPosition;
	int* CurrentPosition;

	int resetPixelDistance = 5; 
	double doubleClickTimeLimit = 0.5;

};

