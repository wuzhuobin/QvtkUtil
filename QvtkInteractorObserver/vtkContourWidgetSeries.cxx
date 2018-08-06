#include "vtkContourWidgetSeries.h"
#include "vtkContourWidgetSeriesRepresentation.h"
#include "vtkContourWidgetSeriesOrientedGlyphRepresentation.h"

#include <vtkObjectFactory.h>
#include <vtkContourWidget.h>
#include <vtkRenderWindow.h>
#include <vtkWidgetCallbackMapper.h>
#include <vtkWidgetEvent.h>
//#include <vtkCommand.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCallbackCommand.h>
#include <vtkContourRepresentation.h>
#include <vtkEvent.h>

#include <list>
#include <iterator>
#include <algorithm>
#include <functional>

vtkStandardNewMacro(vtkContourWidgetSeries);

class vtkContourList: public std::list<vtkContourWidget*>{};
typedef std::list<vtkContourWidget*>::iterator vtkContourListIterator;

vtkContourWidget * vtkContourWidgetSeries::CreateNewContourWidget()
{
	vtkContourWidgetSeriesRepresentation *rep =
		vtkContourWidgetSeriesRepresentation::SafeDownCast(this->WidgetRep);
	if (!rep)
	{
		vtkErrorMacro(<< "Please set, or create a default vtkContourWidgetSeriesRepresentation "
			<< "before adding requesting creation of a new contour.");
		return NULL;
	}

	// Create the contour widget or reuse an old one
	int currentHandleNumber = static_cast<int>(this->Contours->size());
	vtkContourWidget *widget = vtkContourWidget::New();
	// Configure the contour widget
	widget->SetAllowNodePicking(AllowNodePicking);
	widget->SetContinuousDraw(ContinuousDraw);
	widget->SetFollowCursor(FollowCursor);
	widget->SetParent(this);
	widget->SetInteractor(this->Interactor);
	vtkContourRepresentation *contourRep = rep->GetContourRepresentation(currentHandleNumber);
	if (!contourRep)
	{
		widget->Delete();
		return NULL;
	}
	else
	{
		contourRep->SetRenderer(this->CurrentRenderer);
		widget->SetRepresentation(contourRep);

		// Now place the widget into the list of contour widgets (if not already there)
		this->Contours->push_back(widget);
		return widget;
	}
}

void vtkContourWidgetSeries::DeleteContour(int i)
{
	if (this->Contours->size() <= static_cast< size_t >(i))
	{
		return;
	}

	vtkContourWidgetSeriesRepresentation *rep =
		static_cast<vtkContourWidgetSeriesRepresentation*>(this->WidgetRep);
	if (rep)
	{
		rep->RemoveContour(i);
	}

	vtkContourListIterator iter = this->Contours->begin();
	std::advance(iter, i);
	(*iter)->SetEnabled(0);
	(*iter)->RemoveObservers(vtkCommand::StartInteractionEvent);
	(*iter)->RemoveObservers(vtkCommand::InteractionEvent);
	(*iter)->RemoveObservers(vtkCommand::EndInteractionEvent);
	vtkContourWidget * w = (*iter);
	this->Contours->erase(iter);
	w->Delete();
}

vtkContourWidget * vtkContourWidgetSeries::GetContour(int i)
{
	if (this->Contours->size() <= static_cast< size_t >(i))
	{
		return NULL;
	}
	vtkContourListIterator iter = this->Contours->begin();
	std::advance(iter, i);
	return *iter;
}

void vtkContourWidgetSeries::ResetContours()
{
	this->WidgetState = START;
	while (this->Contours->size() != 0)
	{
		this->DeleteContour(this->Contours->size() - 1);
	}
	this->Render();
}

vtkContourWidgetSeries::vtkContourWidgetSeries()
{
	this->ManagesCursor = 1;
	this->WidgetState = vtkContourWidgetSeries::START;

	// The list for containing contours.
	this->Contours = new vtkContourList;

	// These are the event callbacks supported by this widget
	this->CallbackMapper->SetCallbackMethod(vtkCommand::LeftButtonPressEvent,
		vtkWidgetEvent::AddPoint,
		this, vtkContourWidgetSeries::AddContourAction);
	this->CallbackMapper->SetCallbackMethod(vtkCommand::RightButtonPressEvent,
		vtkWidgetEvent::Completed,
		this, vtkContourWidgetSeries::CompletedAction);
	this->CallbackMapper->SetCallbackMethod(vtkCommand::MouseMoveEvent,
		vtkWidgetEvent::Move,
		this, vtkContourWidgetSeries::MoveAction);
	this->CallbackMapper->SetCallbackMethod(vtkCommand::LeftButtonReleaseEvent,
		vtkWidgetEvent::EndSelect,
		this, vtkContourWidgetSeries::EndSelectAction);
	this->CallbackMapper->SetCallbackMethod(vtkCommand::KeyPressEvent,
		vtkEvent::NoModifier, 127, 1, "Delete",
		vtkWidgetEvent::Delete,
		this, vtkContourWidgetSeries::DeleteAction);
	this->CallbackMapper->SetCallbackMethod(vtkCommand::KeyPressEvent,
		vtkEvent::ShiftModifier, 127, 1, "Delete",
		vtkWidgetEvent::Reset,
		this, vtkContourWidgetSeries::ResetAction);
	//this->CallbackMapper->SetCallbackMethod(vtkCommand::KeyPressEvent,
	//	vtkEvent::NoModifier, 127, 1, "Delete",
	//	vtkWidgetEvent::Delete,
	//	this, vtkContourWidgetSeries::DeleteAction);
	//this->Defining = 1;
}

