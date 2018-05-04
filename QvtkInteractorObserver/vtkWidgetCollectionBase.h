#ifndef __VTK_WIDGET_COLLECTION_BASE_H__
#define __VTK_WIDGET_COLLECTION_BASE_H__
#pragma once
// me
#include "qvtkinteractorobserver_export.h"
// vtk
#include <vtkObject.h>
#include <vtkDistanceWidget.h>
#include <vtkSmartPointer.h>
class vtkRenderWindowInteractor;
class vtkRenderer;
// std
#include <list>
#define DESTORY_WIDGETS(NAME, CLASS) \
void Destroy##NAME(CLASS * widget = nullptr) \
{ \
	if(this->NAME.size() == 0) { \
		return; \
	} \
	if(widget == nullptr){ \
		widget = this->NAME.back(); \
	} \
	widget->SetInteractor(nullptr); \
	this->NAME.remove(widget); \
} 
#define PRODUCE_WIDGETS(NAME, CLASS) \
CLASS* Produce##NAME() \
{ \
	vtkSmartPointer<CLASS> widget = vtkSmartPointer<CLASS>::New(); \
	this->NAME.push_back(widget); \
	widget->SetInteractor(this->Interactor); \
	widget->SetDefaultRenderer(this->DefaultRenderer); \
	return widget; \
}
#define SET_ALL_WIDGETS_ENABLED(NAME, CLASS) \
void SetAll##NAME##Enabled(bool flag) \
{ \
	for (std::list<vtkSmartPointer<CLASS>>::const_iterator cit = this->NAME.cbegin(); \
		cit != this->NAME.cend(); ++cit) \
	{ \
		if(flag) \
		{ \
			this->InternalUpdate(*cit, flag); \
		} \
		(*cit)->SetEnabled(flag); \
		if(!flag) \
		{ \
			this->InternalUpdate(*cit, flag); \
		} \
	} \
}
#define SET_ONE_OF_WIDGETS_ENABLED(NAME, CLASS) \
void SetOneOf##NAME##Enabled(CLASS* widget, bool flag) \
{ \
	if(flag) \
	{ \
		this->InternalUpdate(widget, flag); \
	} \
	widget->SetEnabled(flag); \
	if(!flag) \
	{ \
		this->InternalUpdate(widget, flag); \
	} \
} 
#define GET_WIDGETS(NAME, CLASS) \
virtual std::list<vtkSmartPointer<CLASS>> Get##NAME() { return this->NAME; }
#define SETUP_WIDGETS(NAME, CLASS) \
DESTORY_WIDGETS(NAME, CLASS) \
PRODUCE_WIDGETS(NAME, CLASS) \
SET_ALL_WIDGETS_ENABLED(NAME, CLASS) \
SET_ONE_OF_WIDGETS_ENABLED(NAME, CLASS) \
GET_WIDGETS(NAME, CLASS)
class QVTKINTERACTOROBSERVER_EXPORT vtkWidgetCollectionBase :public vtkObject
{
public: 
	vtkTypeMacro(vtkWidgetCollectionBase, vtkObject);
	virtual void PrintSelf(ostream& os, vtkIndent indent) override;
	virtual void SetInteractor(vtkRenderWindowInteractor* interactor);
	virtual void SetDefaultRenderer(vtkRenderer* renderer);
	virtual void SetCurrentRenderer(vtkRenderer* renderer);
	virtual void InternalUpdate(vtkInteractorObserver* widget, bool flag) = 0;
	virtual vtkDistanceWidget* ProduceDistanceWidgets();
	virtual void DestroyDistanceWidgets(vtkDistanceWidget* widget = nullptr);
	virtual void SetAllDistanceWidgetsEnabled(bool flag);
	virtual void SetOneOfDistanceWidgetsEnabled(vtkDistanceWidget* widget, bool flag);
	virtual std::list<vtkSmartPointer<vtkDistanceWidget>> GetDistanceWidgets() { return this->DistanceWidgets; }
protected:
	vtkWidgetCollectionBase();
	virtual ~vtkWidgetCollectionBase() override;
	std::list<vtkSmartPointer<vtkDistanceWidget>> DistanceWidgets;
	vtkRenderWindowInteractor* Interactor;
	vtkRenderer* DefaultRenderer;
	vtkRenderer* CurrentRenderer;
private:
	vtkWidgetCollectionBase(const vtkWidgetCollectionBase&) VTK_DELETE_FUNCTION;
	void operator=(const vtkWidgetCollectionBase&) VTK_DELETE_FUNCTION;
};
#endif // !__VTK_WIDGET_COLLECTION_BASE_H__
