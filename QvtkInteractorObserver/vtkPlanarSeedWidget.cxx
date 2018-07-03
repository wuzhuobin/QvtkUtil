// me
#include "vtkPlanarSeedWidget.h"
#include "vtkWidgetSet2.h"
// vtk
#include <vtkObjectFactory.h>
#include <vtkSeedRepresentation.h>
#include <vtkPointHandleRepresentation3D.h>
#include <vtkBoundedPlanePointPlacer.h>
#include <vtkNew.h>
#include <vtkCallbackCommand.h>
#include <vtkWidgetEvent.h>
#include <vtkWidgetCallbackMapper.h>
#include <vtkHandleWidget.h>
#include <vtkHandleRepresentation.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkEvent.h>
#include <vtkPlane.h>
vtkStandardNewMacro(vtkPlanarSeedWidget);
vtkCxxSetObjectMacro(vtkPlanarSeedWidget, WidgetSet, vtkWidgetSet2);
void vtkPlanarSeedWidget::PrintSelf(ostream & os, vtkIndent indent)
{
	Superclass::PrintSelf(os, indent);
}

void vtkPlanarSeedWidget::SetProjectionNormal(int normal)
{
	vtkBoundedPlanePointPlacer* placer = static_cast<vtkBoundedPlanePointPlacer*>(
		this->GetSeedRepresentation()->GetHandleRepresentation()->GetPointPlacer());
	switch (normal)
	{
	case vtkBoundedPlanePointPlacer::XAxis:
	case vtkBoundedPlanePointPlacer::YAxis:
	case vtkBoundedPlanePointPlacer::ZAxis:
		placer->SetProjectionNormal(normal);
		break;
	case vtkBoundedPlanePointPlacer::Oblique:
	default:
		placer->SetProjectionNormal(vtkBoundedPlanePointPlacer::Oblique);
		break;
	}
	for (int i = 0; i < this->GetSeedRepresentation()->GetNumberOfSeeds(); ++i) {
		EnabledHandleInRange(this->GetSeed(i));
	}
}

void vtkPlanarSeedWidget::SetProjectionPosition(double x, double y, double z)
{
	vtkBoundedPlanePointPlacer* placer = static_cast<vtkBoundedPlanePointPlacer*>(
		this->GetSeedRepresentation()->GetHandleRepresentation()->GetPointPlacer());
	switch (placer->GetProjectionNormal())
	{
	case vtkBoundedPlanePointPlacer::XAxis:
		placer->SetProjectionPosition(x);
		break;
	case vtkBoundedPlanePointPlacer::YAxis:
		placer->SetProjectionPosition(y);
		break;
	case vtkBoundedPlanePointPlacer::ZAxis:
		placer->SetProjectionPosition(z);
		break;
	case vtkBoundedPlanePointPlacer::Oblique:
		placer->GetObliquePlane()->SetOrigin(x, y, z);
	default:
		break;
	}
	for (int i = 0; i < this->GetSeedRepresentation()->GetNumberOfSeeds(); ++i) {

		EnabledHandleInRange(this->GetSeed(i));
	}
}

void vtkPlanarSeedWidget::DropSeed(double x, double y, double z)
{
	double pos[3] = { x, y, z };
	vtkHandleWidget* newSeed = this->CreateNewHandle();
	newSeed->GetHandleRepresentation()->SetWorldPosition(pos);
	newSeed->SetInteractor(this->GetInteractor());
	this->EnabledHandleInRange(newSeed);
}

void vtkPlanarSeedWidget::CleanAllSeed()
{
	for (int i = this->GetSeedRepresentation()->GetNumberOfSeeds(); i > 0; --i) {
		this->DeleteSeed(i - 1);
	}
}

void vtkPlanarSeedWidget::LoadSeedFromPolyData(vtkPolyData * polyData)
{
	if (!polyData) {
		vtkErrorMacro(<< "Input polydata is a nullptr." << 
			"Seed cannot be generated. ");
		return;
	}
	vtkPoints* points = polyData->GetPoints();
	if (!points) {
		vtkErrorMacro(<< "Input polydata does not have vtkPoints, which is a nullptr. " <<
			"Seed cannot be generated. ");
		return;
	}

	for (int i = 0; i < points->GetNumberOfPoints(); ++i) {
		vtkHandleWidget* seed = this->CreateNewHandle();
		seed->GetHandleRepresentation()->SetWorldPosition(points->GetPoint(i));
		seed->SetInteractor(this->GetInteractor());
		this->EnabledHandleInRange(seed);
	}

}