vtkContourWidgetSeries::~vtkContourWidgetSeries()
{
	// Loop over all contours releasing their observers and deleting them
	while (!this->Contours->empty())
	{
		this->DeleteContour(static_cast<int>(this->Contours->size()) - 1);
	}
	delete this->Contours;
}

void vtkContourWidgetSeries::AddContourAction(vtkAbstractWidget * w)
{
	vtkContourWidgetSeries* self = reinterpret_cast<vtkContourWidgetSeries*>(w);
	// compute some info we need for all cases
	int X = self->Interactor->GetEventPosition()[0];
	int Y = self->Interactor->GetEventPosition()[1];

	int state = self->WidgetRep->ComputeInteractionState(X, Y);
	switch (self->WidgetState)
	{
	case vtkContourWidgetSeries::START:
	case vtkContourWidgetSeries::MANIPULATE:
		// if outside any contour, create a new contour
		if (state == vtkContourWidgetSeriesRepresentation::OUTSIDE)
		{
			// we are creating a new contour.
			// creat a new contour and enable it 
			self->CreateNewContourWidget()->EnabledOn();
			self->InvokeEvent(vtkCommand::StartInteractionEvent, NULL);
		}
		// place a first node
	case  vtkContourWidgetSeries::DEFINE:
		self->WidgetState = vtkContourWidgetSeries::DEFINE;

		self->InvokeEvent(vtkCommand::LeftButtonPressEvent, NULL);
		// place a node
		self->InvokeEvent(vtkCommand::InteractionEvent, NULL);
		self->EventCallbackCommand->SetAbortFlag(1);
		self->Render();
		// if the close loop, it need to re-generate a new contour
		if (self->WidgetState == DEFINE &&
			self->Contours->size() &&
			self->Contours->back()->GetWidgetState() == vtkContourWidget::Manipulate) {
			self->WidgetState = MANIPULATE;
		}
		break;
	default:
		break;
	}

}

void vtkContourWidgetSeries::CompletedAction(vtkAbstractWidget *w)
{
	vtkContourWidgetSeries *self = reinterpret_cast<vtkContourWidgetSeries*>(w);

	// Do something only if we are in the middle of placing the seeds
	if (self->WidgetState == vtkContourWidgetSeries::DEFINE)
	{
		// set to manipulate for manipulate a contour or prepare to creat a new one
		self->WidgetState = vtkContourWidgetSeries::MANIPULATE;

		// conduct to contours.
		self->InvokeEvent(vtkCommand::RightButtonPressEvent, NULL);
		self->InvokeEvent(vtkCommand::EndInteractionEvent, NULL);
		self->EventCallbackCommand->SetAbortFlag(1);
		self->Render();
	}
}

void vtkContourWidgetSeries::MoveAction(vtkAbstractWidget *w)
{
	//std::cerr << __func__ << std::endl;
	vtkContourWidgetSeries *self = reinterpret_cast<vtkContourWidgetSeries*>(w);
	//std::cerr << "Widget State " << self->WidgetState << std::endl;
	//std::cerr << "Number of contours" << self->Contours->size() << std::endl;

	// Do nothing if there are no contours
	switch (self->WidgetState)
	{
	case START:
		break;
	case MANIPULATE:
	//{
	//	int X = self->Interactor->GetEventPosition()[0];
	//	int Y = self->Interactor->GetEventPosition()[1];
	//	int state = self->WidgetRep->ComputeInteractionState(X, Y);
	//	if (state == vtkContourWidgetSeriesRepresentation::OUTSIDE) {
	//		return;
	//	}
	//}
	case DEFINE:
		// else conduct to contours
		self->InvokeEvent(vtkCommand::InteractionEvent, NULL);
		self->InvokeEvent(vtkCommand::MouseMoveEvent, NULL);
		self->EventCallbackCommand->SetAbortFlag(1);
		self->Render();
	default:
		break;
	}


}

