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
#include <chrono>
#include <ctime>

#include "Grid.h"
#include <vtkUnstructuredGrid.h>


class GridStyleFour : public vtkInteractorStyleTrackballCamera
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

	void buildLine(double* coordinate, vtkSmartPointer<vtkActor> lineActor);
	void buildBrokenLine();
	void buildPoint(double* coordinate);
	void moveGrid();
	bool isDoubleClickedL(int* pickPosition);
	bool isDoubleClickedR(int* pickPosition);

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
	int countBack = 1, countForw = 1;
	double grid_CellX = 0.05;
	double grid_CellY = 0.05;
	double viewportSize[2];
	int cellScreenWidth_ = 0;
	double worldToScreenCoeff_ = 0;
	double  lastCameraScale_ = 0;
	bool isPaneOnly_ = false;
	double zPosition;
	int countOfPoints_;

	// Double click -----
	bool isAddLine;
	double* prevPosition;
	int PreviousPositionL[2];
	int PreviousPositionR[2];
	int ResetPixelDistance;
	std::chrono::system_clock::time_point startL_ = std::chrono::system_clock::now();
	std::chrono::system_clock::time_point endL_ = std::chrono::system_clock::now();


	std::chrono::system_clock::time_point startR_ = std::chrono::system_clock::now();
	std::chrono::system_clock::time_point endR_ = std::chrono::system_clock::now();

	vtkSmartPointer<vtkPoints> points_;

	// ------------------

	vtkSmartPointer<vtkCamera> camera_;
	// ------------------

	std::vector<double> pointsOfLines_;

	// ------------------

	
	vtkSmartPointer<vtkActor> axesMain_;
	vtkSmartPointer<vtkActor> lineActor_ = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkRenderer> renderer_;
	vtkSmartPointer<vtkActor> actorMarker_;
	//--
	Grid* gridA_;
	Grid* gridB_;
	Grid* gridC_;
	Grid* gridD_;


};

