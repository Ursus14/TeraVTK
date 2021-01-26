#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkCamera.h>
#include <vtkPropPicker.h>
#include <vtkRenderer.h>
#include <vtkSetGet.h>
#include <vtkObjectFactory.h>
#include <vtkCoordinate.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkActor.h>
#include <vtkPointPicker.h>
#include <vtkRendererCollection.h>
#include <vtkRegularPolygonSource.h>
#include <vtkPointPicker.h>
#include <vtkDataSetMapper.h>
#include <vtkSphereSource.h>
#include <vtkLineSource.h>
#include <vtkRegularPolygonSource.h>
#include <chrono>
#include <ctime>

#include "Grid.h"
#include "Point.h"
#include "Lines.h"
#include "BrokenLine.h"
#include "DoubleClickMouse.h"


class GridStyleFour : public vtkInteractorStyleTrackballCamera, DoubleClickMouse
{
public:
	static GridStyleFour* New();
	vtkTypeMacro(GridStyleFour, vtkInteractorStyleTrackballCamera);
	GridStyleFour();
	GridStyleFour(
		Grid* gridA,
		Grid* gridB,
		Grid* gridC,
		Grid* gridD,
		vtkSmartPointer<vtkActor> axesMain,
		vtkSmartPointer<vtkActor> actorMarker,
		vtkSmartPointer<vtkRenderer> renderer);

private:
	virtual void OnLeftButtonDown();
	virtual void OnLeftButtonUp();
	virtual void OnRightButtonDown();
	virtual void OnRightButtonUp();
	virtual void OnMouseWheelBackward();
	virtual void OnMouseWheelForward();
	virtual void OnTimer();
	virtual void OnMouseMove();
	virtual void OnLeave();

	void rebuildMarker(double* coordinate);




	bool isPresentInOne(Grid* grid);
	bool isPair(Grid* gridA, Grid* gridB);
	void moveTwoGridByX(Grid* gridA, Grid* gridB, double d);
	void moveTwoGridByY(Grid* gridA, Grid* gridB, double d);
	bool isPresentInPair(Grid* gridA, Grid* gridB);



	void rebuildGrid();
	void moveGrid();

	int getCurrentState(Grid* gridA, Grid* gridB, Grid* gridC, Grid* gridD);
	double* getCurrentMousePosition();

	//(+x, +y)
	void moveToNorthEast(Grid* gridA, Grid* gridB, Grid* gridC);
	//(-x, +y)
	void moveToNorthWest(Grid* gridA, Grid* gridB, Grid* gridC);
	//(-x, -y)
	void moveToSouthWest(Grid* gridA, Grid* gridB, Grid* gridC);
	//(+x, -y)
	void moveToSouthEast(Grid* gridA, Grid* gridB, Grid* gridC);


private:
	bool flg = false;
	int numberOfDoubleClicks = 0;
	int countOfLines = 0;
	int countBack = 1, countForw = 1;

	
	double viewportSize[2];
	int cellScreenWidth_ = 0;
	double worldToScreenCoeff_ = 0;
	double  lastCameraScale_ = 0;
	double zPosition;
	int countOfPoints_;

	// Double click -----
	bool isAddLine = false;
	double prevPosition[2]{ 0.0, 0.0 };
	double currPosition[2]{ 0.0, 0.0 };
	std::vector<Line*> lines_;
	int PreviousPositionL[2];
	int PreviousPositionR[2];
	int ResetPixelDistance;
	std::chrono::system_clock::time_point startL_ = std::chrono::system_clock::now();
	std::chrono::system_clock::time_point endL_ = std::chrono::system_clock::now();


	std::chrono::system_clock::time_point startR_ = std::chrono::system_clock::now();
	std::chrono::system_clock::time_point endR_ = std::chrono::system_clock::now();

	vtkSmartPointer<vtkPoints> points_ = vtkSmartPointer<vtkPoints>::New();

	// ------------------

	// ------------------
	Line* line_ = new Line(vtkSmartPointer<vtkActor>::New());
	Lines* lines = new Lines();
	BrokenLine* brokenLine_ = new BrokenLine();
	// ------------------

	vtkSmartPointer<vtkCamera> camera_;
	// ------------------

	std::vector<double> pointsOfLines_;

	// ------------------

	
	vtkSmartPointer<vtkActor> axesMain_;
	vtkSmartPointer<vtkActor> lineActor_;
	vtkSmartPointer<vtkRenderer> renderer_;
	vtkSmartPointer<vtkActor> actorMarker_;

	//--
	Grid* gridA_;
	Grid* gridB_;
	Grid* gridC_;
	Grid* gridD_;


};

