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
#include <vtkMatrix4x4.h>
// qt
#include <QDebug>
namespace Q {
	namespace vtk {
		vtkCursor3D *syncCursorSource = nullptr;
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
				if (cursor != self->getCursorSource()) {
					return;
				}
				double* xyz = reinterpret_cast<double*>(callData);
				double _xyz[3];
				std::copy(xyz, xyz + 3, _xyz);
				cursor->SetFocalPoint(xyz);
				if (self->cursorTransformFunction != nullptr) {
					self->cursorTransformFunction(_xyz, self->cursorActor->GetUserMatrix(), self);
				}
				emit self->cursorPositionChanged(_xyz[0], _xyz[1], _xyz[2]);
			}

		};
		QList<Viewer*> Viewer::viewerList;
		Viewer* Viewer::getViewerOfInteractor(vtkInteractorStyle* style)
		{
			QList<Viewer*> viewersVect = Viewer::getAllViewers();
			typedef QList<Viewer*>::const_iterator Const_Iter;
			for (Const_Iter iter = viewersVect.cbegin(); iter != viewersVect.cend(); iter++)
			{
				if ((*iter)->getInteractor()->GetInteractorStyle() == style)
				{
					return (*iter);
				}
			}
			qCritical() << "Cannot find viewer for the interactor style: " << style;
			return nullptr;
		}

		void Viewer::updateAllViewers()
		{
			for (Viewer *viewer : viewerList) {
				viewer->update();
			}
		}

		void Viewer::updateViewersByGroup(int group)
		{
			foreach(Viewer* viewer, viewerList)
			{
				if (viewer->getViewerGroup() != group)
					continue;
				else
				{
					viewer->update();
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
			//for(IterType iter = viewerList.begin(); iter != viewerList.end();iter++)
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

		vtkRenderer* Viewer::addRenderer(int layer)
		{
			vtkRenderer* ren = vtkRenderer::New();
			//#TODO: Expand number of layer of renderwindo if layer > GetNumberofLayer()
			if (layer >= this->getRenderWindow()->GetNumberOfLayers()) {
				this->getRenderWindow()->SetNumberOfLayers(layer + 1);
			}
			addRenderer(ren, layer);
			return ren;
		}

		void Viewer::addRenderer(vtkRenderer* ren, int layer)
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
			if (this->getRenderWindow())
			{
				if (this->getRenderWindow()->GetNumberOfLayersMaxValue() < ren->GetLayer())
					this->getRenderWindow()->SetNumberOfLayers(ren->GetLayer());
				this->getRenderWindow()->AddRenderer(ren);
			}
		}

		void Viewer::removeRenderer(int index)
		{
			if (index < 0 || index > this->renderers.size()) {
				qWarning() << "Renderer requested to delete doesn't exist.";
				return;
			}
			vtkRenderer* ren = this->renderers[index];
			this->renderers.removeAt(index);
			removeAllProp(ren);
			this->getRenderWindow()->RemoveRenderer(ren);
			ren->Delete();
			qWarning() << "Renderer requested to delete doesn't exist.";
		}

		void Viewer::removeRenderer(vtkRenderer* ren)
		{
			int index = this->renderers.indexOf(ren);
			removeRenderer(index);
		}

		void Viewer::addProp(Prop * prop)
		{
			addProp(prop, this->renderers[0]);
		}

		void Viewer::addProp(Prop * prop, vtkRenderer * renderer)
		{
			if (!this->renderers.contains(renderer)) {
				qWarning() << "The renderer does not belong to this viewer.";
			}

			prop->setViewerID(viewerList.indexOf(this));
			prop->setRendererID(this->renderers.indexOf(renderer));
			prop->RenderingOn();
			renderer->AddViewProp(prop->getProp());
			propToRenderer->insert(prop, renderer);
		}

		QList<Prop*> Viewer::getProps()
		{
			return this->propToRenderer->keys();
		}

		void Viewer::removeProp(Prop * prop)
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


		void Viewer::removeAllProp()
		{
			for (QList<vtkRenderer*>::const_iterator cit = this->renderers.cbegin();
				cit != this->renderers.cend(); ++cit) {
				removeAllProp(*cit);
			}
		}

		void Viewer::removeAllProp(vtkRenderer * renderer)
		{
			if (!this->renderers.contains(renderer)) {
				qWarning() << "The renderer does not belong to this viewer.";
			}
			QList<Prop* > keys = this->propToRenderer->keys(renderer);

			for (QList<Prop* >::ConstIterator cit = keys.cbegin();
				cit != keys.cend(); ++cit) {
				removeProp(*cit);
			}
		}

		void Viewer::setCursorAlwaysFaceCamera(bool)
		{
			//throw std::logic_error("Not Implemented!");
			qCritical() << "Not Implemented!";
			Q_ASSERT("Not Implemented!");
			// #TODO: Implement this function 

			/*
			1. Create a new member transform
			2. Set new member transform as transform of actor
			3. Cocatenate rotation before translation
			*/
		}

		void Viewer::setCursorSyncFlag(bool sync)
		{
			if (this->cursorSyncFlag == sync) {
				return;
			}

			this->cursorSyncFlag = sync;
			if (!sync) {

				this->cursorActor->GetMapper()->SetInputConnection(
					this->getCursorSource()->GetOutputPort());
			}
			else {
				this->cursorActor->GetMapper()->SetInputConnection(
					Viewer::getSyncCursorSource()->GetOutputPort());

			}

		}


		void Viewer::setCursorPosition(double x, double y, double z)
		{
			if (x == this->getCursorSource()->GetFocalPoint()[0] &&
				y == this->getCursorSource()->GetFocalPoint()[1] &&
				z == this->getCursorSource()->GetFocalPoint()[2]) {
				return;
			}
			double xyz[3] = { x, y, z };
			this->getCursorSource()->InvokeEvent(vtkCommand::CursorChangedEvent, xyz);
		}

		const double* Viewer::getCursorPosition() const 
		{
			std::copy(this->getCursorSource()->GetFocalPoint(), this->getCursorSource()->GetFocalPoint() + 3, this->cursorPosition);
			if (this->cursorActor == nullptr) {
				 this->cursorActor->GetUserMatrix()->MultiplyPoint(this->cursorPosition, this->cursorPosition);
			}
			return this->cursorPosition;
		}

		vtkCursor3D * Q::vtk::Viewer::getCursorSource() const
		{
			if (this->getCursorSyncFlag()) {
				return syncCursorSource;
			}
			else {
				return this->cursorSource;
			}
		}

		void Q::vtk::Viewer::updateCursorPosition(double x, double y, double z)
		{
			if (this->getCornerAnnotation()->GetVisibility()) {
				QString format = "%1, %2, %3";
				QString text = format.arg(x, 0, 'f', 2).arg(y, 0, 'f', 2).arg(z, 0, 'f', 2);
				this->getCornerAnnotation()->SetText(1, text.toStdString().c_str());
			}
			this->update();
		}

		void Viewer::updateDepthPeeling()
		{
			// 1. Use a render window with alpha bits (as initial value is 0 (false)):
			this->getRenderWindow()->SetAlphaBitPlanes(this->depthPeelingFlag);
			if (this->depthPeelingFlag) {
				// 2. Force to not pick a framebuffer with a multisample buffer
				// (as initial value is 8):
				this->getRenderWindow()->SetMultiSamples(0);
			}
			else {
				this->getRenderWindow()->SetMultiSamples(8);
			}

			for(vtkRenderer *renderer: this->renderers){
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

		void Viewer::resetCamera()
		{
			qCritical() << "it not working well, camera is always defined by the last renderer";
			qCritical() << "layer must be specified.";
		}

		void Viewer::resetCamera(int layer /*= -1*/)
		{
			for(vtkRenderer *ren: this->renderers)
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

		void Viewer::resetCameraClippingRange(int layer /*= -1*/)
		{
			for(vtkRenderer *ren: this->renderers)
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

		void Viewer::resetCameraClippingRange()
		{
			qCritical() << "it not working well, camera is always defined by the last renderer";
			qCritical() << "layer must be specified.";
		}

		void Q::vtk::Viewer::update()
		{
			this->getRenderWindow()->Render();
			QWidget::update();
		}

		void Viewer::updateAllViewersOfThisClass()
		{
			typedef QList<Viewer *>::iterator IterType;
			for (IterType iter = this->viewerList.begin(); iter != this->viewerList.end(); iter++) {
				Viewer *viewer = *iter;
				if (this->metaObject()->className() == viewer->metaObject()->className()) {
					viewer->getRenderWindow()->Render();
				}
			}
		}

		void Viewer::setEnableCornerAnnotation(bool b)
		{
			this->getCornerAnnotation()->SetVisibility(b);
		}

		void Viewer::appendCornerAnnotation(int textPosition, QString text)
		{
			const char* oldText = this->cornerAnnotation->GetText(textPosition);
			this->cornerAnnotation->SetText(textPosition, (oldText + text).toStdString().c_str());
		}

		void Viewer::setMaxNoOfPeelings(int i)
		{
			if (this->maxNoOfPeels == i) {
				return;
			}
			this->maxNoOfPeels = i;
			this->updateDepthPeeling();
		}

		void Viewer::setOcclusionRatio(double ratio)
		{
			if (this->occlusionRatio == ratio) {
				return;
			}
			this->occlusionRatio = ratio;
			this->updateDepthPeeling();
		}

		void Viewer::setEnableDepthPeeling(bool flag)
		{
			if (this->depthPeelingFlag == flag) {
				return;
			}
			this->depthPeelingFlag = flag;
			this->updateDepthPeeling();
		}

		vtkCursor3D * Q::vtk::Viewer::getSyncCursorSource()
		{
			return syncCursorSource;
		}

		Viewer::Viewer(QWidget* parent)
			:QWidget(parent)
		{
			Viewer::viewerList.push_back(this);
			this->cursorSyncFlag = true;
			this->cursorCallback = CursorCallbackCommand::New();
			this->cursorCallback->SetClientData(this);
			connect(this, &Viewer::cursorPositionChanged,
				this, static_cast<void(Viewer::*)(double, double, double)>(&Viewer::updateCursorPosition));
			if (syncCursorSource == nullptr) {
				syncCursorSource = vtkCursor3D::New();
				syncCursorSource->TranslationModeOn();
				syncCursorSource->SetModelBounds(-15, 15, -15, 15, -15, 15);
				syncCursorSource->SetFocalPoint(0, 0, 0);
				syncCursorSource->AllOff();
				syncCursorSource->AxesOn();
			}
			syncCursorSource->AddObserver(vtkCommand::CursorChangedEvent, this->cursorCallback);
			this->cursorSource = vtkCursor3D::New();
			this->cursorSource->TranslationModeOn();
			this->cursorSource->SetModelBounds(-15, 15, -15, 15, -15, 15);
			this->cursorSource->SetFocalPoint(0, 0, 0);
			this->cursorSource->AllOff();
			this->cursorSource->AxesOn();
			this->cursorSource->AddObserver(vtkCommand::CursorChangedEvent, this->cursorCallback);
			this->cursorActor = vtkActor::New();
			this->cursorActor->SetMapper(vtkSmartPointer<vtkPolyDataMapper>::New());
			this->cursorActor->GetMapper()->SetInputConnection(
				syncCursorSource->GetOutputPort());
			this->cursorActor->GetProperty()->SetColor(1, 1, 0);
			this->cursorActor->SetPickable(false);
			this->cursorTransformFunction = nullptr;
			this->cornerAnnotation = vtkCornerAnnotation::New();
			this->cornerAnnotation->SetPickable(false);
			this->cornerAnnotation->SetMaximumFontSize(10);
			this->cornerAnnotation->SetMaximumLineHeight(0.3);
			this->cornerAnnotation->SetVisibility(false);
			this->camera = vtkCamera::New();
			this->propToRenderer = new PropToRenderer;
			this->depthPeelingFlag = true;
			this->maxNoOfPeels = 100;
			this->occlusionRatio = 0.1;
		}

		Viewer::~Viewer()
		{
			// Remove all renderers
			for (vtkRenderer *renderer : Viewer::renderers) {
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
			syncCursorSource->RemoveObserver(this->cursorCallback);
			this->cursorSource->Delete();
			this->cursorCallback->Delete();
			Viewer::viewerList.removeOne(this);
			if (numOfViewers() == 0) {
				syncCursorSource->Delete();
				syncCursorSource = nullptr;
			}
		}
		void Viewer::setupFirstRenderer(vtkRenderer *firstRenderer) {
			
			if (firstRenderer == nullptr) {
				firstRenderer = this->addRenderer(0);
			}
			firstRenderer->AddActor(this->cursorActor);
			firstRenderer->AddActor(this->cornerAnnotation);
			firstRenderer->SetActiveCamera(this->camera);
		}
	}
}