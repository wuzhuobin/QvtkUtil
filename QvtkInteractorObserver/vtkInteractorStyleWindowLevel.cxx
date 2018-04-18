#include "vtkInteractorStyleWindowLevel.h"

// vtk
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkCallbackCommand.h>

vtkInteractorStyleWindowLevel::vtkInteractorStyleWindowLevel()
	:WindowLevelCurrentPosition{0,0}, 
	WindowLevelStartPosition{0, 0}
{
	this->InitialWindow = 0;
	this->InitialLevel = 0;

}

vtkInteractorStyleWindowLevel::~vtkInteractorStyleWindowLevel()
{
}

void vtkInteractorStyleWindowLevel::OnMouseMove()
{
	Superclass::OnMouseMove();
	int x = this->Interactor->GetEventPosition()[0];
	int y = this->Interactor->GetEventPosition()[1];
	switch (this->State)
	{
	case VTKIS_WINDOW_LEVEL:
		this->FindPokedRenderer(x, y);
		this->WindowLevel();
		this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
		break;
	default:
		break;
	}
}

void vtkInteractorStyleWindowLevel::OnLeftButtonDown()
{
	this->FindPokedRenderer(this->Interactor->GetEventPosition()[0],
		this->Interactor->GetEventPosition()[1]);
	if (this->CurrentRenderer == nullptr)
	{
		return;
	}
	this->GrabFocus(this->EventCallbackCommand);
	StartWindowLevel();
}

void vtkInteractorStyleWindowLevel::OnLeftButtonUp()
{
	switch (this->State)
	{
	case VTKIS_WINDOW_LEVEL:
		this->ReleaseFocus();
		this->EndWindowLevel();
	default:
		break;
	}
	Superclass::OnLeftButtonUp();
}

void vtkInteractorStyleWindowLevel::OnChar()
{
	char key = this->Interactor->GetKeyCode();
	switch (key)
	{
	case 'r':
	case 'R':
		ResetWindowLevel();
		break;
	default:
		Superclass::OnChar();
		break;
	}
}

void vtkInteractorStyleWindowLevel::StartWindowLevel()
{
	if (this->State != VTKIS_NONE) {
		return;
	}
	this->StartState(VTKIS_WINDOW_LEVEL);
	vtkRenderWindowInteractor *rwi = this->Interactor;

	this->WindowLevelStartPosition[0] = rwi->GetEventPosition()[0];
	this->WindowLevelStartPosition[1] = rwi->GetEventPosition()[1];

	this->InitialWindow = this->GetWindow();
	this->InitialLevel = this->GetLevel();
}

void vtkInteractorStyleWindowLevel::WindowLevel()
{
	int *size = this->Interactor->GetRenderWindow()->GetSize();
	double window = this->InitialWindow;
	double level = this->InitialLevel;

	// Compute normalized delta
	vtkRenderWindowInteractor *rwi = this->Interactor;

	this->WindowLevelCurrentPosition[0] = rwi->GetEventPosition()[0];
	this->WindowLevelCurrentPosition[1] = rwi->GetEventPosition()[1];

	double dx = 4.0 *
		(this->WindowLevelCurrentPosition[0] -
			this->WindowLevelStartPosition[0]) / size[0];
	double dy = 4.0 *
		(this->WindowLevelStartPosition[1] -
			this->WindowLevelCurrentPosition[1]) / size[1];

	// Scale by current values

	if (fabs(window) > 0.01)
	{
		dx = dx * window;
	}
	else
	{
		dx = dx * (window < 0 ? -0.01 : 0.01);
	}
	if (fabs(level) > 0.01)
	{
		dy = dy * level;
	}
	else
	{
		dy = dy * (level < 0 ? -0.01 : 0.01);
	}

	// Abs so that direction does not flip

	if (window < 0.0)
	{
		dx = -1 * dx;
	}
	if (level < 0.0)
	{
		dy = -1 * dy;
	}

	// Compute new window level

	double newWindow = dx + window;
	double newLevel;
	newLevel = level - dy;

	// Stay away from zero and really

	if (fabs(newWindow) < 0.01)
	{
		newWindow = 0.01*(newWindow < 0 ? -1 : 1);
	}

	if (fabs(newLevel) < 0.01)
	{
		newLevel = 0.01*(newLevel < 0 ? -1 : 1);
	}
	SetWindow(newWindow);
	SetLevel(newLevel);

	this->Interactor->Render();
}

void vtkInteractorStyleWindowLevel::EndWindowLevel()
{
	if (this->State != VTKIS_WINDOW_LEVEL)
	{
		return;
	}
	this->StopState();
}

void vtkInteractorStyleWindowLevel::ResetWindowLevel()
{
	SetWindow(GetResetWindow());
	SetLevel(GetResetLevel());
}

void vtkInteractorStyleWindowLevel::PrintSelf(ostream & os, vtkIndent indent)
{
	Superclass::PrintSelf(os, indent);
}
