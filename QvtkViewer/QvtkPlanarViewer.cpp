// me
#include "QvtkPlanarViewer.h"
#include "QvtkPlanarProp.h"
//vtk
#include <vtkOpenGLCamera.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkCommand.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkAxisActor2D.h>
#include <vtkProperty2D.h>
#include <vtkCornerAnnotation.h>
// qt
#include <QDebug>
namespace Q {
	namespace vtk {
		class PlanarViewerScrollCallback : public vtkCommand
		{
		public:
			static PlanarViewerScrollCallback* New() {
				return new PlanarViewerScrollCallback;
			}

			virtual void Execute(vtkObject *caller, unsigned long eventId,
				void *callData) override {
				// Do not process if any modifiers are ON
				if (this->viewer->GetInteractor()->GetShiftKey() ||
					this->viewer->GetInteractor()->GetControlKey() ||
					this->viewer->GetInteractor()->GetAltKey())
				{
					return;
				}
				// forwards or backwards
				bool sign = (eventId == vtkCommand::MouseWheelForwardEvent) ? true : false;
				this->viewer->IncrementSlice(sign);
				// Abort further event processing for the scroll.
				this->AbortFlagOn();
			}
			PlanarViewer* viewer;
		};

		class QvtkPlanarViewerAxisCallback : public vtkCommand
		{
		public:
			static QvtkPlanarViewerAxisCallback* New() {
				return new QvtkPlanarViewerAxisCallback;
			}

			virtual void Execute(vtkObject *caller, unsigned long eventId,
				void *callData) override {
				this->viewer->setUpdateAxesFlag(this->viewer->getUpdateAxesFlag());
			}
			PlanarViewer* viewer;
		};

		void PlanarViewer::setOrientation(int orientation)
		{
			OrthogonalViewer::setOrientation(orientation);
			switch (orientation)
			{
			case ORIENTATION_XZ:
			case ORIENTATION_YZ:
			case ORIENTATION_XY: {
				QList<Prop*> props = this->propToRenderer->keys();
				for (Prop *prop : props) {
					PlanarProp* slice = qobject_cast<PlanarProp*>(prop);
					if (slice) {
						slice->setPlanarOrientation(orientation);
					}
				}
				break;
			}
			case SAGITAL:
			case CORONAL:
			case AXIAL: {
				QList<Prop*> props = this->propToRenderer->keys();
				for (Prop *prop : props) {
					PlanarProp* slice = qobject_cast<PlanarProp*>(prop);
					if (slice) {
						slice->setPlanarOrientationToOblique();
						slice->setPlanarNormal(this->getCurrentPlaneNormal());
					}
				}
				break;
			}
			default:
				break;
			}
			this->setOrientationTextFlag(this->orientationTextFlag);
			this->update();
		}

		PlanarViewer::PlanarViewer(QWidget * parent)
			:OrthogonalViewer(parent)
		{
			this->camera->ParallelProjectionOn();
			vtkRenderer* ren = this->addRenderer(1);
			this->GetRenderers()[0]->RemoveActor(this->cursorActor);
			this->GetRenderers()[0]->RemoveActor(this->cornerAnnotation);
			this->GetAnnotationRenderer()->AddActor(this->cursorActor);
			this->GetAnnotationRenderer()->AddActor(this->cornerAnnotation);
			this->orientationTextFlag = false;
			for (int i = 0; i < 4; i++)
			{
				this->orientationActor[i] = vtkTextActor::New();
				this->orientationActor[i]->GetTextProperty()->SetColor(1, 0.749, 0);
				this->orientationActor[i]->GetTextProperty()->SetFontSize(15);
				this->orientationActor[i]->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
				this->GetAnnotationRenderer()->AddActor2D(this->orientationActor[i]);
			}
			// superior	
			this->orientationActor[0]->SetPosition(0.49, 0.95);
			// inferior 
			this->orientationActor[1]->SetPosition(0.49, 0.03);
			// left
			this->orientationActor[2]->SetPosition(0.03, 0.48);
			// right
			this->orientationActor[3]->SetPosition(0.98, 0.48);
			// axisActor;
			this->setOrientationTextFlag(this->orientationTextFlag);
			////////////////////////////////////////////////////
			this->updateAxesFlag = false;
			this->verticalAxis = vtkAxisActor2D::New();
			this->verticalAxis->SetNumberOfMinorTicks(0);
			this->verticalAxis->RulerModeOff();
			this->verticalAxis->SetTickLength(3);
			this->verticalAxis->SetNumberOfLabels(11);
			this->verticalAxis->SetTitleVisibility(false);
			this->verticalAxis->AdjustLabelsOff();
			this->verticalAxis->LabelVisibilityOff();
			this->verticalAxis->GetProperty()->SetColor(0.2, 1, 0.2);
			this->GetAnnotationRenderer()->AddActor2D(this->verticalAxis);
			//Set axis position on display
			this->horizontalAxis = vtkAxisActor2D::New();
			this->horizontalAxis->SetNumberOfMinorTicks(0);
			this->horizontalAxis->SetTickLength(3);
			this->horizontalAxis->SetNumberOfLabels(11);
			this->horizontalAxis->SetTitleVisibility(false);
			this->horizontalAxis->RulerModeOff();
			this->horizontalAxis->AdjustLabelsOff();
			this->horizontalAxis->LabelVisibilityOff();
			this->horizontalAxis->GetProperty()->SetColor(0.2, 1, 0.2);
			this->GetAnnotationRenderer()->AddActor2D(this->horizontalAxis);
			//Set axis position on display
			this->setUpdateAxesFlag(this->updateAxesFlag);
			QvtkPlanarViewerAxisCallback* axisCallback = QvtkPlanarViewerAxisCallback::New();
			axisCallback->viewer = this;
			this->GetActiveCamera()->AddObserver(vtkCommand::ModifiedEvent, axisCallback);
			axisCallback->Delete();
			PlanarViewerScrollCallback* callback = PlanarViewerScrollCallback::New();
			callback->viewer = this;
			this->GetInteractor()->AddObserver(vtkCommand::MouseWheelBackwardEvent, callback, 0.6);
			this->GetInteractor()->AddObserver(vtkCommand::MouseWheelForwardEvent, callback, 0.6);
			callback->Delete();
			this->setSyncViewPlaneNormalFlag(false);
			this->SetAxialViewPlaneNormal(0, 0, -1);
		}
		PlanarViewer::~PlanarViewer()
		{
			for (int i = 0; i < 4; i++)
			{
				this->GetAnnotationRenderer()->RemoveActor(this->orientationActor[i]);
				this->orientationActor[i]->Delete();
			}
			this->GetAnnotationRenderer()->RemoveActor(this->verticalAxis);
			this->verticalAxis->Delete();
			this->GetAnnotationRenderer()->RemoveActor(this->horizontalAxis);
			this->horizontalAxis->Delete();
		}