void vtkPlanarSeedWidget::SaveSeedToPolyData(vtkPolyData * polyData)
{
	if (!polyData)
	{
		vtkErrorMacro(<< "Input polydata is a nullptr. " 
			<< "Seed cannot be saved. ");
		return;
	}
	vtkNew<vtkPoints> points;
	double pos[3];
	for (int i = 0; i < this->GetSeedRepresentation()->GetNumberOfSeeds(); ++i) {
		this->GetSeedRepresentation()->GetSeedWorldPosition(i, pos);
		points->InsertNextPoint(pos);
	}
	polyData->SetPoints(points.GetPointer());
}

vtkPlanarSeedWidget::vtkPlanarSeedWidget()
{
	// initialization.
	vtkNew<vtkPlane> plane;
	vtkNew<vtkBoundedPlanePointPlacer> pointPlacer;
	pointPlacer->SetObliquePlane(plane.GetPointer());
	vtkNew<vtkPointHandleRepresentation3D> handleRep;
	handleRep->SetPointPlacer(pointPlacer.GetPointer());
	this->CreateDefaultRepresentation();
	this->GetSeedRepresentation()->SetHandleRepresentation(handleRep.GetPointer());
	this->WidgetSet = nullptr;
	this->Range = 0.5;
	this->DeletedHandle = -1;
	this->CallbackMapper->SetCallbackMethod(vtkCommand::LeftButtonPressEvent,
		vtkWidgetEvent::AddPoint, this, vtkPlanarSeedWidget::AddPointDispatcher);
	this->CallbackMapper->SetCallbackMethod(vtkCommand::MouseMoveEvent,
		vtkWidgetEvent::Move,
		this, vtkPlanarSeedWidget::MoveDispatcher);
	this->CallbackMapper->SetCallbackMethod(vtkCommand::KeyPressEvent, 
		vtkEvent::NoModifier, 127, 1, "Delete", 
		vtkWidgetEvent::Delete, 
		this, vtkPlanarSeedWidget::DeleteDispatcher);
}

void vtkPlanarSeedWidget::EnabledHandleInRange(vtkHandleWidget* handle)
{
	vtkSeedRepresentation* rep = this->GetSeedRepresentation();
	vtkBoundedPlanePointPlacer* placer = static_cast<vtkBoundedPlanePointPlacer*>(
		handle->GetHandleRepresentation()->GetPointPlacer());
	int orientation = placer->GetProjectionNormal();
	double* worldPos = handle->GetHandleRepresentation()->GetWorldPosition();
	double distance = 0;
	switch (orientation)
	{
	case vtkBoundedPlanePointPlacer::XAxis:
	case vtkBoundedPlanePointPlacer::YAxis:
	case vtkBoundedPlanePointPlacer::ZAxis: {
		double pos = placer->GetProjectionPosition();
		distance = abs(pos - worldPos[orientation]);
	}
	case vtkBoundedPlanePointPlacer::Oblique:
	default:
		distance = abs(placer->GetObliquePlane()->DistanceToPlane(worldPos));
		break;
	}
	bool inRange = distance > this->Range ? false : true;
	if (Range == 0) {
		inRange = true;
	}
	handle->SetEnabled(inRange);
}

void vtkPlanarSeedWidget::AddPointDispatcher(vtkAbstractWidget * widget)
{
	vtkPlanarSeedWidget* self =
		reinterpret_cast<vtkPlanarSeedWidget*>(widget);
	if (self->WidgetSet) {
		self->WidgetSet->DispatchAction(self, &vtkPlanarSeedWidget::AddPointAction);
	}
	else {
		vtkSeedWidget::AddPointAction(self);
	}
}

void vtkPlanarSeedWidget::MoveDispatcher(vtkAbstractWidget * widget)
{
	vtkPlanarSeedWidget* self =
		reinterpret_cast<vtkPlanarSeedWidget*>(widget);
	if (self->WidgetSet) {
		self->WidgetSet->DispatchAction(self, &vtkPlanarSeedWidget::MoveAction);
	}
	else {
		vtkSeedWidget::MoveAction(self);
	}
}

void vtkPlanarSeedWidget::DeleteDispatcher(vtkAbstractWidget * widget)
{
}

