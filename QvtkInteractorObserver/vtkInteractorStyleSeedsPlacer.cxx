/*
Author:		Wong, Matthew Lun
Date:		16th, June 2016
Occupation:	Chinese University of Hong Kong,
Department of Imaging and Inteventional Radiology,
Junior Research Assistant


This class is an interactor updatedOverlay from TrackBallCamera, providing an extra function
of switching slice planes position to the world position clicked on.


Wong Matthew Lun
Copyright (C) 2016
*/
#include "vtkInteractorStyleSeedsPlacer.h"

#include <vtkRenderWindowInteractor.h>
#include <vtkAbstractPicker.h>
#include <vtkProperty.h>
#include <vtkCallbackCommand.h>
#include <vtkObjectFactory.h>
#include <vtkImageActorPointPlacer.h>
#include <vtkPointHandleRepresentation3D.h>
#include <vtkWidgetEvent.h>
#include <vtkWidgetCallbackMapper.h>
#include <vtkHandleWidget.h>
#include <vtkSeedWidget.h>
#include <vtkSeedRepresentation.h>



#include <algorithm>



using namespace std;

//class SeedsPlacerRepresentation : public vtkSeedRepresentation
//{
//public:
//	vtkTypeMacro(SeedsPlacerRepresentation, vtkSeedRepresentation);
//	static SeedsPlacerRepresentation* New();
//
//	vtkImageActorPointPlacer* GetImagePointPlacer();
//protected:
//	SeedsPlacerRepresentation();
//
//};

//class SeedsPlacerWidget : public vtkSeedWidget
//{
//public:
//	vtkTypeMacro(SeedsPlacerWidget, vtkSeedWidget);
//	static SeedsPlacerWidget* New() { return new SeedsPlacerWidget; }
//protected:
//	SeedsPlacerWidget() {
//		// using a NULL function to replace CompletedAction(vtkAbstractWidget *w) {};
//		this->CallbackMapper->SetCallbackMethod(vtkCommand::RightButtonPressEvent,
//			vtkWidgetEvent::Completed,
//			this, [](vtkAbstractWidget* widget) {});
//		vtkSmartPointer<vtkCallbackCommand> callback =
//			vtkSmartPointer<vtkCallbackCommand>::New();
//		callback->SetCallback(SeedsPlacerWidget::CallbackSave);
//		this->AddObserver(vtkCommand::StartInteractionEvent, callback);
//		this->AddObserver(vtkCommand::EndInteractionEvent, callback);
//		this->AddObserver(vtkCommand::PlacePointEvent, callback);
//	}
//
//	//static void MoveAction(vtkAbstractWidget *w){
//	//	w->InvokeEvent(vtkCommand::LeftButtonPressEvent);
//	//}
//	//static void DeleteAction(vtkAbstractWidget *w) {}
//
//private:
//	friend class vtkInteractorStyleSeedsPlacer;
//	vtkInteractorStyleSeedsPlacer* seedsPlacer;
//	static void CallbackSave(vtkObject* caller,
//		unsigned long eid, void* clientdata, void *calldata) {
//		SeedsPlacerWidget* _widget = static_cast<SeedsPlacerWidget*>(caller);
//		vtkSeedRepresentation* seedRep = _widget->GetSeedRepresentation();
//		double worldPos[3];
//		int seedNum = seedRep->GetActiveHandle();
//		int newImagePos[3];
//		seedRep->GetSeedWorldPosition(seedNum, worldPos);
//		if (eid == vtkCommand::EndInteractionEvent) {
//
//			for (int pos = 0; pos < 3; ++pos) {
//				newImagePos[pos] = (worldPos[pos] - _widget->seedsPlacer->GetOrigin()[pos]) /
//					_widget->seedsPlacer->GetSpacing()[pos] + 0.5;
//			}
//			_widget->seedsPlacer->UpdateWidgetToSeeds(newImagePos, oldImagePos);
//
//			// update other viewers' seed widgets
//			for (list<AbstractInteractorStyleImage*>::const_iterator cit =
//				_widget->seedsPlacer->imageStyles.cbegin(); cit != _widget->seedsPlacer->imageStyles.cend(); ++cit) {
//				vtkInteractorStyleSeedsPlacer* _style = vtkInteractorStyleSeedsPlacer::SafeDownCast(*cit);
//				if (_style != nullptr && _style->customFlag && _style != _widget->seedsPlacer) {
//					_style->GenerateWidgetFromSeeds();
//				}
//			}
//		}
//		else if (eid == vtkCommand::PlacePointEvent) {
//			_widget->seedsPlacer->SaveWidgetToSeeds();
//		}
//		else if (eid == vtkCommand::StartInteractionEvent) {
//			for (int pos = 0; pos < 3; ++pos) {
//				oldImagePos[pos] = (worldPos[pos] - _widget->seedsPlacer->GetOrigin()[pos]) /
//					_widget->seedsPlacer->GetSpacing()[pos] + 0.5;
//			}
//		}
//	}
//	static int oldImagePos[3];
//};

