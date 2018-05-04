// me 
#include "vtkWidgetCollectionBase.h"
// vtk
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorObserver.h>
#include <vtkRenderer.h>
vtkCxxSetObjectMacro(vtkWidgetCollectionBase, Interactor, vtkRenderWindowInteractor);
vtkCxxSetObjectMacro(vtkWidgetCollectionBase, CurrentRenderer, vtkRenderer);
vtkCxxSetObjectMacro(vtkWidgetCollectionBase, DefaultRenderer, vtkRenderer);

class vtkDistanceWidget2 : public vtkDistanceWidget
{
public:
	static vtkDistanceWidget2*New() { return new vtkDistanceWidget2; }
	vtkTypeMacro(vtkDistanceWidget2, vtkDistanceWidget);
protected:
	vtkDistanceWidget2() { this->CreateDefaultRepresentation(); }
};

void vtkWidgetCollectionBase::PrintSelf(ostream & os, vtkIndent indent)
{
	Superclass::PrintSelf(os, indent);
}

vtkWidgetCollectionBase::vtkWidgetCollectionBase()
{
	this->Interactor = nullptr;
	this->DefaultRenderer = nullptr;
	this->CurrentRenderer = nullptr;
}

vtkWidgetCollectionBase::~vtkWidgetCollectionBase()
{
}

vtkDistanceWidget* vtkWidgetCollectionBase::ProduceDistanceWidgets()
{
	vtkSmartPointer<vtkDistanceWidget> widget = 
		vtkSmartPointer<vtkDistanceWidget2>::New();
	this->DistanceWidgets.push_back(widget);
	widget->SetInteractor(this->Interactor);
	widget->SetDefaultRenderer(this->DefaultRenderer);
	//widget->On();
	return widget;
}

//void vtkWidgetCollectionBase::DestoryDistanceWidgets()
//{
//	if (this->DistanceWidgets.size() == 0) {
//		return;
//	}
//	DestroyDistanceWidgets(this->DistanceWidgets.back());
//}

void vtkWidgetCollectionBase::DestroyDistanceWidgets(vtkDistanceWidget * widget)
{
	if (this->DistanceWidgets.size() == 0) {
		return;
	}

	if (widget == nullptr) {
		widget = this->DistanceWidgets.back();
	}
	widget->SetInteractor(nullptr);
	//widget->Delete();
	this->DistanceWidgets.remove(widget);
}

void vtkWidgetCollectionBase::SetAllDistanceWidgetsEnabled(bool flag)
{
	for (std::list<vtkSmartPointer<vtkDistanceWidget>>::const_iterator cit = this->DistanceWidgets.cbegin();
		cit != this->DistanceWidgets.cend(); ++cit)
	{
		this->InternalUpdate(*cit, flag);
		(*cit)->SetEnabled(flag);
	}

}

void vtkWidgetCollectionBase::SetOneOfDistanceWidgetsEnabled(vtkDistanceWidget* widget, bool flag)
{
	this->InternalUpdate(widget, flag);
	widget->SetEnabled(flag);
}