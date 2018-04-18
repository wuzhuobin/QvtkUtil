#include "vtkInteractorStyleNavigation.h"


// vtk
#include <vtkRenderWindowInteractor.h>
#include <vtkAbstractPicker.h>
#include <vtkCallbackCommand.h>
#include <vtkCamera.h>
#include <vtkRenderer.h>

void vtkInteractorStyleNavigation::PrintSelf(ostream & os, vtkIndent indent)
{
	Superclass::PrintSelf(os, indent);
}

void vtkInteractorStyleNavigation::OnMouseMove()
{
	Superclass::OnMouseMove();
	int x = this->Interactor->GetEventPosition()[0];
	int y = this->Interactor->GetEventPosition()[1];

	switch (this->State)
	{
	case VTKIS_NAVIGATION:
		this->FindPokedRenderer(x, y);
		this->Navigation();
		this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
		break;
	}
}

void vtkInteractorStyleNavigation::OnLeftButtonDown()
{
	this->NumberOfLeftClicks++;
	int pickPosition[2];
	this->GetInteractor()->GetEventPosition(pickPosition);

	int xdist = pickPosition[0] - this->PreviousLeftPosition[0];
	int ydist = pickPosition[1] - this->PreviousLeftPosition[1];

	this->PreviousLeftPosition[0] = pickPosition[0];
	this->PreviousLeftPosition[1] = pickPosition[1];

	int moveDistance = (int)sqrt((double)(xdist*xdist + ydist*ydist));

	if (moveDistance > this->RESET_PIXEL_DISTANCE )
	{
		this->NumberOfLeftClicks = 1;
	}


	if (this->NumberOfLeftClicks == 2 )
	{
		Superclass::OnLeftButtonDown();
		this->NumberOfLeftClicks = 0;
	}
	else 
	{

		this->FindPokedRenderer(this->Interactor->GetEventPosition()[0],
			this->Interactor->GetEventPosition()[1]);
		if (this->CurrentRenderer == NULL)
		{
			return;
		}

		this->GrabFocus(this->EventCallbackCommand);
		this->StartNavigation();

	}

}

void vtkInteractorStyleNavigation::OnLeftButtonUp()
{
	switch (this->State)
	{
	case VTKIS_NAVIGATION:
		this->ReleaseFocus();
		this->EndNavigation();
		break;
	}
	Superclass::OnLeftButtonUp();
}

void vtkInteractorStyleNavigation::StartNavigation()
{
	if (this->State != VTKIS_NONE) {
		return;
	}
	this->StartState(VTKIS_NAVIGATION);

}

void vtkInteractorStyleNavigation::EndNavigation()
{
	if (this->State != VTKIS_NAVIGATION) {
		return;
	}
	this->StopState();
}

void vtkInteractorStyleNavigation::Navigation()
{
	double index[3];
	if (CalculateIndex(index)) {
		SetCursorPosition(index);
	}
}

void vtkInteractorStyleNavigation::Dolly()
{
	Superclass::Dolly();

	if (this->CurrentRenderer == NULL)
	{
		return;
	}

	vtkCamera* camera = this->CurrentRenderer->GetActiveCamera();
	if (camera->GetParallelProjection()) {
		double scale = camera->GetParallelScale();
		SetCameraScale(scale);
	}
}

void vtkInteractorStyleNavigation::SetCursorPosition(const double index[3])
{
	SetCursorPosition(index[0], index[1], index[2]);
}

void vtkInteractorStyleNavigation::SetCameraScale(double scale)
{
	if (this->CurrentRenderer == NULL)
	{
		return;
	}

	vtkCamera* camera = this->CurrentRenderer->GetActiveCamera();
	if (camera->GetParallelProjection() && 
		scale != camera->GetParallelScale()) {
		camera->SetParallelScale(scale);
	}
	this->Interactor->Render();

}

vtkInteractorStyleNavigation::vtkInteractorStyleNavigation()
{
	this->PreviousLeftPosition[0] = 0;
	this->PreviousLeftPosition[1] = 0;
}

vtkInteractorStyleNavigation::~vtkInteractorStyleNavigation()
{
}

bool vtkInteractorStyleNavigation::CalculateIndex(double index[3])
{
	//Pick
	int picked = this->GetInteractor()->GetPicker()->Pick(
		this->GetInteractor()->GetEventPosition()[0],
		this->GetInteractor()->GetEventPosition()[1],
		0,  // always zero.
		this->GetCurrentRenderer());

	this->GetInteractor()->GetPicker()->GetPickPosition(index);
	return picked != 0;

}
