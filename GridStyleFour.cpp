#include "GridStyleFour.h"
#include <vtkRegularPolygonSource.h>

vtkStandardNewMacro(GridStyleFour);

GridStyleFour::GridStyleFour()
{
}

GridStyleFour::GridStyleFour(
	Grid* gridA,
	Grid* gridB,
	Grid* gridC,
	Grid* gridD,
	vtkSmartPointer<vtkActor> axesMain,
	vtkSmartPointer<vtkActor> actorMarker,
	vtkSmartPointer<vtkCamera> camera,
	vtkSmartPointer<vtkRenderer> renderer)
{
	gridA_ = gridA;
	gridB_ = gridB;
	gridC_ = gridC;
	gridD_ = gridD;

	axesMain_ = axesMain;
	actorMarker_ = actorMarker;
	camera_ = camera;
	renderer_ = renderer;
	
}


void GridStyleFour::OnMouseMove()
{
	double x = Interactor->GetEventPosition()[0];
	double y = Interactor->GetEventPosition()[1];
	

	vtkSmartPointer<vtkCoordinate> coordinate =
		vtkSmartPointer<vtkCoordinate>::New();
	coordinate->SetCoordinateSystemToDisplay();
	coordinate->SetValue(x, y, 0);
	double* world = coordinate->GetComputedWorldValue(Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());

	

	/*if (world[0] < coordinate_[0])
	{
		coordinate_[0] -= 0.01;
	}
	if (world[0] > coordinate_[0])
	{
		coordinate_[0] += 0.01;
	}
	if (world[1] < coordinate_[1])
	{
		coordinate_[1] -= 0.01;
	}
	if (world[1] > coordinate_[1])
	{
		coordinate_[1] += 0.01;
	}*/

	std::cout << "World coordinate: " << world[0] << ", " << world[1] << std::endl;

	vtkInteractorStyleTrackballCamera::OnMouseMove();
	rebuildMarker(world);
}

void GridStyleFour::OnLeftButtonDown()
{
	SetTimerDuration(1);
	UseTimersOn();
	vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
	StartTimer();
}

void GridStyleFour::OnLeftButtonUp()
{
	EndTimer();
	//UseTimersOff();
	vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
}

void GridStyleFour::OnMouseWheelBackward()
{
	vtkInteractorStyleTrackballCamera::OnMouseWheelBackward();
	//zPosition = camera_->GetPosition()[2];
	//cout << zPosition << endl;
	countBack++;
	if (countBack % 5 == 0) {
		countBack = 1;
		//grid_CellX *= 2;
	}
	//OnTimer();
	//flg = true;
}

void GridStyleFour::OnMouseWheelForward()
{
	vtkInteractorStyleTrackballCamera::OnMouseWheelForward();
	zPosition = camera_->GetPosition()[2];
	/*cout << zPosition << endl;*/
	countForw++;
	if (countForw % 5 == 0) {
		countForw = 1;
		//grid_CellX /= 2;
	}
	//OnTimer();
	//flg = true;
}

void GridStyleFour::OnTimer()
{
	if (camera_->GetPosition()[2] / zPosition < 0.9999) {
		camera_->SetPosition(camera_->GetPosition()[0], camera_->GetPosition()[1], zPosition);
	}
	rebuildGird();
	/*int* sizes = renderer_->GetSize();

	auto cameraScale = camera_->GetParallelScale();
	lastCameraScale_ = cameraScale;
	auto height = 2 * cameraScale;
	worldToScreenCoeff_ = height / sizes[1];
	auto width = worldToScreenCoeff_ * sizes[0];
	viewportSize[0] = width * camera_->GetPosition()[2];
	viewportSize[1] = height * camera_->GetPosition()[2];

	auto scaleX = std::abs(camera_->GetFocalPoint()[0]) + viewportSize[0] / 2.0;
	auto scaleY = std::abs(camera_->GetFocalPoint()[1]) + viewportSize[1] / 2.0;

	double scale[3] = { scaleX, scaleY, 0 };
	axesMain_->SetScale(scale);*/
	
}


void GridStyleFour::rebuildMarker(double* coordinate)
{
	vtkSmartPointer<vtkRegularPolygonSource> marker = vtkSmartPointer<vtkRegularPolygonSource>::New();
	marker->SetNumberOfSides(50);
	marker->SetRadius(0.001);
	marker->SetCenter(coordinate[0], coordinate[1], 0);

	vtkPolyDataMapper::SafeDownCast(actorMarker_->GetMapper())->SetInputConnection(marker->GetOutputPort());
	Interactor->GetRenderWindow()->Render();
}



