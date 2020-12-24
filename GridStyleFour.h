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
#include "Grid.h"


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
		vtkSmartPointer<vtkCamera> camera,
		vtkSmartPointer<vtkRenderer> renderer);

private:
	virtual void OnLeftButtonDown();
	virtual void OnLeftButtonUp();
	virtual void OnMouseWheelBackward();
	virtual void OnMouseWheelForward();
	virtual void OnTimer();
	virtual void OnMouseMove();

	void rebuildMarker(double* coordinate);

	bool isPresentInOne(Grid* grid);
	bool isPair(Grid* gridA, Grid* gridB);
	void moveTwoActorsByX(Grid* gridA, Grid* gridB, double d);
	void moveTwoActorsByY(Grid* gridA, Grid* gridB, double d);
	bool isPresentInPair(Grid* gridA, Grid* gridB);


	void rebuildGird();
	
	int getCurrentState(Grid* gridA, Grid* gridB, Grid* gridC, Grid* gridD);

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
	int countBack = 1, countForw = 1;
	double grid_CellX = 0.05;
	double grid_CellY = 0.05;
	double viewportSize[2];
	int cellScreenWidth_ = 0;
	double worldToScreenCoeff_ = 0;
	double  lastCameraScale_ = 0;
	bool isPaneOnly_ = false;
	double zPosition;
	double coordinate_[2];

	vtkSmartPointer<vtkCamera> camera_;
	vtkSmartPointer<vtkActor> axesMain_;
	vtkSmartPointer<vtkRenderer> renderer_;
	vtkSmartPointer<vtkActor> actorMarker_;
	Grid* gridA_;
	Grid* gridB_;
	Grid* gridC_;
	Grid* gridD_;


};