void vtkContourWidgetSeries::EndSelectAction(vtkAbstractWidget *w)
{
	vtkContourWidgetSeries *self = reinterpret_cast<vtkContourWidgetSeries*>(w);

	// conduct to contours
	self->InvokeEvent(vtkCommand::LeftButtonReleaseEvent, NULL);
	self->InvokeEvent(vtkCommand::EndInteractionEvent, NULL);
	self->Superclass::EndInteraction();
	self->EventCallbackCommand->SetAbortFlag(1);
	self->Render();
}

void vtkContourWidgetSeries::DeleteAction(vtkAbstractWidget *w)
{
	vtkContourWidgetSeries* self = reinterpret_cast<vtkContourWidgetSeries*>(w);
	//vtkContourWidgetSeriesRepresentation* rep = self->GetvtkContourWidgetSeriesRepresentation();
	if (self->WidgetState == START) {
		return;
	}
	self->InvokeEvent(vtkCommand::KeyPressEvent, NULL);
	self->InvokeEvent(vtkCommand::InteractionEvent, NULL);
	self->EventCallbackCommand->SetAbortFlag(1);

	// find out all contours whose number of node is 0
	std::list<int> nullContour;
	int index = 0;
	std::for_each(self->Contours->cbegin(), self->Contours->cend(), [&](vtkContourWidget* contour) {
		if (contour->GetContourRepresentation()->GetNumberOfNodes() < 1) {
			nullContour.push_back(index);
		}
		++index;
	});
	// delete this contours in case memory leak
	// since everytime delete one element, the size will change 
	// it need to be deleted in one direction from big to small
	nullContour.sort(std::greater<int>());
	// if the widget state is define, need to change to manipulate to re-generate 
	// a new contour 
	if (self->WidgetState == DEFINE && 
		nullContour.size() &&
		self->Contours->size() == nullContour.front() + 1) {
		self->WidgetState = MANIPULATE;
	}

	std::for_each(nullContour.cbegin(), nullContour.cend(), [=](int index) {
		self->DeleteContour(index);
	});

	self->Render();
}

void vtkContourWidgetSeries::ResetAction(vtkAbstractWidget *w)
{
	vtkContourWidgetSeries* self = reinterpret_cast<vtkContourWidgetSeries*>(w);
	self->ResetContours();
	self->InvokeEvent(vtkCommand::InteractionEvent, NULL);

}

void vtkContourWidgetSeries::PrintSelf(ostream & os, vtkIndent indent)
{

}

void vtkContourWidgetSeries::SetEnabled(int enabling)
{
	this->Superclass::SetEnabled(enabling);

	vtkContourListIterator iter;
	for (iter = this->Contours->begin(); iter != this->Contours->end(); ++iter)
	{
		(*iter)->SetEnabled(enabling);
	}

	if (!enabling)
	{
		this->RequestCursorShape(VTK_CURSOR_DEFAULT);
		this->WidgetState = vtkContourWidgetSeries::START;
	}

	this->Render();
}

void vtkContourWidgetSeries::SetCurrentRenderer(vtkRenderer * ren)
{
	this->Superclass::SetCurrentRenderer(ren);
	vtkContourListIterator iter = this->Contours->begin();
	for (; iter != this->Contours->end(); ++iter)
	{
		if (!ren)
		{
			// Disable widget if its being removed from the the renderer
			(*iter)->EnabledOff();
		}
		(*iter)->SetCurrentRenderer(ren);
	}
}

void vtkContourWidgetSeries::SetInteractor(vtkRenderWindowInteractor *rwi)
{
	this->Superclass::SetInteractor(rwi);
	vtkContourListIterator iter = this->Contours->begin();
	for (; iter != this->Contours->end(); ++iter)
	{
		(*iter)->SetInteractor(rwi);
	}
}

void vtkContourWidgetSeries::CreateDefaultRepresentation()
{
	if (!this->WidgetRep)
	{
		this->WidgetRep = vtkContourWidgetSeriesOrientedGlyphRepresentation::New();
	}
}

void vtkContourWidgetSeries::CloseAllLoops()
{
	std::for_each(this->Contours->cbegin(), this->Contours->cend(), [](vtkContourWidget* w) {
		w->CloseLoop(); 
	});
	this->Render();
}

void vtkContourWidgetSeries::SetProcessEvents(int pe)
{
	this->Superclass::SetProcessEvents(pe);

	vtkContourListIterator iter = this->Contours->begin();
	for (; iter != this->Contours->end(); ++iter)
	{
		(*iter)->SetProcessEvents(pe);
	}
}