//int SeedsPlacerWidget::oldImagePos[3] = { -1 };
vtkStandardNewMacro(vtkInteractorStyleSeedsPlacer);

//vector<int*> vtkInteractorStyleSeedsPlacer::seeds;

//void vtkInteractorStyleSeedsPlacer::SetCustomEnabled(bool flag)
//{
//	AbstractNavigation::SetCustomEnabled(flag);
//	if (flag) {
//		vtkImageActorPointPlacer* pointPlacer = vtkImageActorPointPlacer::New();
//		pointPlacer->SetImageActor(GetVtkImageViewer2()->GetImageActor());
//
//		vtkPointHandleRepresentation3D* pointHandleRep = vtkPointHandleRepresentation3D::New();
//		pointHandleRep->SetPointPlacer(pointPlacer);
//		pointPlacer->Delete();
//
//		vtkSeedRepresentation* seedRep = vtkSeedRepresentation::New();
//		seedRep->SetHandleRepresentation(pointHandleRep);
//		pointHandleRep->Delete();
//
//		seedWidget->SetRepresentation(seedRep);
//		seedRep->Delete();
//		// for some opengl error
//		//SeedsPlacerRepresentation::SafeDownCast(seedRep)->GetImagePointPlacer()->SetPixelTolerance(2);
//		// SetInteractor CANNOT place in the constructor
//		seedWidget->SetInteractor(this->Interactor);
//		seedWidget->EnabledOn();
//		GenerateWidgetFromSeeds();
//	}
//	else {
//		seedWidget->EnabledOff();
//		ClearAllSeedWidget();
//	}
//}

//void vtkInteractorStyleSeedsPlacer::SetFocalSeed(vector<int*>& seeds, int i)
//{
//	//if (i >= seedRep->GetNumberOfSeeds() || i == focalSeed) {
//	//	return;
//	//}
//	//focalSeed = i;
//	if (i < 0 || i > seeds.size()) {
//		return;
//	}
//	int imageCoordinate[3];
//	memcpy(imageCoordinate, seeds[i], sizeof(imageCoordinate));
//	SetCurrentFocalPointWithImageCoordinate(imageCoordinate[0], imageCoordinate[1],
//		imageCoordinate[2]);
//}

void vtkInteractorStyleSeedsPlacer::PrintSelf(ostream & os, vtkIndent indent)
{
	Superclass::PrintSelf(os, indent);
}

void vtkInteractorStyleSeedsPlacer::SetInteractor(vtkRenderWindowInteractor * interactor)
{
	Superclass::SetInteractor(interactor);
	this->SeedWidget->SetInteractor(interactor);
}

//void vtkInteractorStyleSeedsPlacer::SetFocalSeed(int i)
//{
//	SetFocalSeed(seeds, i);
//}

