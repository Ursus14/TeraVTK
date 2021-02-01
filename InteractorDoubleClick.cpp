#include "InteractorDoubleClick.h"

vtkStandardNewMacro(InteractorDoubleClick);

InteractorDoubleClick::InteractorDoubleClick()
{

}

void InteractorDoubleClick::OnLeftDoubleClick()
{
    FindPokedRenderer(
        Interactor->GetEventPosition()[0],Interactor->GetEventPosition()[1]);
    if (CurrentRenderer == nullptr)
    {
        return;
    }

    GrabFocus(EventCallbackCommand);
}

void InteractorDoubleClick::OnLeftButtonDown()
{ 
    NumberOfClicks++;

    if (NumberOfClicks == 1)
        start = std::chrono::system_clock::now();
    if (NumberOfClicks == 2)
        end = std::chrono::system_clock::now();

    int pickPosition[2];
    this->GetInteractor()->GetEventPosition(pickPosition);

    int xdist = pickPosition[0] - this->PreviousPosition[0];
    int ydist = pickPosition[1] - this->PreviousPosition[1];

    this->PreviousPosition[0] = pickPosition[0];
    this->PreviousPosition[1] = pickPosition[1];

    int moveDistance = (int)(xdist * xdist + ydist * ydist);
    if (abs(std::chrono::duration<double>(end - start).count()) > doubleClickTimeLimit)
    {
        NumberOfClicks = 1;
        start = std::chrono::system_clock::now();
    }
    if (moveDistance > (ResetPixelDistance * ResetPixelDistance))
    {
        NumberOfClicks = 1;
    }
    if (NumberOfClicks == 2)
    {
        OnLeftDoubleClick();
        NumberOfClicks = 0;
        return;
    }
}
