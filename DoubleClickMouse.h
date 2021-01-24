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

	bool isDoubleClick(int* pickPosition, std::chrono::system_clock::time_point start, std::chrono::system_clock::time_point end, int countOfClicks);


private:

	int* PreviousPosition;
	int* CurrentPosition;

	int resetPixelDistance = 1; 
	double doubleClickTimeLimit = 0.2;

};