//void vtkInteractorStyleSeedsPlacer::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
//{
//
//	//int oldIJK[3];
//	//GetImageViewer()->GetFocalPointWithImageCoordinate(oldIJK);
//	AbstractNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
//	//for (list<AbstractInteractorStyleImage*>::const_iterator cit =
//	//	imageStyles.cbegin(); cit != imageStyles.cend(); ++cit) {
//	//	vtkInteractorStyleSeedsPlacer* _style = vtkInteractorStyleSeedsPlacer::SafeDownCast(*cit);
//	//	if (_style != nullptr &&
//	//		_style->customFlag &&
//	//		_style->GetSlice() != oldIJK[_style->GetSliceOrientation()]) {
//	//		_style->GenerateWidgetFromSeeds();
//	//		_style->seedWidget->Render();
//	//	}
//	//}
//}
//
//void vtkInteractorStyleSeedsPlacer::SetCurrentFocalPointWithImageCoordinateByViewer(int i, int j, int k)
//{
//	// Only Generate seedWidgets when it is needed
//	// if its slice has not changed, it needs not to re-generate
//	int ijk[3];
//	GetImageViewer()->GetFocalPointWithImageCoordinate(ijk); 
//	AbstractNavigation::SetCurrentFocalPointWithImageCoordinateByViewer(i, j, k);
//	if (GetSlice() != ijk[GetSliceOrientation()]) {
//		GenerateWidgetFromSeeds();
//		seedWidget->Render();
//	}
//
//}
#include <vtkBoundedPlanePointPlacer.h>
#include <vtkNew.h>

vtkInteractorStyleSeedsPlacer::vtkInteractorStyleSeedsPlacer()
{
	vtkNew<vtkBoundedPlanePointPlacer> pointPlacer;
	this->PointPlacer = pointPlacer.GetPointer();
	vtkNew<vtkPointHandleRepresentation3D> pointHandleRep;
	pointHandleRep->SetPointPlacer(this->PointPlacer);

	this->SeedWidget = vtkSeedWidget::New();
	this->SeedWidget->CreateDefaultRepresentation();
	this->SeedWidget->GetSeedRepresentation()->SetHandleRepresentation(pointHandleRep.GetPointer());
	//vtkPointHandleRepresentation3D* pointHandleRep = vtkPointHandleRepresentation3D::New();
	//pointHandleRep->SetPointPlacer(pointPlacer);
	//pointPlacer->Delete();

	//vtkSeedRepresentation* seedRep = vtkSeedRepresentation::New();
	//seedRep->SetHandleRepresentation(pointHandleRep);
	//pointHandleRep->Delete();
	//SeedsPlacerWidget::SafeDownCast(seedWidget)->seedsPlacer = this;
}

vtkInteractorStyleSeedsPlacer::~vtkInteractorStyleSeedsPlacer()
{
	this->SeedWidget->EnabledOff();
	this->SeedWidget->SetInteractor(nullptr);
	this->SeedWidget ->Delete();
	//ClearAllSeeds();
}

