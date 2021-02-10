#include "UserEvents.h"

vtkStandardNewMacro(UserEvents);

UserEvents::UserEvents() {

}

void UserEvents::OnLeftDoubleClick() {
    FindPokedRenderer(Interactor->GetEventPosition()[0], Interactor->GetEventPosition()[1]);
    if (CurrentRenderer == nullptr) {
        return;
    }

    GrabFocus(this->EventCallbackCommand);
}

void UserEvents::OnLeftButtonDown() {
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

void UserEvents::OnLeftButtonUp() {
    EndTimer();
    if (Interactor) {
        ReleaseFocus();
    }
}


void UserEvents::OnTimer() {
    if (CurrentRenderer == nullptr) {
        return;
    }

    Scrolling();
}