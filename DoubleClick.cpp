#include "DoubleClick.h"

vtkStandardNewMacro(DoubleClick);

DoubleClick::DoubleClick() {

}

void DoubleClick::OnLeftDoubleClick() {
    FindPokedRenderer(Interactor->GetEventPosition()[0], Interactor->GetEventPosition()[1]);
    if (CurrentRenderer == nullptr) {
        return;
    }

    GrabFocus(this->EventCallbackCommand);
}

void DoubleClick::OnLeftButtonDown() { 
    FindPokedRenderer(Interactor->GetEventPosition()[0], Interactor->GetEventPosition()[1]);
    if (CurrentRenderer == nullptr) {
        return;
    }

    GrabFocus(EventCallbackCommand);

    SetTimerDuration(1);
    UseTimersOn();
    StartTimer();

    //----------------------
    NumberOfClicks++;

    if (NumberOfClicks == 1) {
        start = std::chrono::system_clock::now();
    }
    if (NumberOfClicks == 2) {
        end = std::chrono::system_clock::now();
    }

    int pickPosition[2];
    this->GetInteractor()->GetEventPosition(pickPosition);

    int xdist = pickPosition[0] - this->PreviousPosition[0];
    int ydist = pickPosition[1] - this->PreviousPosition[1];

    this->PreviousPosition[0] = pickPosition[0];
    this->PreviousPosition[1] = pickPosition[1];

    int moveDistance = (int)(xdist * xdist + ydist * ydist);
    if (abs(std::chrono::duration<double>(end - start).count()) > doubleClickTimeLimit) {
        NumberOfClicks = 1;
        start = std::chrono::system_clock::now();
    }
    if (moveDistance > (ResetPixelDistance * ResetPixelDistance)) {
        NumberOfClicks = 1;
    }
    if (NumberOfClicks == 2) {
        OnLeftDoubleClick();
        NumberOfClicks = 0;
        EndTimer();
    }
}

void DoubleClick::OnLeftButtonUp() {
    EndTimer();
    if (Interactor) {
        ReleaseFocus();
    }
}


void DoubleClick ::OnTimer() {
    if (CurrentRenderer == nullptr) {
        return;
    }

    Scrolling();
}