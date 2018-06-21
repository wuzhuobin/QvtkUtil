// me
#include "QvtkViewer.h"
#include "QvtkProp.h"

// vtk
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkInteractorStyle.h>
#include <vtkCursor3D.h>
#include <vtkCornerAnnotation.h>
#include <vtkNew.h>
#include <vtkCallbackCommand.h>

// qt
#include <QDebug>
namespace Q {
namespace vtk{

class CursorCallbackCommand : public vtkCallbackCommand
{
public:
	static CursorCallbackCommand* New() { return new CursorCallbackCommand; }
	vtkTypeMacro(CursorCallbackCommand, vtkCallbackCommand);
	void Execute(vtkObject *caller,
		unsigned long eid,
		void *callData) VTK_OVERRIDE {
		Viewer* self = static_cast<Viewer*>(this->ClientData);
		vtkCursor3D* cursor = static_cast<vtkCursor3D*>(caller);
		if (self->GetCursorDesyncFlag() && cursor != self->GetCursorSource()) {
			return;
		}
		double* xyz = reinterpret_cast<double*>(callData);
		emit self->CursorPositionChanged(xyz[0], xyz[1], xyz[2]);
	}

};

//vtkNew<vtkCursor3D> cursorSourceMemory;
//vtkCursor3D* Viewer::GetSyncCursorSource()
//    = cursorSourceMemory.GetPointer();
QList<Viewer*> Viewer::s_viewersList;

Viewer* Viewer::getViewerOfInteractor(vtkInteractorStyle* style)
{
	QList<Viewer*> viewersVect = Viewer::GetAllViewers();
	typedef QList<Viewer*>::const_iterator Const_Iter;
	for (Const_Iter iter = viewersVect.cbegin(); iter != viewersVect.cend(); iter++)
	{
		if ((*iter)->GetInteractor()->GetInteractorStyle() == style)
		{
			return (*iter);
		}
	}
	//throw std::logic_error("Cannot find viewer for this interactor style!");
	qCritical() << "Cannot find viewer for the interactor style: " << style;
	return nullptr;
}

void Viewer::renderAllViewers()
{
	foreach (Viewer* viewer, s_viewersList)
	{
		viewer->Render();
	}
}

void Viewer::renderViewersByGroup(int group)
{
	foreach(Viewer* viewer, s_viewersList)
	{
		if (viewer->GetViewerGroup() != group)
			continue;
		else
		{
			viewer->Render();
		}
	}

}

void Viewer::setGroupInteractorStyle(vtkInteractorStyle* style, int group)
{
    // #TODO: QvtkABstractViewer::setGroupInteractorStyle(style, group)
    //throw std::logic_error("Not Implemented!");
	qCritical() << "Not implemented!";
	return;

    //typedef std::vector<Viewer*>::iterator IterType;
    //for(IterType iter = s_viewersList.begin(); iter != s_viewersList.end();iter++)
    //{
    //    Viewer* viewers = *iter;
    //    if (viewers->viewerGroup != group)
    //        continue;
    //    else
    //    {
    //        try {
    //            viewers->ErrorCheck();
    //        }
    //        catch (std::exception &e)
    //        {
    //            qCritical() << "Viewer: " << viewers << e.what() << endl;
    //            continue;
    //        }
    //        viewers->iren->SetInteractorStyle(style);
    //    }
    //}
}

vtkRenderer* Viewer::AddRenderer(int layer)
{
    vtkRenderer* ren = vtkRenderer::New();
    //#TODO: Expand number of layer of renderwindo if layer > GetNumberofLayer()
	if (layer >= this->GetRenderWindow()->GetNumberOfLayers()) {
		this->GetRenderWindow()->SetNumberOfLayers(layer + 1);
	}
	AddRenderer(ren, layer);
    return ren;
}

void Viewer::AddRenderer(vtkRenderer* ren, int layer)
{
    if (layer >= 0)
    {
        ren->SetLayer(layer);
    }
    else {
        ren->SetLayer(this->renderers.size());
    }
    ren->SetActiveCamera(this->camera);
	// 3. Choose to use depth peeling (if supported) (initial value is 0 (false)):
	ren->SetUseDepthPeeling(this->depthPeelingFlag);
	// 4. Set depth peeling parameters
	// - Set the maximum number of this->Rendering passes (initial value is 4):
	ren->SetMaximumNumberOfPeels(this->maxNoOfPeels);
	// - Set the occlusion ratio (initial value is 0.0, exact image):
	ren->SetOcclusionRatio(this->occlusionRatio);
    this->renderers.push_back(ren);

    /* Add to renderwindow if it exist*/
    if (this->GetRenderWindow())
    {
        if (this->GetRenderWindow()->GetNumberOfLayersMaxValue() < ren->GetLayer())
            this->GetRenderWindow()->SetNumberOfLayers(ren->GetLayer());
        this->GetRenderWindow()->AddRenderer(ren);
    }
}

void Viewer::RemoveRenderer(int index)
{
	if (index < 0 || index > this->renderers.size()) {
		qWarning() << "Renderer requested to delete doesn't exist.";
		return;
	}
	vtkRenderer* ren = this->renderers[index];
	this->renderers.removeAt(index);
	RemoveAllProp(ren);
	this->GetRenderWindow()->RemoveRenderer(ren);
	ren->Delete();
	qWarning() << "Renderer requested to delete doesn't exist.";
}

void Viewer::RemoveRenderer(vtkRenderer* ren)
{
	int index = this->renderers.indexOf(ren);
	RemoveRenderer(index);
}

void Viewer::AddProp(Prop * prop)
{
	AddProp(prop, this->renderers[0]);
}

void Viewer::AddProp(Prop * prop, vtkRenderer * renderer)
{
	if (!this->renderers.contains(renderer)) {
		qWarning() << "The renderer does not belong to this viewer.";
	}
	
	prop->setViewerID(s_viewersList.indexOf(this));
	prop->setRendererID(this->renderers.indexOf(renderer));
	prop->RenderingOn();
	renderer->AddViewProp(prop->getProp());
	propToRenderer->insert(prop, renderer);
}

QList<Prop*> Viewer::GetProps()
{
	return this->propToRenderer->keys();
}

void Viewer::RemoveProp(Prop * prop)
{
	if (!this->propToRenderer->contains(prop)) {
		qWarning() << "Remove prop" << prop->getUniqueName() << "fail.";
		qWarning() << "This prop does not belong to this viewer.";
		return;
	}
	
	vtkRenderer* renderer = this->propToRenderer->take(prop);
	
	if (!renderer || !this->renderers.contains(renderer)) 
	{
		qWarning() << "Remove prop" << prop->getUniqueName() << "fail.";
		qWarning() << "The renderer does not belong to this viewer.";
		return;
	}
	prop->setViewerID(-1);
	prop->setRendererID(-1);
	prop->RenderingOff();
	renderer->RemoveViewProp(prop->getProp());
	propToRenderer->remove(prop);
}


void Viewer::RemoveAllProp()
{
	for (QList<vtkRenderer*>::const_iterator cit = this->renderers.cbegin();
		cit != this->renderers.cend(); ++cit) {
		RemoveAllProp(*cit);
	}
}

void Viewer::RemoveAllProp(vtkRenderer * renderer)
{
	if (!this->renderers.contains(renderer)) {
		qWarning() << "The renderer does not belong to this viewer.";
	}
	QList<Prop* > keys = this->propToRenderer->keys(renderer);

	for (QList<Prop* >::ConstIterator cit = keys.cbegin();
		cit != keys.cend(); ++cit) {
		RemoveProp(*cit);
	}
}

void Viewer::SetCursorAlwaysFaceCamera(bool)
{
	//throw std::logic_error("Not Implemented!");
	qCritical() << "Not Implemented!";

	// #TODO: Implement this function 

	/*
	1. Create a new member transform
	2. Set new member transform as transform of actor
	3. Cocatenate rotation before translation
	*/
}

void Viewer::SetCursorDesyncFlag(bool desync)
{
	if (this->desyncCursorFlag == desync) {
		return;
	}

	this->desyncCursorFlag = desync;
	if (desync) {

		this->cursorActor->GetMapper()->SetInputConnection(
			this->GetCursorSource()->GetOutputPort());
	}
	else {
		this->cursorActor->GetMapper()->SetInputConnection(
			Viewer::GetSyncCursorSource()->GetOutputPort());

	}

}


void Viewer::SetCursorPosition(double x, double y, double z)
{
	if (x == this->GetCursorPosition()[0] &&
		y == this->GetCursorPosition()[1] &&
		z == this->GetCursorPosition()[2]) {
		return;
	}
	double xyz[3] = {x, y, z};
	if (this->desyncCursorFlag) {
		this->cursorSource->SetFocalPoint(xyz);
		// invoke event for synchronization
		this->cursorSource->InvokeEvent(vtkCommand::CursorChangedEvent, xyz);
	}
	else {
		Viewer::GetSyncCursorSource()->SetFocalPoint(xyz);
		// invoke event for synchronization
		Viewer::GetSyncCursorSource()->InvokeEvent(vtkCommand::CursorChangedEvent, xyz);
	}
}

double* Viewer::GetCursorPosition()
{
	/* NOTE: the returned pointer will change with calls to TransformPoint() function, advoid using
	         the pointer directly!
	*/
    //return this->GetCursorTransform()->TransformPoint(0, 0, 0);
	if (this->desyncCursorFlag) {
		return this->cursorSource->GetFocalPoint();
	}
	else {
		return Viewer::GetSyncCursorSource()->GetFocalPoint();

	}
}

void Viewer::UpdateDepthPeeling()
{
	// 1. Use a render window with alpha bits (as initial value is 0 (false)):
	this->GetRenderWindow()->SetAlphaBitPlanes(this->depthPeelingFlag);
	if (this->depthPeelingFlag) {
		// 2. Force to not pick a framebuffer with a multisample buffer
		// (as initial value is 8):
		this->GetRenderWindow()->SetMultiSamples(0);
	}
	else {
		this->GetRenderWindow()->SetMultiSamples(8);
	}

	foreach(vtkRenderer* renderer, this->renderers) {
		// 3. Choose to use depth peeling (if supported) (initial vCurrentChangedEventalue is 0 (false)):
		renderer->SetUseDepthPeeling(this->depthPeelingFlag);
		if (!this->depthPeelingFlag) {
			continue;
		}
		// 4. Set depth peeling parameters
		// - Set the maximum number of this->Rendering passes (initial value is 4):
		renderer->SetMaximumNumberOfPeels(this->maxNoOfPeels);
		// - Set the occlusion ratio (initial value is 0.0, exact image):
		renderer->SetOcclusionRatio(this->occlusionRatio);
	}
}

vtkCamera* Viewer::GetActiveCamera()
{
    return this->camera;
}

void Viewer::ResetCamera()
{
	qCritical() << "it not working well, camera is always defined by the last renderer";
	qCritical() << "layer must be specified.";
}

void Viewer::ResetCamera(int layer /*= -1*/)
{
	foreach(vtkRenderer* ren, this->renderers)
	{
		if (layer >= 0)
		{
			if (ren->GetLayer() == layer)
			{
				ren->ResetCamera();
				return;
			}
		}
	}

}

void Viewer::ResetCameraClippingRange(int layer /*= -1*/)
{
	foreach(vtkRenderer* ren, this->renderers)
	{
		if (layer >= 0)
		{
			if (ren->GetLayer() == layer)
			{
				ren->ResetCameraClippingRange();
				return;
			}
		}
	}
}

void Viewer::ResetCameraClippingRange()
{
	qCritical() << "it not working well, camera is always defined by the last renderer";
	qCritical() << "layer must be specified.";
}

void Viewer::Render()
{
	this->GetRenderWindow()->Render();
}

void Viewer::RenderAllViewersOfThisClass()
{
    typedef QList<Viewer *>::iterator IterType;
    for (IterType iter = this->s_viewersList.begin(); iter != this->s_viewersList.end(); iter++) {
        Viewer *viewer = *iter;
		if(this->metaObject()->className() == viewer->metaObject()->className()){
            viewer->GetRenderWindow()->Render();
        }
    }
}

void Viewer::SetEnableCornerAnnotation(bool b)
{
	this->GetCornerAnnotation()->SetVisibility(b);
}

void Viewer::AppendCornerAnnotation(int textPosition, QString text)
{
	const char* oldText = this->cornerAnnotation->GetText(textPosition);
    this->cornerAnnotation->SetText(textPosition, (oldText + text).toStdString().c_str());
}

void Viewer::SetMaxNoOfPeelings(int i)
{
	if (this->maxNoOfPeels == i) {
		return;
	}
	this->maxNoOfPeels = i;
	this->UpdateDepthPeeling();
}

void Viewer::SetOcclusionRatio(double ratio)
{
	if (this->occlusionRatio == ratio) {
		return;
	}
	this->occlusionRatio = ratio;
	this->UpdateDepthPeeling();
}


void Viewer::UpdateCursorPosition(double x, double y, double z)
{
	if (this->GetCornerAnnotation()->GetVisibility()) {
		QString format = "%1, %2, %3";
		QString text = format.arg(x, 0, 'f', 2).arg(y, 0, 'f', 2).arg(z, 0, 'f', 2);
		this->GetCornerAnnotation()->SetText(1, text.toStdString().c_str());
	}
	this->Render();
}

void Viewer::SetEnableDepthPeeling(bool flag)
{
	if (this->depthPeelingFlag == flag) {
		return;
	}
	this->depthPeelingFlag = flag;
	this->UpdateDepthPeeling();


}

vtkCursor3D * Q::vtk::Viewer::GetSyncCursorSource()
{
	if (Viewer::s_viewersList.empty()) {
		return nullptr;
	}
	return s_viewersList.first()->cursorSource;
}

Viewer::Viewer(QWidget* parent)
    :QWidget(parent)
{
	/* if every thing goes well, add self to viewer list */
	Viewer::s_viewersList.push_back(this);
	/* Initialize variables*/
	this->desyncCursorFlag = false;



	/* Create objects for member pointers */


	this->cursorCallback = CursorCallbackCommand::New();
	this->cursorCallback->SetClientData(this);
	connect(this, &Viewer::CursorPositionChanged,
		this, static_cast<void(Viewer::*)(double, double, double)>(&Viewer::UpdateCursorPosition));
	
	//if (Viewer::NumOfViewers() == 1) {
	//	Viewer::GetSyncCursorSource()->TranslationModeOn();
	//	Viewer::GetSyncCursorSource()->SetModelBounds(-15, 15, -15, 15, -15, 15);
	//	Viewer::GetSyncCursorSource()->SetFocalPoint(0, 0, 0);
	//	Viewer::GetSyncCursorSource()->AllOff();
	//	Viewer::GetSyncCursorSource()->AxesOn();
	//}

	this->cursorSource = vtkCursor3D::New();
	this->cursorSource->TranslationModeOn();
	this->cursorSource->SetModelBounds(-15, 15, -15, 15, -15, 15);
	this->cursorSource->SetFocalPoint(0, 0, 0);
	this->cursorSource->AllOff();
	this->cursorSource->AxesOn();
	this->cursorSource->AddObserver(vtkCommand::CursorChangedEvent, this->cursorCallback);
	Viewer::GetSyncCursorSource()->AddObserver(vtkCommand::CursorChangedEvent, this->cursorCallback);
	

	this->cursorActor = vtkActor::New();
    this->cursorActor->SetMapper(vtkSmartPointer<vtkPolyDataMapper>::New());
	this->cursorActor->GetMapper()->SetInputConnection(
		Viewer::GetSyncCursorSource()->GetOutputPort());
    this->cursorActor->GetProperty()->SetColor(1, 1, 0);
    this->cursorActor->SetPickable(false);

	this->cornerAnnotation = vtkCornerAnnotation::New();
	this->cornerAnnotation->SetPickable(false);
	this->cornerAnnotation->SetMaximumFontSize(10);
	this->cornerAnnotation->SetMaximumLineHeight(0.3);
	this->cornerAnnotation->SetVisibility(false);



	this->camera = vtkCamera::New();

	this->propToRenderer = new PropToRenderer;
	/* One renderer is created by default, its camera will be used by all other renderers */
	this->firstRenderer = vtkRenderer::New();
	this->firstRenderer->AddActor(this->cursorActor);
	this->firstRenderer->AddActor(this->cornerAnnotation);
	this->firstRenderer->SetActiveCamera(this->camera);

	this->depthPeelingFlag = true;
	this->maxNoOfPeels = 100;
	this->occlusionRatio = 0.1;

}

Viewer::~Viewer()
{
	// Remove all renderers
	foreach(vtkRenderer* renderer, renderers)
	{
		renderer->RemoveAllObservers();
		renderer->RemoveAllViewProps();
		renderer->SetRenderWindow(nullptr);
		renderer->Delete();
	}
	delete this->propToRenderer;

	this->camera->Delete();

	this->cornerAnnotation->Delete();

	this->cursorActor->Delete();

	this->cursorSource->RemoveObserver(this->cursorCallback);
	Viewer::GetSyncCursorSource()->RemoveObserver(this->cursorCallback);

	this->cursorSource->Delete();

	
	this->cursorCallback->Delete();

	Viewer::s_viewersList.removeOne(this);


}

}
}