void GridStyleFour::rebuildGird()
{
	double xCam = camera_->GetPosition()[0];
	double yCam = camera_->GetPosition()[1];

	double xA = gridA_->GetPosition()[0];
	double yA = gridA_->GetPosition()[1];

	double xB = gridB_->GetPosition()[0];
	double yB = gridB_->GetPosition()[1];

	double xC = gridC_->GetPosition()[0];
	double yC = gridC_->GetPosition()[1];

	double xD = gridD_->GetPosition()[0];
	double yD = gridD_->GetPosition()[1];

	if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 1) {
		if (abs(xA - (xCam - 0.16)) < 0.20 && abs(yA - (yCam - 0.16)) < 0.20) {
			moveToSouthWest(gridB_, gridD_, gridC_);

		}
		else if (abs(xB - (xCam - 0.16)) < 0.20 && abs((yB + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthWest(gridD_, gridC_, gridA_);
		}
		else if (abs((xD +1.0) - (xCam + 0.16)) < 0.20 && abs((yD + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthEast(gridB_, gridA_, gridC_);
		}
		else if (abs((xC + 1.0) - (xCam + 0.16)) < 0.20 && abs(yC - (yCam - 0.16)) < 0.20) {
			moveToSouthEast(gridA_, gridB_, gridC_);
		}


		else if (abs((yB + 1.0) - (yCam + 0.16)) < 0.20 && abs(xB - (xCam - 0.16)) >= 0.20 && abs((xD + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridA_, gridC_, 2.0);
		} 
		else if (abs((xD + 1.0) - (xCam + 0.16)) < 0.20 && abs((yD + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yC - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridB_, gridA_, 2.0);
		}
		else if (abs(yA-(yCam - 0.16))<0.20 && abs(xA- (xCam - 0.16)) >= 0.20 && abs((xC+1.0) - (xCam+0.16))>=0.20) {
			moveTwoActorsByY(gridB_, gridD_, -2.0);
		} 
		else if (abs(xB - (xCam - 0.16)) < 0.20 && abs((yB + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yA - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridD_, gridC_, -2.0);
		}
	} 
	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 2) {
		if (abs(xA - (xCam - 0.16)) < 0.20 && abs(yA - (yCam - 0.16)) < 0.20) {
			moveToSouthWest(gridC_, gridD_, gridB_);

		}
		else if (abs(xC - (xCam - 0.16)) < 0.20 && abs((yC + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthWest(gridD_, gridB_, gridA_);
		}
		else if (abs((xD + 1.0) - (xCam + 0.16)) < 0.20 && abs((yD + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthEast(gridC_, gridA_, gridB_);
		}
		else if (abs((xB + 1.0) - (xCam + 0.16)) < 0.20 && abs(yB - (yCam - 0.16)) < 0.20) {
			moveToSouthEast(gridA_, gridC_, gridB_);
		}


		else if (abs((yC + 1.0) - (yCam + 0.16)) < 0.20 && abs(xC - (xCam - 0.16)) >= 0.20 && abs((xD + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridA_, gridB_, 2.0);
		}
		else if (abs((xD + 1.0) - (xCam + 0.16)) < 0.20 && abs((yD + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yB - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridC_, gridA_, 2.0);
		}
		else if (abs(yA - (yCam - 0.16)) < 0.20 && abs(xA - (xCam - 0.16)) >= 0.20 && abs((xB + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridC_, gridD_, -2.0);
		}
		else if (abs(xC - (xCam - 0.16)) < 0.20 && abs((yC + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yA - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridD_, gridB_, -2.0);
		}
	} 

	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 3) {
		if (abs(xA - (xCam - 0.16)) < 0.20 && abs(yA - (yCam - 0.16)) < 0.20) {
			moveToSouthWest(gridD_, gridC_, gridB_);

		}
		else if (abs(xD - (xCam - 0.16)) < 0.20 && abs((yD + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthWest(gridC_, gridB_, gridA_);
		}
		else if (abs((xC + 1.0) - (xCam + 0.16)) < 0.20 && abs((yC + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthEast(gridD_, gridA_, gridB_);
		}
		else if (abs((xB + 1.0) - (xCam + 0.16)) < 0.20 && abs(yB - (yCam - 0.16)) < 0.20) {
			moveToSouthEast(gridA_, gridD_, gridB_);
		}


		else if (abs((yD + 1.0) - (yCam + 0.16)) < 0.20 && abs(xD - (xCam - 0.16)) >= 0.20 && abs((xC + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridA_, gridB_, 2.0);
		}
		else if (abs((xC + 1.0) - (xCam + 0.16)) < 0.20 && abs((yC + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yB - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridD_, gridA_, 2.0);
		}
		else if (abs(yA - (yCam - 0.16)) < 0.20 && abs(xA - (xCam - 0.16)) >= 0.20 && abs((xB + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridD_, gridC_, -2.0);
		}
		else if (abs(xD - (xCam - 0.16)) < 0.20 && abs((yD + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yA - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridC_, gridB_, -2.0);
		}
	}
	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 4) {
		if (abs(xA - (xCam - 0.16)) < 0.20 && abs(yA - (yCam - 0.16)) < 0.20) {
			moveToSouthWest(gridB_, gridC_, gridD_);

		}
		else if (abs(xB - (xCam - 0.16)) < 0.20 && abs((yB + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthWest(gridC_, gridD_, gridA_);
		}
		else if (abs((xC + 1.0) - (xCam + 0.16)) < 0.20 && abs((yC + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthEast(gridB_, gridA_, gridD_);
		}
		else if (abs((xD + 1.0) - (xCam + 0.16)) < 0.20 && abs(yD - (yCam - 0.16)) < 0.20) {
			moveToSouthEast(gridA_, gridB_, gridD_);
		}


		else if (abs((yB + 1.0) - (yCam + 0.16)) < 0.20 && abs(xB - (xCam - 0.16)) >= 0.20 && abs((xC + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridA_, gridD_, 2.0);
		}
		else if (abs((xC + 1.0) - (xCam + 0.16)) < 0.20 && abs((yC + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yD - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridB_, gridA_, 2.0);
		}
		else if (abs(yA - (yCam - 0.16)) < 0.20 && abs(xA - (xCam - 0.16)) >= 0.20 && abs((xD + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridB_, gridC_, -2.0);
		}
		else if (abs(xB - (xCam - 0.16)) < 0.20 && abs((yB + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yA - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridC_, gridD_, -2.0);
		}
	}
	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 5) {
		if (abs(xA - (xCam - 0.16)) < 0.20 && abs(yA - (yCam - 0.16)) < 0.20) {
			moveToSouthWest(gridC_, gridB_, gridD_);

		}
		else if (abs(xC - (xCam - 0.16)) < 0.20 && abs((yC + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthWest(gridB_, gridD_, gridA_);
		}
		else if (abs((xB + 1.0) - (xCam + 0.16)) < 0.20 && abs((yB + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthEast(gridC_, gridA_, gridD_);
		}
		else if (abs((xD + 1.0) - (xCam + 0.16)) < 0.20 && abs(yD - (yCam - 0.16)) < 0.20) {
			moveToSouthEast(gridA_, gridC_, gridD_);
		}


		else if (abs((yC + 1.0) - (yCam + 0.16)) < 0.20 && abs(xC - (xCam - 0.16)) >= 0.20 && abs((xB + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridA_, gridD_, 2.0);
		}
		else if (abs((xB + 1.0) - (xCam + 0.16)) < 0.20 && abs((yB + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yD - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridC_, gridA_, 2.0);
		}
		else if (abs(yA - (yCam - 0.16)) < 0.20 && abs(xA - (xCam - 0.16)) >= 0.20 && abs((xD + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridC_, gridB_, -2.0);
		}
		else if (abs(xC - (xCam - 0.16)) < 0.20 && abs((yC + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yA - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridB_, gridD_, -2.0);
		}
	}
	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 6) {
	if (abs(xA - (xCam - 0.16)) < 0.20 && abs(yA - (yCam - 0.16)) < 0.20) {
		moveToSouthWest(gridD_, gridB_, gridC_);

	}
	else if (abs(xD - (xCam - 0.16)) < 0.20 && abs((yD + 1.0) - (yCam + 0.16)) < 0.20) {
		moveToNorthWest(gridB_, gridC_, gridA_);
	}
	else if (abs((xB + 1.0) - (xCam + 0.16)) < 0.20 && abs((yB + 1.0) - (yCam + 0.16)) < 0.20) {
		moveToNorthEast(gridD_, gridA_, gridC_);
	}
	else if (abs((xC + 1.0) - (xCam + 0.16)) < 0.20 && abs(yC - (yCam - 0.16)) < 0.20) {
		moveToSouthEast(gridA_, gridD_, gridC_);
	}


	else if (abs((yD + 1.0) - (yCam + 0.16)) < 0.20 && abs(xD - (xCam - 0.16)) >= 0.20 && abs((xB + 1.0) - (xCam + 0.16)) >= 0.20) {
		moveTwoActorsByY(gridA_, gridC_, 2.0);
	}
	else if (abs((xB + 1.0) - (xCam + 0.16)) < 0.20 && abs((yB + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yC - (yCam - 0.16)) >= 0.20) {
		moveTwoActorsByX(gridD_, gridA_, 2.0);
	}
	else if (abs(yA - (yCam - 0.16)) < 0.20 && abs(xA - (xCam - 0.16)) >= 0.20 && abs((xC + 1.0) - (xCam + 0.16)) >= 0.20) {
		moveTwoActorsByY(gridD_, gridB_, -2.0);
	}
	else if (abs(xD - (xCam - 0.16)) < 0.20 && abs((yD + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yA - (yCam - 0.16)) >= 0.20) {
		moveTwoActorsByX(gridB_, gridC_, -2.0);
	}
	}
	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 7) {
		if (abs(xC - (xCam - 0.16)) < 0.20 && abs(yC - (yCam - 0.16)) < 0.20) {
			moveToSouthWest(gridA_, gridB_, gridD_);

		}
		else if (abs(xA - (xCam - 0.16)) < 0.20 && abs((yA + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthWest(gridB_, gridD_, gridC_);
		}
		else if (abs((xB + 1.0) - (xCam + 0.16)) < 0.20 && abs((yB + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthEast(gridA_, gridC_, gridD_);
		}
		else if (abs((xD + 1.0) - (xCam + 0.16)) < 0.20 && abs(yD - (yCam - 0.16)) < 0.20) {
			moveToSouthEast(gridC_, gridA_, gridD_);
		}


		else if (abs((yA + 1.0) - (yCam + 0.16)) < 0.20 && abs(xA - (xCam - 0.16)) >= 0.20 && abs((xB + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridC_, gridD_, 2.0);
		}
		else if (abs((xB + 1.0) - (xCam + 0.16)) < 0.20 && abs((yB + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yD - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridA_, gridC_, 2.0);
		}
		else if (abs(yC - (yCam - 0.16)) < 0.20 && abs(xC - (xCam - 0.16)) >= 0.20 && abs((xD + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridA_, gridB_, -2.0);
		}
		else if (abs(xA - (xCam - 0.16)) < 0.20 && abs((yA + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yC - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridB_, gridD_, -2.0);
		}
	}
	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 8) {
		if (abs(xB - (xCam - 0.16)) < 0.20 && abs(yB - (yCam - 0.16)) < 0.20) {
			moveToSouthWest(gridA_, gridC_, gridD_);

		}
		else if (abs(xA - (xCam - 0.16)) < 0.20 && abs((yA + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthWest(gridC_, gridD_, gridB_);
		}
		else if (abs((xC + 1.0) - (xCam + 0.16)) < 0.20 && abs((yC + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthEast(gridA_, gridB_, gridD_);
		}
		else if (abs((xD + 1.0) - (xCam + 0.16)) < 0.20 && abs(yD - (yCam - 0.16)) < 0.20) {
			moveToSouthEast(gridB_, gridA_, gridD_);
		}


		else if (abs((yA + 1.0) - (yCam + 0.16)) < 0.20 && abs(xA - (xCam - 0.16)) >= 0.20 && abs((xC + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridB_, gridD_, 2.0);
		}
		else if (abs((xC + 1.0) - (xCam + 0.16)) < 0.20 && abs((yC + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yD - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridA_, gridB_, 2.0);
		}
		else if (abs(yB - (yCam - 0.16)) < 0.20 && abs(xB - (xCam - 0.16)) >= 0.20 && abs((xD + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridA_, gridC_, -2.0);
		}
		else if (abs(xA - (xCam - 0.16)) < 0.20 && abs((yA + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yB - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridC_, gridD_, -2.0);
		}
	}
	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 9) {
		if (abs(xB - (xCam - 0.16)) < 0.20 && abs(yB - (yCam - 0.16)) < 0.20) {
			moveToSouthWest(gridA_, gridD_, gridC_);

		}
		else if (abs(xA - (xCam - 0.16)) < 0.20 && abs((yA + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthWest(gridD_, gridC_, gridB_);
		}
		else if (abs((xD + 1.0) - (xCam + 0.16)) < 0.20 && abs((yD + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthEast(gridA_, gridB_, gridC_);
		}
		else if (abs((xC + 1.0) - (xCam + 0.16)) < 0.20 && abs(yC - (yCam - 0.16)) < 0.20) {
			moveToSouthEast(gridB_, gridA_, gridC_);
		}


		else if (abs((yA + 1.0) - (yCam + 0.16)) < 0.20 && abs(xA - (xCam - 0.16)) >= 0.20 && abs((xD + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridB_, gridC_, 2.0);
		}
		else if (abs((xD + 1.0) - (xCam + 0.16)) < 0.20 && abs((yD + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yC - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridA_, gridB_, 2.0);
		}
		else if (abs(yB - (yCam - 0.16)) < 0.20 && abs(xB - (xCam - 0.16)) >= 0.20 && abs((xC + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridA_, gridD_, -2.0);
		}
		else if (abs(xA - (xCam - 0.16)) < 0.20 && abs((yA + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yB - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridD_, gridC_, -2.0);
		}

	}
	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 10) {
		if (abs(xD - (xCam - 0.16)) < 0.20 && abs(yD - (yCam - 0.16)) < 0.20) {
			moveToSouthWest(gridA_, gridB_, gridC_);

		}
		else if (abs(xA - (xCam - 0.16)) < 0.20 && abs((yA + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthWest(gridB_, gridC_, gridD_);
		}
		else if (abs((xB + 1.0) - (xCam + 0.16)) < 0.20 && abs((yB + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthEast(gridA_, gridD_, gridC_);
		}
		else if (abs((xC + 1.0) - (xCam + 0.16)) < 0.20 && abs(yC - (yCam - 0.16)) < 0.20) {
			moveToSouthEast(gridD_, gridA_, gridC_);
		}


		else if (abs((yA + 1.0) - (yCam + 0.16)) < 0.20 && abs(xA - (xCam - 0.16)) >= 0.20 && abs((xB + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridD_, gridC_, 2.0);
		}
		else if (abs((xB + 1.0) - (xCam + 0.16)) < 0.20 && abs((yB + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yC - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridA_, gridD_, 2.0);
		}
		else if (abs(yD - (yCam - 0.16)) < 0.20 && abs(xD - (xCam - 0.16)) >= 0.20 && abs((xC + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridA_, gridB_, -2.0);
		}
		else if (abs(xA - (xCam - 0.16)) < 0.20 && abs((yA + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yD - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridB_, gridC_, -2.0);
		}

	}
	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 11) {
		if (abs(xC - (xCam - 0.16)) < 0.20 && abs(yC - (yCam - 0.16)) < 0.20) {
			moveToSouthWest(gridA_, gridD_, gridB_);

		}
		else if (abs(xA - (xCam - 0.16)) < 0.20 && abs((yA + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthWest(gridD_, gridB_, gridC_);
		}
		else if (abs((xD + 1.0) - (xCam + 0.16)) < 0.20 && abs((yD + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthEast(gridA_, gridC_, gridB_);
		}
		else if (abs((xB + 1.0) - (xCam + 0.16)) < 0.20 && abs(yB - (yCam - 0.16)) < 0.20) {
			moveToSouthEast(gridC_, gridA_, gridB_);
		}


		else if (abs((yA + 1.0) - (yCam + 0.16)) < 0.20 && abs(xA - (xCam - 0.16)) >= 0.20 && abs((xD + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridC_, gridB_, 2.0);
		}
		else if (abs((xD + 1.0) - (xCam + 0.16)) < 0.20 && abs((yD + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yB - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridA_, gridC_, 2.0);
		}
		else if (abs(yC - (yCam - 0.16)) < 0.20 && abs(xC - (xCam - 0.16)) >= 0.20 && abs((xB + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridA_, gridD_, -2.0);
		}
		else if (abs(xA - (xCam - 0.16)) < 0.20 && abs((yA + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yC - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridD_, gridB_, -2.0);
		}
	}
	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 12) {
		if (abs(xD - (xCam - 0.16)) < 0.20 && abs(yD - (yCam - 0.16)) < 0.20) {
			moveToSouthWest(gridA_, gridC_, gridB_);

		}
		else if (abs(xA - (xCam - 0.16)) < 0.20 && abs((yA + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthWest(gridC_, gridB_, gridD_);
		}
		else if (abs((xC + 1.0) - (xCam + 0.16)) < 0.20 && abs((yC + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthEast(gridA_, gridD_, gridB_);
		}
		else if (abs((xB + 1.0) - (xCam + 0.16)) < 0.20 && abs(yB - (yCam - 0.16)) < 0.20) {
			moveToSouthEast(gridD_, gridA_, gridB_);
		}


		else if (abs((yA + 1.0) - (yCam + 0.16)) < 0.20 && abs(xA - (xCam - 0.16)) >= 0.20 && abs((xC + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridD_, gridB_, 2.0);
		}
		else if (abs((xC + 1.0) - (xCam + 0.16)) < 0.20 && abs((yC + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yB - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridA_, gridD_, 2.0);
		}
		else if (abs(yD - (yCam - 0.16)) < 0.20 && abs(xD - (xCam - 0.16)) >= 0.20 && abs((xB + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridA_, gridC_, -2.0);
		}
		else if (abs(xA - (xCam - 0.16)) < 0.20 && abs((yA + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yD - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridC_, gridB_, -2.0);
		}
	}
	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 13) {
		if (abs(xD - (xCam - 0.16)) < 0.20 && abs(yD - (yCam - 0.16)) < 0.20) {
			moveToSouthWest(gridB_, gridA_, gridC_);

		}
		else if (abs(xB - (xCam - 0.16)) < 0.20 && abs((yB + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthWest(gridA_, gridC_, gridD_);
		}
		else if (abs((xA + 1.0) - (xCam + 0.16)) < 0.20 && abs((yA + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthEast(gridB_, gridD_, gridC_);
		}
		else if (abs((xC + 1.0) - (xCam + 0.16)) < 0.20 && abs(yC - (yCam - 0.16)) < 0.20) {
			moveToSouthEast(gridD_, gridB_, gridC_);
		}


		else if (abs((yB + 1.0) - (yCam + 0.16)) < 0.20 && abs(xB - (xCam - 0.16)) >= 0.20 && abs((xA + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridD_, gridC_, 2.0);
		}
		else if (abs((xA + 1.0) - (xCam + 0.16)) < 0.20 && abs((yA + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yC - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridB_, gridD_, 2.0);
		}
		else if (abs(yD - (yCam - 0.16)) < 0.20 && abs(xD - (xCam - 0.16)) >= 0.20 && abs((xC + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridB_, gridA_, -2.0);
		}
		else if (abs(xB - (xCam - 0.16)) < 0.20 && abs((yB + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yD - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridA_, gridC_, -2.0);
		}
	}
	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 14) {
		if (abs(xD - (xCam - 0.16)) < 0.20 && abs(yD - (yCam - 0.16)) < 0.20) {
			moveToSouthWest(gridC_, gridA_, gridB_);

		}
		else if (abs(xC - (xCam - 0.16)) < 0.20 && abs((yC + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthWest(gridA_, gridB_, gridD_);
		}
		else if (abs((xA + 1.0) - (xCam + 0.16)) < 0.20 && abs((yA + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthEast(gridC_, gridD_, gridB_);
		}
		else if (abs((xB + 1.0) - (xCam + 0.16)) < 0.20 && abs(yB - (yCam - 0.16)) < 0.20) {
			moveToSouthEast(gridD_, gridC_, gridB_);
		}


		else if (abs((yC + 1.0) - (yCam + 0.16)) < 0.20 && abs(xC - (xCam - 0.16)) >= 0.20 && abs((xA + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridD_, gridB_, 2.0);
		}
		else if (abs((xA + 1.0) - (xCam + 0.16)) < 0.20 && abs((yA + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yB - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridC_, gridD_, 2.0);
		}
		else if (abs(yD - (yCam - 0.16)) < 0.20 && abs(xD - (xCam - 0.16)) >= 0.20 && abs((xB + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridC_, gridA_, -2.0);
		}
		else if (abs(xC - (xCam - 0.16)) < 0.20 && abs((yC + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yD - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridA_, gridB_, -2.0);
		}
	}
	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 15) {
		if (abs(xB - (xCam - 0.16)) < 0.20 && abs(yB - (yCam - 0.16)) < 0.20) {
			moveToSouthWest(gridD_, gridA_, gridC_);

		}
		else if (abs(xD - (xCam - 0.16)) < 0.20 && abs((yD + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthWest(gridA_, gridC_, gridB_);
		}
		else if (abs((xA + 1.0) - (xCam + 0.16)) < 0.20 && abs((yA + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthEast(gridD_, gridB_, gridC_);
		}
		else if (abs((xC + 1.0) - (xCam + 0.16)) < 0.20 && abs(yC - (yCam - 0.16)) < 0.20) {
			moveToSouthEast(gridB_, gridD_, gridC_);
		}


		else if (abs((yD + 1.0) - (yCam + 0.16)) < 0.20 && abs(xD - (xCam - 0.16)) >= 0.20 && abs((xA + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridB_, gridC_, 2.0);
		}
		else if (abs((xA + 1.0) - (xCam + 0.16)) < 0.20 && abs((yA + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yC - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridD_, gridB_, 2.0);
		}
		else if (abs(yB - (yCam - 0.16)) < 0.20 && abs(xB - (xCam - 0.16)) >= 0.20 && abs((xC + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridD_, gridA_, -2.0);
		}
		else if (abs(xD - (xCam - 0.16)) < 0.20 && abs((yD + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yB - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridA_, gridC_, -2.0);
		}
	}
	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 16) {
		if (abs(xB - (xCam - 0.16)) < 0.20 && abs(yB - (yCam - 0.16)) < 0.20) {
			moveToSouthWest(gridC_, gridA_, gridD_);

		}
		else if (abs(xC - (xCam - 0.16)) < 0.20 && abs((yC + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthWest(gridA_, gridD_, gridB_);
		}
		else if (abs((xA + 1.0) - (xCam + 0.16)) < 0.20 && abs((yA + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthEast(gridC_, gridB_, gridD_);
		}
		else if (abs((xD + 1.0) - (xCam + 0.16)) < 0.20 && abs(yD - (yCam - 0.16)) < 0.20) {
			moveToSouthEast(gridB_, gridC_, gridD_);
		}


		else if (abs((yC + 1.0) - (yCam + 0.16)) < 0.20 && abs(xC - (xCam - 0.16)) >= 0.20 && abs((xA + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridB_, gridD_, 2.0);
		}
		else if (abs((xA + 1.0) - (xCam + 0.16)) < 0.20 && abs((yA + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yD - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridC_, gridB_, 2.0);
		}
		else if (abs(yB - (yCam - 0.16)) < 0.20 && abs(xB - (xCam - 0.16)) >= 0.20 && abs((xD + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridC_, gridA_, -2.0);
		}
		else if (abs(xC - (xCam - 0.16)) < 0.20 && abs((yC + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yB - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridA_, gridD_, -2.0);
		}
	}
	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 17) {
		if (abs(xC - (xCam - 0.16)) < 0.20 && abs(yC - (yCam - 0.16)) < 0.20) {
			moveToSouthWest(gridD_, gridA_, gridB_);

		}
		else if (abs(xD - (xCam - 0.16)) < 0.20 && abs((yD + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthWest(gridA_, gridB_, gridC_);
		}
		else if (abs((xA + 1.0) - (xCam + 0.16)) < 0.20 && abs((yA + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthEast(gridD_, gridC_, gridB_);
		}
		else if (abs((xB + 1.0) - (xCam + 0.16)) < 0.20 && abs(yB - (yCam - 0.16)) < 0.20) {
			moveToSouthEast(gridC_, gridD_, gridB_);
		}


		else if (abs((yD + 1.0) - (yCam + 0.16)) < 0.20 && abs(xD - (xCam - 0.16)) >= 0.20 && abs((xA + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridC_, gridB_, 2.0);
		}
		else if (abs((xA + 1.0) - (xCam + 0.16)) < 0.20 && abs((yA + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yB - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridD_, gridC_, 2.0);
		}
		else if (abs(yC - (yCam - 0.16)) < 0.20 && abs(xC - (xCam - 0.16)) >= 0.20 && abs((xB + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridD_, gridA_, -2.0);
		}
		else if (abs(xD - (xCam - 0.16)) < 0.20 && abs((yD + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yC - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridA_, gridB_, -2.0);
		}
	}
	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 18) {
		if (abs(xC - (xCam - 0.16)) < 0.20 && abs(yC - (yCam - 0.16)) < 0.20) {
			moveToSouthWest(gridB_, gridA_, gridD_);

		}
		else if (abs(xB - (xCam - 0.16)) < 0.20 && abs((yB + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthWest(gridA_, gridD_, gridC_);
		}
		else if (abs((xA + 1.0) - (xCam + 0.16)) < 0.20 && abs((yA + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthEast(gridB_, gridC_, gridD_);
		}
		else if (abs((xD + 1.0) - (xCam + 0.16)) < 0.20 && abs(yD - (yCam - 0.16)) < 0.20) {
			moveToSouthEast(gridC_, gridB_, gridD_);
		}


		else if (abs((yB + 1.0) - (yCam + 0.16)) < 0.20 && abs(xB - (xCam - 0.16)) >= 0.20 && abs((xA + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridC_, gridD_, 2.0);
		}
		else if (abs((xA + 1.0) - (xCam + 0.16)) < 0.20 && abs((yA + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yD - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridB_, gridC_, 2.0);
		}
		else if (abs(yC - (yCam - 0.16)) < 0.20 && abs(xC - (xCam - 0.16)) >= 0.20 && abs((xD + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridB_, gridA_, -2.0);
		}
		else if (abs(xB - (xCam - 0.16)) < 0.20 && abs((yB + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yC - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridA_, gridD_, -2.0);
		}
	}
	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 19) {
		if (abs(xB - (xCam - 0.16)) < 0.20 && abs(yB - (yCam - 0.16)) < 0.20) {
			moveToSouthWest(gridD_, gridC_, gridA_);

		}
		else if (abs(xD - (xCam - 0.16)) < 0.20 && abs((yD + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthWest(gridC_, gridA_, gridB_);
		}
		else if (abs((xC + 1.0) - (xCam + 0.16)) < 0.20 && abs((yC + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthEast(gridD_, gridB_, gridA_);
		}
		else if (abs((xA + 1.0) - (xCam + 0.16)) < 0.20 && abs(yA - (yCam - 0.16)) < 0.20) {
			moveToSouthEast(gridB_, gridD_, gridA_);
		}


		else if (abs((yD + 1.0) - (yCam + 0.16)) < 0.20 && abs(xD - (xCam - 0.16)) >= 0.20 && abs((xC + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridB_, gridA_, 2.0);
		}
		else if (abs((xC + 1.0) - (xCam + 0.16)) < 0.20 && abs((yC + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yA - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridD_, gridB_, 2.0);
		}
		else if (abs(yB - (yCam - 0.16)) < 0.20 && abs(xB - (xCam - 0.16)) >= 0.20 && abs((xA + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridD_, gridC_, -2.0);
		}
		else if (abs(xD - (xCam - 0.16)) < 0.20 && abs((yD + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yB - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridC_, gridA_, -2.0);
		}
	}
	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 20) {
		if (abs(xC - (xCam - 0.16)) < 0.20 && abs(yC - (yCam - 0.16)) < 0.20) {
			moveToSouthWest(gridD_, gridB_, gridA_);
		}
		else if (abs(xD - (xCam - 0.16)) < 0.20 && abs((yD + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthWest(gridB_, gridA_, gridC_);
		}
		else if (abs((xB + 1.0) - (xCam + 0.16)) < 0.20 && abs((yB + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthEast(gridD_, gridC_, gridA_);
		}
		else if (abs((xA + 1.0) - (xCam + 0.16)) < 0.20 && abs(yA - (yCam - 0.16)) < 0.20) {
			moveToSouthEast(gridC_, gridD_, gridA_);
		}


		else if (abs((yD + 1.0) - (yCam + 0.16)) < 0.20 && abs(xD - (xCam - 0.16)) >= 0.20 && abs((xB + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridC_, gridA_, 2.0);
		}
		else if (abs((xB + 1.0) - (xCam + 0.16)) < 0.20 && abs((yB + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yA - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridD_, gridC_, 2.0);
		}
		else if (abs(yC - (yCam - 0.16)) < 0.20 && abs(xC - (xCam - 0.16)) >= 0.20 && abs((xA + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridD_, gridB_, -2.0);
		}
		else if (abs(xD - (xCam - 0.16)) < 0.20 && abs((yD + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yC - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridB_, gridA_, -2.0);
		}
	}
	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 21) {
		if (abs(xD - (xCam - 0.16)) < 0.20 && abs(yD - (yCam - 0.16)) < 0.20) {
			moveToSouthWest(gridC_, gridB_, gridA_);

		}
		else if (abs(xC - (xCam - 0.16)) < 0.20 && abs((yC + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthWest(gridB_, gridA_, gridD_);
		}
		else if (abs((xB + 1.0) - (xCam + 0.16)) < 0.20 && abs((yB + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthEast(gridC_, gridD_, gridA_);
		}
		else if (abs((xA + 1.0) - (xCam + 0.16)) < 0.20 && abs(yA - (yCam - 0.16)) < 0.20) {
			moveToSouthEast(gridD_, gridC_, gridA_);
		}


		else if (abs((yC + 1.0) - (yCam + 0.16)) < 0.20 && abs(xC - (xCam - 0.16)) >= 0.20 && abs((xB + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridD_, gridA_, 2.0);
		}
		else if (abs((xB + 1.0) - (xCam + 0.16)) < 0.20 && abs((yB + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yA - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridC_, gridD_, 2.0);
		}
		else if (abs(yD - (yCam - 0.16)) < 0.20 && abs(xD - (xCam - 0.16)) >= 0.20 && abs((xA + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridC_, gridB_, -2.0);
		}
		else if (abs(xC - (xCam - 0.16)) < 0.20 && abs((yC + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yD - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridB_, gridA_, -2.0);
		}
	}
	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 22) {
		if (abs(xB - (xCam - 0.16)) < 0.20 && abs(yB - (yCam - 0.16)) < 0.20) {
			moveToSouthWest(gridC_, gridD_, gridA_);

		}
		else if (abs(xC - (xCam - 0.16)) < 0.20 && abs((yC + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthWest(gridD_, gridA_, gridB_);
		}
		else if (abs((xD + 1.0) - (xCam + 0.16)) < 0.20 && abs((yD + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthEast(gridC_, gridB_, gridA_);
		}
		else if (abs((xA + 1.0) - (xCam + 0.16)) < 0.20 && abs(yA - (yCam - 0.16)) < 0.20) {
			moveToSouthEast(gridB_, gridC_, gridA_);
		}


		else if (abs((yC + 1.0) - (yCam + 0.16)) < 0.20 && abs(xC - (xCam - 0.16)) >= 0.20 && abs((xD + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridB_, gridA_, 2.0);
		}
		else if (abs((xD + 1.0) - (xCam + 0.16)) < 0.20 && abs((yD + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yA - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridC_, gridB_, 2.0);
		}
		else if (abs(yB - (yCam - 0.16)) < 0.20 && abs(xB - (xCam - 0.16)) >= 0.20 && abs((xA + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridC_, gridD_, -2.0);
		}
		else if (abs(xC - (xCam - 0.16)) < 0.20 && abs((yC + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yB - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridD_, gridA_, -2.0);
		}
	}
	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 23) {
		if (abs(xD - (xCam - 0.16)) < 0.20 && abs(yD - (yCam - 0.16)) < 0.20) {
			moveToSouthWest(gridB_, gridC_, gridA_);

		}
		else if (abs(xB - (xCam - 0.16)) < 0.20 && abs((yB + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthWest(gridC_, gridA_, gridD_);
		}
		else if (abs((xC + 1.0) - (xCam + 0.16)) < 0.20 && abs((yC + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthEast(gridB_, gridD_, gridA_);
		}
		else if (abs((xA + 1.0) - (xCam + 0.16)) < 0.20 && abs(yA - (yCam - 0.16)) < 0.20) {
			moveToSouthEast(gridD_, gridB_, gridA_);
		}


		else if (abs((yB + 1.0) - (yCam + 0.16)) < 0.20 && abs(xB - (xCam - 0.16)) >= 0.20 && abs((xC + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridD_, gridA_, 2.0);
		}
		else if (abs((xC + 1.0) - (xCam + 0.16)) < 0.20 && abs((yC + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yA - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridB_, gridD_, 2.0);
		}
		else if (abs(yD - (yCam - 0.16)) < 0.20 && abs(xD - (xCam - 0.16)) >= 0.20 && abs((xA + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridB_, gridC_, -2.0);
		}
		else if (abs(xB - (xCam - 0.16)) < 0.20 && abs((yB + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yD - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridC_, gridA_, -2.0);
		}
	}
	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 24) {
		if (abs(xC - (xCam - 0.16)) < 0.20 && abs(yC - (yCam - 0.16)) < 0.20) {
			moveToSouthWest(gridB_, gridD_, gridA_);

		}
		else if (abs(xB - (xCam - 0.16)) < 0.20 && abs((yB + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthWest(gridD_, gridA_, gridC_);
		}
		else if (abs((xD + 1.0) - (xCam + 0.16)) < 0.20 && abs((yD + 1.0) - (yCam + 0.16)) < 0.20) {
			moveToNorthEast(gridB_, gridC_, gridA_);
		}
		else if (abs((xA + 1.0) - (xCam + 0.16)) < 0.20 && abs(yA - (yCam - 0.16)) < 0.20) {
			moveToSouthEast(gridC_, gridB_, gridA_);
		}


		else if (abs((yB + 1.0) - (yCam + 0.16)) < 0.20 && abs(xB - (xCam - 0.16)) >= 0.20 && abs((xD + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridC_, gridA_, 2.0);
		}
		else if (abs((xD + 1.0) - (xCam + 0.16)) < 0.20 && abs((yD + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yA - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridB_, gridC_, 2.0);
		}
		else if (abs(yC - (yCam - 0.16)) < 0.20 && abs(xC - (xCam - 0.16)) >= 0.20 && abs((xA + 1.0) - (xCam + 0.16)) >= 0.20) {
			moveTwoActorsByY(gridB_, gridD_, -2.0);
		}
		else if (abs(xB - (xCam - 0.16)) < 0.20 && abs((yB + 1.0) - (yCam + 0.16)) >= 0.20 && abs(yC - (yCam - 0.16)) >= 0.20) {
			moveTwoActorsByX(gridD_, gridA_, -2.0);
		}
	}

	//if (isPresentInOne(gridA_)) {
	//	cout << "1" << endl;
	//	if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 1 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 2) {
	//		
	//		//moveToSouthWest(gridB_, gridD_, gridC_);
	//	} 
	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 3 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 4) {
	//		//moveToSouthWest(gridB_, gridC_, gridD_);
	//	}
	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 5 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 6) {
	//		//moveToSouthWest(gridC_, gridB_, gridD_); 
	//	}

	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 7 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 8) {
	//		//moveToNorthWest(gridB_, gridD_, gridC_);
	//	}
	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 9 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 10) {
	//		//moveToNorthWest(gridB_, gridC_, gridD_);
	//	} 
	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 11 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 12) {
	//		//moveToNorthWest(gridC_, gridB_, gridD_);
	//	}

	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 13 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 14) {
	//		double xCam = camera_->GetPosition()[0];
	//		double yCam = camera_->GetPosition()[1];

	//		double xA = gridA_->GetPosition()[0];
	//		double yA = gridA_->GetPosition()[1];
	//		cout << "13" << endl;
	//		if (abs((xA + 1.0) - (xCam + 0.16)) < 0.2 && abs((yA + 1.0) - (yCam + 0.16)) < 0.2) {
	//			//cout << "13" << endl;
	//			moveToNorthEast(gridC_, gridD_, gridB_);
	//		}

	//		
	//	}
	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 15 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 16) {
	//		double xCam = camera_->GetPosition()[0];
	//		double yCam = camera_->GetPosition()[1];

	//		double xA = gridA_->GetPosition()[0];
	//		double yA = gridA_->GetPosition()[1];
	//		cout << "15" << endl;
	//		if (abs((xA + 1.0) - (xCam + 0.16)) < 0.2 && abs((yA + 1.0) - (yCam + 0.16)) < 0.2) {
	//			//cout << "15" << endl;
	//			moveToNorthEast(gridC_, gridD_, gridB_);
	//		}
	//	}
	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 17 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 18) {
	//		double xCam = camera_->GetPosition()[0];
	//		double yCam = camera_->GetPosition()[1];

	//		double xA = gridA_->GetPosition()[0];
	//		double yA = gridA_->GetPosition()[1];
	//		cout << "16" << endl;
	//		if (abs((xA + 1.0) - (xCam + 0.16)) < 0.2 && abs((yA + 1.0) - (yCam + 0.16)) < 0.2) {
	//			//cout << "16" << endl;
	//			moveToNorthEast(gridC_, gridD_, gridB_);
	//		}
	//	}

	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 19 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 20) {
	//		//moveToSouthEast(gridB_, gridD_, gridC_);
	//	}
	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 21 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 22) {
	//		//moveToSouthEast(gridB_, gridC_, gridD_);
	//	}
	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 23 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 24) {
	//		//moveToSouthEast(gridD_, gridB_, gridC_);
	//	}

	//}

	//else if (isPresentInOne(gridB_)) {
	//	if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 1 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 2) {
	//		moveToSouthWest(gridA_, gridD_, gridC_);
	//	}
	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 3 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 4) {
	//		moveToSouthWest(gridA_, gridC_, gridD_);
	//	}
	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 5 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 6) {
	//		moveToSouthWest(gridC_, gridA_, gridD_);
	//	}

	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 7 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 8) {
	//		moveToNorthWest(gridA_, gridD_, gridC_);
	//	}
	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 9 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 10) {
	//		moveToNorthWest(gridA_, gridC_, gridD_);
	//	}
	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 11 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 12) {
	//		moveToNorthWest(gridC_, gridA_, gridD_);
	//	}

	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 13 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 14) {
	//		moveToNorthEast(gridC_, gridD_, gridA_);
	//	}
	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 15 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 16) {
	//		moveToNorthEast(gridC_, gridA_, gridD_);
	//	}
	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 17 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 18) {
	//		moveToNorthEast(gridA_, gridC_, gridD_);
	//	}

	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 19 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 20) {
	//		moveToSouthEast(gridA_, gridD_, gridC_);
	//	}
	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 21 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 22) {
	//		moveToSouthEast(gridA_, gridC_, gridD_);
	//	}
	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 23 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 24) {
	//		moveToSouthEast(gridD_, gridA_, gridC_);
	//	}
	//}

	//else if (isPresentInOne(gridC_)) {
	//	if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 1 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 2) {
	//		moveToSouthWest(gridB_, gridD_, gridA_);
	//	}
	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 3 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 4) {
	//		moveToSouthWest(gridB_, gridA_, gridD_);
	//	}
	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 5 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 6) {
	//		moveToSouthWest(gridA_, gridB_, gridD_);
	//	}

	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 7 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 8) {
	//		moveToNorthWest(gridB_, gridD_, gridA_);
	//	}
	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 9 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 10) {
	//		moveToNorthWest(gridB_, gridA_, gridD_);
	//	}
	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 11 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 12) {
	//		moveToNorthWest(gridA_, gridB_, gridD_);
	//	}

	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 13 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 14) {
	//		moveToNorthEast(gridA_, gridD_, gridB_);
	//	}
	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 15 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 16) {
	//		moveToNorthEast(gridA_, gridB_, gridD_);
	//	}
	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 17 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 18) {
	//		moveToNorthEast(gridB_, gridA_, gridD_);
	//	}

	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 19 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 20) {
	//		moveToSouthEast(gridB_, gridD_, gridA_);
	//	}
	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 21 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 22) {
	//		moveToSouthEast(gridB_, gridA_, gridD_);
	//	}
	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 23 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 24) {
	//		moveToSouthEast(gridD_, gridB_, gridA_);
	//	}
	//}
	//
	//else if (isPresentInOne(gridD_)) {
	//	if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 1 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 2) {
	//		moveToSouthWest(gridB_, gridA_, gridC_);
	//	}
	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 3 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 4) {
	//		moveToSouthWest(gridB_, gridC_, gridA_);
	//	}
	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 5 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 6) {
	//		moveToSouthWest(gridC_, gridB_, gridA_);
	//	}

	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 7 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 8) {
	//		moveToNorthWest(gridB_, gridA_, gridC_);
	//	}
	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 9 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 10) {
	//		moveToNorthWest(gridB_, gridC_, gridA_);
	//	}
	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 11 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 12) {
	//		moveToNorthWest(gridC_, gridB_, gridA_);
	//	}

	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 13 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 14) {
	//		moveToNorthEast(gridC_, gridA_, gridB_);
	//	}
	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 15 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 16) {
	//		moveToNorthEast(gridC_, gridB_, gridA_);
	//	}
	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 17 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 18) {
	//		moveToNorthEast(gridB_, gridC_, gridA_);
	//	}

	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 19 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 20) {
	//		moveToSouthEast(gridB_, gridA_, gridC_);
	//	}
	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 21 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 22) {
	//		moveToSouthEast(gridB_, gridC_, gridA_);
	//	}
	//	else if (getCurrentState(gridA_, gridB_, gridC_, gridD_) == 23 || getCurrentState(gridA_, gridB_, gridC_, gridD_) == 24) {
	//		moveToSouthEast(gridA_, gridB_, gridC_);
	//	}
	//}

	/*if (isPresentInPair(gridA_, gridB_))
	{
		if (gridC_->GetPosition()[0] == gridD_->GetPosition()[0] && gridA_->GetPosition()[0] < gridC_->GetPosition()[0])
		{
			cout << "rebuildGrid" << endl;

			moveTwoActorsByX(gridC_, gridD_, -2.0);
		}
		else if (gridC_->GetPosition()[0] == gridD_->GetPosition()[0] && gridA_->GetPosition()[0] > gridC_->GetPosition()[0])
		{
			cout << "rebuildGrid" << endl;

			moveTwoActorsByX(gridC_, gridD_, 2.0);
		}

		 else if (gridC_->GetPosition()[1] == gridD_->GetPosition()[1] && gridA_->GetPosition()[1] < gridC_->GetPosition()[1])
		{
			moveTwoActorsByY(gridC_, gridD_, -2.0);
		}
		else if (gridC_->GetPosition()[1] == gridD_->GetPosition()[1] && gridA_->GetPosition()[1] > gridC_->GetPosition()[1])
		{
			moveTwoActorsByY(gridC_, gridD_, 2.0);
		}
	}

	else if (isPresentInPair(gridA_, gridC_))
	{
		if (gridB_->GetPosition()[0] == gridD_->GetPosition()[0] && gridA_->GetPosition()[0] < gridB_->GetPosition()[0])
		{
			moveTwoActorsByX(gridB_, gridD_, -2.0);
		}
		else if (gridB_->GetPosition()[0] == gridD_->GetPosition()[0] && gridA_->GetPosition()[0] > gridB_->GetPosition()[0])
		{
			moveTwoActorsByX(gridB_, gridD_, 2.0);
		}

		else if (gridB_->GetPosition()[1] == gridD_->GetPosition()[1] && gridA_->GetPosition()[1] < gridB_->GetPosition()[1])
		{
			moveTwoActorsByY(gridB_, gridD_, -2.0);
		}
		else if (gridB_->GetPosition()[1] == gridD_->GetPosition()[1] && gridA_->GetPosition()[1] > gridB_->GetPosition()[1])
		{
			moveTwoActorsByY(gridB_, gridD_, 2.0);
		}
	} 

	else if (isPresentInPair(gridA_, gridD_))
	{
		if (gridB_->GetPosition()[0] == gridC_->GetPosition()[0] && gridA_->GetPosition()[0] < gridB_->GetPosition()[0])
		{
			moveTwoActorsByX(gridB_, gridC_, -2.0);
		}
		else if (gridB_->GetPosition()[0] == gridC_->GetPosition()[0] && gridA_->GetPosition()[0] > gridB_->GetPosition()[0])
		{
			moveTwoActorsByX(gridB_, gridC_, 2.0);
		}

		else if (gridB_->GetPosition()[1] == gridC_->GetPosition()[1] && gridA_->GetPosition()[1] < gridB_->GetPosition()[1])
		{
			moveTwoActorsByY(gridB_, gridC_, -2.0);
		}
		else if (gridB_->GetPosition()[1] == gridC_->GetPosition()[1] && gridA_->GetPosition()[1] > gridB_->GetPosition()[1])
		{
			moveTwoActorsByY(gridB_, gridC_, 2.0);
		}
	}

	else if (isPresentInPair(gridB_, gridC_))
	{
		if (gridA_->GetPosition()[0] == gridD_->GetPosition()[0] && gridB_->GetPosition()[0] < gridA_->GetPosition()[0]) 
		{
			moveTwoActorsByX(gridA_, gridD_, -2.0);

		} else if (gridA_->GetPosition()[0] == gridD_->GetPosition()[0] && gridB_->GetPosition()[0] > gridA_->GetPosition()[0])
		{
			moveTwoActorsByX(gridA_, gridD_, 2.0);

		} else if (gridA_->GetPosition()[1] == gridD_->GetPosition()[1] && gridB_->GetPosition()[1] < gridA_->GetPosition()[1])
		{
			moveTwoActorsByY(gridA_, gridD_, -2.0);
		}
		else if (gridA_->GetPosition()[1] == gridD_->GetPosition()[1] && gridB_->GetPosition()[1] > gridA_->GetPosition()[1])
		{
			moveTwoActorsByY(gridA_, gridD_, 2.0);
		}
	}

	else if (isPresentInPair(gridB_, gridD_))
	{
		if (gridA_->GetPosition()[0] == gridC_->GetPosition()[0] && gridB_->GetPosition()[0] < gridA_->GetPosition()[0])
		{
			moveTwoActorsByX(gridA_, gridC_, -2.0);
		} else if (gridA_->GetPosition()[0] == gridC_->GetPosition()[0] && gridB_->GetPosition()[0] > gridA_->GetPosition()[0])
		{
			moveTwoActorsByX(gridA_, gridC_, 2.0);

		} else if (gridA_->GetPosition()[1] == gridC_->GetPosition()[1] && gridB_->GetPosition()[1] < gridA_->GetPosition()[1])
		{
			moveTwoActorsByY(gridA_, gridC_, -2.0);

		} else if (gridA_->GetPosition()[1] == gridC_->GetPosition()[1] && gridB_->GetPosition()[1] > gridA_->GetPosition()[1])
		{
			moveTwoActorsByY(gridA_, gridC_, 2.0);
		}

	}

	else if (isPresentInPair(gridC_, gridD_))
	{
		if (gridA_->GetPosition()[0] == gridB_->GetPosition()[0] && gridD_->GetPosition()[0] < gridA_->GetPosition()[0])
		{
			moveTwoActorsByX(gridA_, gridB_, -2.0);
		}
		else if (gridA_->GetPosition()[0] == gridB_->GetPosition()[0] && gridD_->GetPosition()[0] > gridA_->GetPosition()[0])
		{
			moveTwoActorsByX(gridA_, gridB_, 2.0);
		}
		else if (gridA_->GetPosition()[1] == gridB_->GetPosition()[1] && gridD_->GetPosition()[1] < gridA_->GetPosition()[1])
		{
			moveTwoActorsByY(gridA_, gridB_, -2.0);
		}
		else if (gridA_->GetPosition()[1] == gridB_->GetPosition()[1] && gridD_->GetPosition()[1] > gridA_->GetPosition()[1])
		{
			moveTwoActorsByY(gridA_, gridB_, 2.0);
		}
	}*/
}


int GridStyleFour::getCurrentState(Grid* gridA, Grid* gridB, Grid* gridC, Grid* gridD)
{
	int state = 0;
	
	double xA = gridA->GetPosition()[0];
	double yA = gridA->GetPosition()[1];

	double xB = gridB->GetPosition()[0];
	double yB = gridB->GetPosition()[1];

	double xC = gridC->GetPosition()[0];
	double yC = gridC ->GetPosition()[1];

	double xD = gridD->GetPosition()[0];
	double yD = gridD->GetPosition()[1];

	
	if (xA == xB && yA == yC && xA + 1.0 == xD && yA + 1.0 == yD) {
		state = 1;
	} 
	else if (xA == xC && yA == yB && xA + 1.0 == xD && yA + 1.0 == yD) {
		state = 2;
	} 
	else if (xA == xD && yA == yB && xA + 1.0 == xC && yA + 1.0 == yC) {
		state = 3;
	} 
	else if (xA == xB && yA == yD && xA + 1.0 == xC && yA + 1.0 == yC) {
		state = 4;
	} 
	else if (xA == xC && yA == yD && xA + 1.0 == xB && yA + 1.0 == yB) {
		state = 5;
	} 
	else if (xA == xD && yA == yC && xA + 1.0 == xB && yA + 1.0 == yB) {
		state = 6;
	} 
	else if (xA == xC && yA == yB && xA + 1.0 == xD && yA - 1.0 == yD) {
		state = 7;
	} 
	else if (xA == xB && yA == yC && xA + 1.0 == xD && yA - 1.0 == yD) {
		state = 8;
	}  
	else if (xA == xB && yA == yD && xA + 1.0 == xC && yA - 1.0 == yC) {
		state = 9;
	} 
	else if (xA == xD && yA == yD && xA + 1.0 == xC && yA - 1.0 == yC) {
		state = 10;
	}
	else if (xA == xC && yA == yD && xA + 1.0 == xB && yA - 1.0 == yB) {
		state = 11;
	}
	else if (xA == xD && yA == yC && xA + 1.0 == xB && yA - 1.0 == yB) {
		state = 12;
	}
	else if (xA == xC && yA == yB && xA - 1.0 == xD && yA - 1.0 == yD) {
		state = 13;
	} 
	else if (xA == xB && yA == yC && xA - 1.0 == xD && yA - 1.0 == yD) {
		state = 14;
	}
	else if (xA == xC && yA == yD && xA - 1.0 == xB && yA - 1.0 == yB) {
		state = 15;
	}
	else if (xA == xD && yA == yC && xA - 1.0 == xB && yA - 1.0 == yB) {
		state = 16;
	}
	else if (xA == xB && yA == yD && xA - 1.0 == xC && yA - 1.0 == yC) {
		state = 17;
	}
	else if (xA == xD && yA == yB && xA - 1.0 == xC && yA - 1.0 == yC) {
		state = 18;
	}
	else if (xA == xC && yA == yB && xA - 1.0 == xD && yA + 1.0 == yD) {
		state = 19;
	}
	else if (xA == xB && yA == yC && xA - 1.0 == xD && yA + 1.0 == yD) {
		state = 20;
	}
	else if (xA == xB && yA == yD && xA - 1.0 == xC && yA + 1.0 == yC) {
		state = 21;
	}
	else if (xA == xD && yA == yB && xA - 1.0 == xC && yA + 1.0 == yC) {
		state = 22;
	}
	else if (xA == xC && yA == yD && xA - 1.0 == xB && yA + 1.0 == yB) {
		state = 23;
	}
	else if (xA == xD && yA == yC && xA - 1.0 == xB && yA + 1.0 == yB) {
		state = 24;
	}


	return state;

}


bool GridStyleFour::isPair(Grid* gridA, Grid* gridB)
{	
	bool flag = false;

	if (gridA->GetPosition()[0] == gridB->GetPosition()[0] || gridA->GetPosition()[1] == gridB->GetPosition()[1]) 
	{
		flag = true;
	}
	return flag;
}

bool GridStyleFour::isPresentInPair(Grid* gridA, Grid* gridB)
{
	bool flag = false;
	
	if (!isPair(gridA, gridB)) 
	{
		return flag;
	}
	else 
	{
		if (isPresentInOne(gridA) || isPresentInOne(gridB)) 
		{
			return flag;
		}
		else
		{
			double xCam = camera_->GetPosition()[0];
			double yCam = camera_->GetPosition()[1];

			double AX = gridA->GetPosition()[0];
			double AY = gridA->GetPosition()[1];

			double BX = gridB->GetPosition()[0];
			double BY = gridB->GetPosition()[1];

			/*if (AX < BX) 
			{
				if (viewPointX - 0.16 > AX && viewPointX + 0.16 < BX + 1.0 && viewPointY + 0.16 < AY + 1.0 && viewPointY - 0.16 > AY)
				{
						flag = true;
				}
			}
			else if (AY < BY) 
			{
				if (viewPointY - 0.16 > AY && viewPointY + 0.16 < BY + 1.0 && viewPointX + 0.16 < AX + 1.0 && viewPointX - 0.16 > AX)
				{
					flag = true;
				}


			} else if (AX > BX)
			{
				if (viewPointX - 0.16 > BX && viewPointX + 0.16 < AX + 1.0 && viewPointY + 0.16 < BY + 1.0 && viewPointY - 0.16 > BY)
				{
						flag = true;
				}

			}
			else if (AY > BY)
			{
				if (viewPointY - 0.16 > BY && viewPointY + 0.16 < AY + 1.0 && viewPointX + 0.16 < BX + 1.0 && viewPointX - 0.16 > BX)
				{
					flag = true;
				}

			}*/
			if (AX == BX) {
				if (abs(AX - (xCam - 0.16)) < 0.15 || abs((AX + 1.0) - (xCam + 0.16)) < 0.15) {
					flag = true;
				}
			}
			else if (AY == BY) {
				if (abs(AY - (yCam - 0.16)) < 0.15 || abs((AY + 1.0) - (yCam + 0.16)) < 0.15) {
					flag = true;
				}
			}
			
		}
	}

	return flag;
}

void GridStyleFour::moveTwoActorsByX(Grid* gridA, Grid* gridB, double d)
{
	gridA->SetPosition(gridA->GetPosition()[0] + d, gridA->GetPosition()[1]);
	gridB->SetPosition(gridB->GetPosition()[0] + d, gridB->GetPosition()[1]);

	cout << gridA->GetPosition()[0] << "   " << gridA->GetPosition()[1] << endl;
	cout << gridB->GetPosition()[0] << "   " << gridB->GetPosition()[1] << endl;
}

void GridStyleFour::moveTwoActorsByY(Grid* gridA, Grid* gridB, double d)
{
	gridA->SetPosition(gridA->GetPosition()[0], gridA->GetPosition()[1] + d);
	gridB->SetPosition(gridB->GetPosition()[0], gridB->GetPosition()[1] + d);

	cout << gridA->GetPosition()[0] << " ---------------  " << gridA->GetPosition()[1] << endl;
	cout << gridB->GetPosition()[0] << " ---------------  " << gridB->GetPosition()[1] << endl;
}

bool GridStyleFour::isPresentInOne(Grid* grid) 
{
	double x = grid->GetPosition()[0];
	double y = grid->GetPosition()[1];

	double xCam = camera_->GetPosition()[0];
	double yCam = camera_->GetPosition()[1];

	bool flag = false;
	if ((std::abs(x - (xCam - 0.16)) < 0.15 and abs(y - (yCam - 0.16)) < 0.15 ) || 
		(std::abs(x - (xCam - 0.16)) < 0.15 and abs((y + 1.0) - (yCam + 0.16)) < 0.15) || 
		(std::abs((x + 1.0) - (xCam + 0.16)) < 0.15 and abs((y + 1.0) - (yCam + 0.16)) < 0.15) || 
		(std::abs((x + 1.0) - (xCam + 0.16)) < 0.15 and abs(y - (yCam - 0.16)) < 0.15))
	{
		flag = true;
	}

	return flag;
	
}


//(+x, +y)
void GridStyleFour::moveToNorthEast(Grid* gridA, Grid* gridB, Grid* gridC)
{
	double xA = gridA->GetPosition()[0];
	double yA = gridA->GetPosition()[1];
	gridA->SetPosition(xA + 1.0, yA + 1.0);

	double xB = gridB->GetPosition()[0];
	double yB = gridB->GetPosition()[1];
	gridB->SetPosition(xB + 2.0, yB + 2.0);

	double xC = gridC->GetPosition()[0];
	double yC = gridC->GetPosition()[1];
	gridC->SetPosition(xC + 1.0, yC + 1.0);

}
//(-x, +y)
void GridStyleFour::moveToNorthWest(Grid* gridA, Grid* gridB, Grid* gridC)
{
	double xA = gridA->GetPosition()[0];
	double yA = gridA->GetPosition()[1];
	gridA->SetPosition(xA - 1.0, yA + 1.0);

	double xB = gridB->GetPosition()[0];
	double yB = gridB->GetPosition()[1];
	gridB->SetPosition(xB - 2.0, yB + 2.0);

	double xC = gridC->GetPosition()[0];
	double yC = gridC->GetPosition()[1];
	gridC->SetPosition(xC - 1.0, yC + 1.0);
}
//(-x, -y)
void GridStyleFour::moveToSouthWest(Grid* gridA, Grid* gridB, Grid* gridC)
{
	double xA = gridA->GetPosition()[0];
	double yA = gridA->GetPosition()[1];
	gridA->SetPosition(xA - 1.0, yA - 1.0);

	double xB = gridB->GetPosition()[0];
	double yB = gridB->GetPosition()[1];
	gridB->SetPosition(xB - 2.0, yB - 2.0);

	double xC = gridC->GetPosition()[0];
	double yC = gridC->GetPosition()[1];
	gridC->SetPosition(xC - 1.0, yC - 1.0);
}
//(+x, -y)
void GridStyleFour::moveToSouthEast(Grid* gridA, Grid* gridB, Grid* gridC)
{
	double xA = gridA->GetPosition()[0];
	double yA = gridA->GetPosition()[1];
	gridA->SetPosition(xA + 1.0, yA - 1.0);

	double xB = gridB->GetPosition()[0];
	double yB = gridB->GetPosition()[1];
	gridB->SetPosition(xB + 2.0, yB - 2.0);

	double xC = gridC->GetPosition()[0];
	double yC = gridC->GetPosition()[1];
	gridC->SetPosition(xC + 1.0, yC - 1.0);
}