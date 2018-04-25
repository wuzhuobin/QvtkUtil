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


#ifndef __VTK_INTERACTOR_STYLE_GENERAL_SWITCH_BASE_H__
#define __VTK_INTERACTOR_STYLE_GENERAL_SWITCH_BASE_H__

// me 
#include "QVTKINTERACTOROBSERVER_EXPORT.h"

// vtk
#include <vtkSetGet.h>
#include <vtkInteractorStyleSwitchBase.h>
#include <vtkInteractorStyleSwitch.h>

// std
#include <vector>

#define CURRENT_STYLE_MACRO(style) \
bool CurrentStyleIs##style () {\
	return CurrentStyle == style; \
}

#define SET_INTERACTOR_STYLE_MACRO(style) \
void SetInteractorStyleTo##style () { \
	if (!CurrentStyleIs##style()) { \
		if (this->CurrentStyle) { \
			this->InternalUpdate(false);\
			this->CurrentStyle->SetInteractor(0); \
	} \
		this->CurrentStyle = this->style;\
	} \
	if (this->CurrentStyle) { \
		this->CurrentStyle->SetInteractor(this->Interactor);\
		this->CurrentStyle->SetTDxStyle(this->TDxStyle);\
		this->InternalUpdate(true);\
	} \
}

#define SETUP_INTERACTOR_STYLE(NAME, CLASS) \
vtkGetObjectMacro(NAME, CLASS); \
SET_INTERACTOR_STYLE_MACRO(NAME); \
CURRENT_STYLE_MACRO(NAME); 

#define NEW_INTERACTOR_STYLE(NAME, CLASS) \
NAME = CLASS::New(); \
AllStyles.push_back(NAME); 

class QVTKINTERACTOROBSERVER_EXPORT vtkInteractorStyleGeneralSwitchBase : public vtkInteractorStyleSwitchBase
{
public:
	vtkTypeMacro(vtkInteractorStyleGeneralSwitchBase, vtkInteractorStyleSwitchBase);
	virtual void PrintSelf(ostream& os, vtkIndent indent);


	SETUP_INTERACTOR_STYLE(InteractorStyleTesting, vtkInteractorStyleSwitch);

	virtual void SetInteractor(vtkRenderWindowInteractor *iren) override;

	virtual void SetDefaultRenderer(vtkRenderer* renderer) override;
	virtual void SetCurrentRenderer(vtkRenderer* renderer) override;


	virtual void SetEnabled(int) override;

protected:
	vtkInteractorStyleGeneralSwitchBase();
	virtual ~vtkInteractorStyleGeneralSwitchBase();

	virtual void InternalUpdate(bool flag) = 0;
	void SetAutoAdjustCameraClippingRange(int value);


	vtkInteractorStyleSwitch* InteractorStyleTesting;
	vtkInteractorStyle*	CurrentStyle;
	std::vector<vtkInteractorStyle*> AllStyles;
};



#endif
