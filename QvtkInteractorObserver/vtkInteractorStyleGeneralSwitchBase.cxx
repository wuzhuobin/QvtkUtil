/*
Author:		Wong, Matthew Lun
Date:		16th, June 2016
Occupation:	Chinese University of Hong Kong,
Department of Imaging and Inteventional Radiology,
Junior Research Assistant
This class is based on vtkInteractorStyleSwitch, written to allow easy
switching between 2D interactors.
Wong Matthew Lun
Copyright (C) 2016
*/
#include "vtkInteractorStyleGeneralSwitchBase.h"

#include <vtkCommand.h>
#include <vtkCallbackCommand.h>	
#include <vtkObjectFactory.h>
#include <vtkInteractorStyle.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

vtkInteractorStyleGeneralSwitchBase::vtkInteractorStyleGeneralSwitchBase()
{
	NEW_INTERACTOR_STYLE(InteractorStyleTesting, vtkInteractorStyleSwitch);
	this->CurrentStyle = 0;
}

vtkInteractorStyleGeneralSwitchBase::~vtkInteractorStyleGeneralSwitchBase()
{
	/* Do not modifiy the lower code£¡ */
	for (std::vector<vtkInteractorStyle*>::iterator it = AllStyles.begin();
		it != AllStyles.end(); ++it) {
		if ((*it) != NULL) {
			(*it)->Delete();
			(*it) = NULL;
		}
	}
	AllStyles.clear();

}

void vtkInteractorStyleGeneralSwitchBase::SetAutoAdjustCameraClippingRange(int value)
{
	if (value == this->AutoAdjustCameraClippingRange)
	{
		return;
	}

	if (value < 0 || value > 1)
	{
		vtkErrorMacro("Value must be between 0 and 1 for" <<
			" SetAutoAdjustCameraClippingRange");
		return;
	}

	for (std::vector<vtkInteractorStyle*>::iterator it = AllStyles.begin();
		it != AllStyles.end(); ++it) {
		(*it)->SetAutoAdjustCameraClippingRange(value);
	}
	this->Modified();
}

void vtkInteractorStyleGeneralSwitchBase::SetDefaultRenderer(vtkRenderer* renderer)
{
	vtkInteractorStyle::SetDefaultRenderer(renderer);

	for (std::vector<vtkInteractorStyle*>::const_iterator cit = AllStyles.cbegin();
		cit != AllStyles.cend(); ++cit) {
		(*cit)->SetDefaultRenderer(renderer);
	}
}

void vtkInteractorStyleGeneralSwitchBase::SetCurrentRenderer(vtkRenderer* renderer)
{
	vtkInteractorStyle::SetCurrentRenderer(renderer);

	for (std::vector<vtkInteractorStyle*>::const_iterator cit = AllStyles.cbegin();
		cit != AllStyles.cend(); ++cit) {
		(*cit)->SetCurrentRenderer(renderer);
	}
}


void vtkInteractorStyleGeneralSwitchBase::SetEnabled(int i)
{
	Superclass::SetEnabled(i);
	for (std::vector<vtkInteractorStyle*>::const_iterator cit = AllStyles.cbegin();
		cit != AllStyles.cend(); ++cit) {
		(*cit)->SetEnabled(i);
	}
}

void vtkInteractorStyleGeneralSwitchBase::PrintSelf(ostream & os, vtkIndent indent)
{
	vtkInteractorStyleSwitchBase::PrintSelf(os, indent);
}

void vtkInteractorStyleGeneralSwitchBase::SetInteractor(vtkRenderWindowInteractor *iren)
{
	if (iren == this->Interactor)
	{
		return;
	}
	// if we already have an Interactor then stop observing it
	if (this->Interactor)
	{
		this->Interactor->RemoveObserver(this->EventCallbackCommand);
	}
	this->Interactor = iren;
	// add observers for each of the events handled in ProcessEvents
	if (iren)
	{
		iren->AddObserver(vtkCommand::CharEvent,
			this->EventCallbackCommand,
			this->Priority);

		iren->AddObserver(vtkCommand::DeleteEvent,
			this->EventCallbackCommand,
			this->Priority);
	}
}