		void PlanarViewer::setUpdateAxesFlag(bool flag)
		{
			this->updateAxesFlag = flag;
			this->verticalAxis->SetVisibility(flag);
			this->horizontalAxis->SetVisibility(flag);
			if (!flag) {
				return;
			}
			// set the scale corresponding to world coordinate distance
			double pdist = this->GetActiveCamera()->GetParallelScale(); // Parallel scale: the height of the viewport 
			int *size = this->getRenderWindow()->GetSize();			  // (focal point to boarder)in world-coordinate distances
			double horizontal = *(size);
			double vertical = *(size + 1);
			double h2vratio = horizontal / vertical;
			double v2hratio = vertical / horizontal;
			this->verticalAxis->SetPoint1(0.01, 0.5 - (25 * (1 / pdist)));		// Parallel scale * length of scalebar on display * 2
			this->verticalAxis->SetPoint2(0.01, 0.5 + (25 * (1 / pdist)));	// Parallel scale * length of scalebar on display * 2
			this->horizontalAxis->SetPoint1(0.5 + v2hratio * (25 * (1 / pdist)), 0.01);
			this->horizontalAxis->SetPoint2(0.5 - v2hratio * (25 * (1 / pdist)), 0.01);
		}
		void vtk::PlanarViewer::setOrientationTextFlag(bool flag)
		{
			this->orientationTextFlag = flag;
			for (int i = 0; i < 4; ++i) {
				this->orientationActor[i]->SetVisibility(flag);
			}
			if (!this->orientationTextFlag) {
				return;
			}
			ORIENTATION _orientation = static_cast<ORIENTATION>(this->getOrientation());
			switch (_orientation)
			{
			case OrthogonalViewer::SAGITAL:
				this->orientationActor[0]->SetInput("S");
				this->orientationActor[1]->SetInput("I");
				this->orientationActor[2]->SetInput("A");
				this->orientationActor[3]->SetInput("P");
				break;
			case OrthogonalViewer::CORONAL:
				this->orientationActor[0]->SetInput("S");
				this->orientationActor[1]->SetInput("I");
				this->orientationActor[2]->SetInput("R");
				this->orientationActor[3]->SetInput("L");
				break;
			case OrthogonalViewer::AXIAL:
				this->orientationActor[0]->SetInput("A");
				this->orientationActor[1]->SetInput("P");
				this->orientationActor[2]->SetInput("R");
				this->orientationActor[3]->SetInput("L");
				break;
				break;
			default:
				break;
			}
		}

		void PlanarViewer::UpdateCursorPosition(double x, double y, double z)
		{
			QList<Prop*> props = this->propToRenderer->keys();

			foreach(Prop* prop, props) {
				PlanarProp* planarProp = qobject_cast<PlanarProp*>(prop);
				if (planarProp) {
					planarProp->setPlanarOrigin(x, y, z);
				}
			}
			OrthogonalViewer::UpdateCursorPosition(x, y, z);
		}

		void PlanarViewer::IncrementSlice(bool sign)
		{
			double pos[3] = {
				getCursorPosition()[0],
				getCursorPosition()[1],
				getCursorPosition()[2]
			};
			ORIENTATION orientation = static_cast<ORIENTATION>(this->getOrientation());
			switch (orientation)
			{
			case OrthogonalViewer::ORIENTATION_YZ:
				pos[0] += (sign ? 1 : -1);
				break;
			case OrthogonalViewer::ORIENTATION_XZ:
				pos[1] += (sign ? 1 : -1);
				break;
			case OrthogonalViewer::ORIENTATION_XY:
				pos[2] += (sign ? 1 : -1);
				break;
			case OrthogonalViewer::SAGITAL:
			case OrthogonalViewer::CORONAL:
			case OrthogonalViewer::AXIAL: {
				if (sign) {
					vtkMath::Add(pos, this->getCurrentPlaneNormal(), pos);
				}
				else {
					vtkMath::Subtract(pos, this->getCurrentPlaneNormal(), pos);
				}
			}
			default:
				break;
			}
			this->SetCursorPosition(pos[0], pos[1], pos[2]);
			this->getRenderWindow()->Render();
		}

		double* vtk::PlanarViewer::UpdateViewUp()
		{
			switch (this->getOrientation()) {
			case AXIAL:
				this->camera->SetViewUp(0, -1, 0);
				break;
			case CORONAL:
			case SAGITAL:
				this->camera->SetViewUp(0, 0, 1);
				break;
			default: 
				return OrthogonalViewer::UpdateViewUp();
			}
			return this->camera->GetViewUp();
		}
	}
}