//void vtkInteractorStyleSeedsPlacer::OnLeftButtonUp()
//{
//	AbstractNavigation::OnLeftButtonUp();
//	
//	//SaveWidgetToSeeds();
//	// update other viewers' seed widgets
//	for (list<AbstractInteractorStyleImage*>::const_iterator cit =
//		imageStyles.cbegin(); cit != imageStyles.cend(); ++cit) {
//		vtkInteractorStyleSeedsPlacer* _style = vtkInteractorStyleSeedsPlacer::SafeDownCast(*cit);
//		if (_style != nullptr && _style->customFlag && _style != this ) {
//			_style->GenerateWidgetFromSeeds();
//		}
//	}
//}
//
//void vtkInteractorStyleSeedsPlacer::OnKeyPress()
//{
//	string key = this->Interactor->GetKeySym();
//	cout << __func__ << ' ' << key << endl;
//	if (key == "D") {
//
//	}
//	else {
//		AbstractNavigation::OnKeyPress();
//	}
//}
//
//void vtkInteractorStyleSeedsPlacer::GenerateWidgetFromSeeds(const vector<int*>& seeds)
//{
//	ClearAllSeedWidget();
//
//
//	for (vector<int*>::const_iterator cit = seeds.cbegin();
//		cit != seeds.cend(); ++cit) {
//		int* imagePos = (*cit);
//		double worldPos[3];
//		for (int pos = 0; pos < 3; ++pos) {
//			worldPos[pos] = (imagePos[pos] * GetSpacing()[pos]) + GetOrigin()[pos];
//		}
//		if (imagePos[GetSliceOrientation()] == GetImageViewer()->GetSlice()) {
//			vtkHandleWidget* newSeed = seedWidget->CreateNewHandle();
//			newSeed->GetHandleRepresentation()->SetWorldPosition(worldPos);
//			newSeed->EnabledOn();
//		}
//	}
//	seedWidget->Render();
//}
//
//void vtkInteractorStyleSeedsPlacer::SaveWidgetToSeeds()
//{
//	SaveWidgetToSeeds(seeds);
//}
//
//void vtkInteractorStyleSeedsPlacer::UpdateWidgetToSeeds(int* newImagePos, int* oldImagePos)
//{
//	UpdateWidgetToSeeds(seeds, newImagePos, oldImagePos);
//}
//
//void vtkInteractorStyleSeedsPlacer::UpdateWidgetToSeeds(vector<int*>& seeds, int * newImagePos, int * oldImagePos)
//{
//
//	// if oldImagePos is nullptr or oldImagePos is not a valid number, 
//	// replace the existed newImagePos or push the new newImagePos
//	if (oldImagePos == nullptr || 
//		oldImagePos[0] < 0 || 
//		oldImagePos[1] < 0 || 
//		oldImagePos[2] < 0) {
//		oldImagePos = newImagePos;
//	}
//	vector<int*>::iterator it = find_if(seeds.begin(),
//		seeds.end(), [oldImagePos](int* index)->bool {
//		return
//			oldImagePos[0] == index[0] &&
//			oldImagePos[1] == index[1] &&
//			oldImagePos[2] == index[2];
//	});
//	if (it != seeds.end()) {
//		memcpy(*it, newImagePos, sizeof(int[3]));
//	}
//	else {
//		seeds.push_back(new int[3]);
//		memcpy(seeds.back(), newImagePos, sizeof(int[3]));
//	}
//}
//
//void vtkInteractorStyleSeedsPlacer::ClearAllSeeds()
//{
//	ClearAllSeeds(seeds);
//}
//
//void vtkInteractorStyleSeedsPlacer::SaveWidgetToSeeds(vector<int*>& seeds)
//{
//	for (int i = seedWidget->GetSeedRepresentation()->GetNumberOfSeeds() - 1; i >= 0; --i) {
//		//double* worldPos = new double[3]; // #MemLeakHere
//		double worldPos[3];
//		seedWidget->GetSeedRepresentation()->GetSeedWorldPosition(i, worldPos);
//		
//		int imagePos[3];
//		for (int pos = 0; pos < 3; ++pos) {
//			imagePos[pos] = (worldPos[pos] - GetOrigin()[pos]) / GetSpacing()[pos] + 0.5;
//		}
//		UpdateWidgetToSeeds(seeds, imagePos);
//	}
//}
//
//void vtkInteractorStyleSeedsPlacer::DropSeed()
//{
//	DropSeed(seeds);
//}
//
//void vtkInteractorStyleSeedsPlacer::DropSeed(vector<int*>& seeds)
//{
//	double* worldPos = GetImageViewer()->GetFocalPointWithWorldCoordinate();
//	vtkHandleWidget* newSeed = seedWidget->CreateNewHandle();
//	newSeed->GetHandleRepresentation()->SetWorldPosition(worldPos);
//	newSeed->EnabledOn();
//	seedWidget->Render();
//
//	SaveWidgetToSeeds(seeds);
//
//	// update other viewers' seed widgets
//	for (list<AbstractInteractorStyleImage*>::const_iterator cit =
//		imageStyles.cbegin(); cit != imageStyles.cend(); ++cit) {
//		vtkInteractorStyleSeedsPlacer* _style = vtkInteractorStyleSeedsPlacer::SafeDownCast(*cit);
//		if (_style != nullptr && _style->customFlag && _style != this) {
//			_style->GenerateWidgetFromSeeds();
//		}
//	}
//}
//
//void vtkInteractorStyleSeedsPlacer::ClearAllSeedWidget()
//{
//	for (int i = seedWidget->GetSeedRepresentation()->GetNumberOfSeeds() - 1; i >= 0; --i) {
//		seedWidget->DeleteSeed(i);
//	}
//	seedWidget->Render();
//}
//
//void vtkInteractorStyleSeedsPlacer::GenerateWidgetFromSeeds()
//{
//	GenerateWidgetFromSeeds(seeds);
//}
//
//void vtkInteractorStyleSeedsPlacer::ClearAllSeeds(vector<int*>& seed)
//{
//	if (seed.size() != 0) {
//		while (!seed.empty())
//		{
//			delete[] seed.back();
//			seed.pop_back();
//		}
//	}
//}
