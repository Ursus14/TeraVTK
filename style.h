#pragma once

#include < vtkInteractorStyleTrackballCamera.h>

class MouseMovemenetStyle : public  vtkInteractorStyleTrackballCamera
{

	virtual void OnLeftButtonDown();
	virtual void OnLeftButtonUp();
	
	virtual void OnMouseWheelForward();
	virtual void OnMouseWheelBackward();

	virtual void OnRightButtonDown();
	virtual void OnRightButtonUp();

	virtual void OnTimer();
};