void vtkPlanarSeedWidget::AddPointAction(vtkPlanarSeedWidget * dispatcher)
{
	if (this != dispatcher) {
		if (dispatcher->WidgetState == vtkSeedWidget::MovingSeed ||
			dispatcher->WidgetState == vtkSeedWidget::Start ||
			dispatcher->WidgetState == vtkSeedWidget::PlacedSeeds) {
			return;
		}
		else if (dispatcher->WidgetState == vtkSeedWidget::PlacingSeeds) {
			int numOfDispatcherSSeeds = dispatcher->GetSeedRepresentation()->GetNumberOfSeeds();
			if (numOfDispatcherSSeeds > 0) {

				vtkHandleWidget* newSeed =
					dispatcher->GetSeed(numOfDispatcherSSeeds - 1);

				int numOfSeeds = this->GetSeedRepresentation()->GetNumberOfSeeds();

				// if there is no first seed, create new seed
				if (numOfSeeds > 0) {
					// if the recently created seed is not the same as the 
					// dispatcher's recently created seed, 
					// create new seed
					vtkHandleWidget* oldSeed = this->GetSeed(numOfSeeds - 1);
					if (std::equal(
						newSeed->GetHandleRepresentation()->GetWorldPosition(),
						newSeed->GetHandleRepresentation()->GetWorldPosition() + 2,
						oldSeed->GetHandleRepresentation()->GetWorldPosition())) {
						return;
					}
				}
				vtkHandleWidget* _newSeed = this->CreateNewHandle();
				_newSeed->GetHandleRepresentation()->SetWorldPosition(
					newSeed->GetHandleRepresentation()->GetWorldPosition());
				_newSeed->SetInteractor(this->GetInteractor());
				vtkBoundedPlanePointPlacer* placer = static_cast<vtkBoundedPlanePointPlacer*>(
					_newSeed->GetHandleRepresentation()->GetPointPlacer());
				EnabledHandleInRange(_newSeed);
				this->Render();
			}
		}


	}
	else {
		Superclass::AddPointAction(this);
	}
}

void vtkPlanarSeedWidget::MoveAction(vtkPlanarSeedWidget * dispatcher)
{
	if (this != dispatcher)
	{
		int X = dispatcher->Interactor->GetEventPosition()[0];
		int Y = dispatcher->Interactor->GetEventPosition()[1];
		int state = dispatcher->WidgetRep->ComputeInteractionState(X, Y);
		if (state != vtkSeedRepresentation::NearSeed)
		{
			return;
		}
		int activeHandle = dispatcher->GetSeedRepresentation()->GetActiveHandle();
		if (activeHandle < 0) {
			return;
		}
		vtkHandleWidget* handle = this->GetSeed(activeHandle);
		handle->GetHandleRepresentation()->SetWorldPosition(
			dispatcher->GetSeed(activeHandle)->GetHandleRepresentation()->GetWorldPosition());
		this->EnabledHandleInRange(handle);
	}
	else
	{
		Superclass::MoveAction(this);
	}
}

void vtkPlanarSeedWidget::DeleteAction(vtkPlanarSeedWidget * dispatcher)
{
	int deleteHandle = dispatcher->DeletedHandle;
	if (this != dispatcher &&
		deleteHandle != -1) {
		this->GetSeedRepresentation()->RemoveHandle(deleteHandle);
		this->DeleteSeed(deleteHandle);
	}
	else
	{
		// Do nothing if outside
		if (this->WidgetState != vtkSeedWidget::PlacingSeeds)
		{
			return;
		}

		// Remove last seed
		vtkSeedRepresentation *rep =
			reinterpret_cast<vtkSeedRepresentation*>(this->WidgetRep);
		this->DeletedHandle  = rep->GetActiveHandle();
		if (this->DeletedHandle != -1)
		{
			rep->RemoveActiveHandle();
		}
		else
		{
			rep->RemoveLastHandle();
			this->DeletedHandle = static_cast<int>(this->GetSeedRepresentation()->GetNumberOfSeeds());
		}
		this->DeleteSeed(this->DeletedHandle);
		// Got this event, abort processing if it
		this->EventCallbackCommand->SetAbortFlag(1);
		this->Render();
		Superclass::DeleteAction(this);
	}
}

vtkPlanarSeedWidget::~vtkPlanarSeedWidget()
{
}
