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
				if (this->viewer->getInteractor()->GetShiftKey() ||
					this->viewer->getInteractor()->GetControlKey() ||
					this->viewer->getInteractor()->GetAltKey())
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
			case SAGITTAL:
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
			this->orientationTextFlag = false;
			this->updateAxesFlag = false;
			this->sliceThickness = 1;
			this->getActiveCamera()->ParallelProjectionOn();
			vtkRenderer* ren = this->addRenderer(1);
			this->getRenderers()[0]->RemoveActor(this->getCursorActor());
			this->getRenderers()[0]->RemoveActor(this->getCornerAnnotation());
			this->GetAnnotationRenderer()->AddActor(this->getCursorActor());
			this->GetAnnotationRenderer()->AddActor(this->getCornerAnnotation());
			for (int i = 0; i < 4; i++)
			{
				this->orientationActor[i] = vtkTextActor::New();
				this->orientationActor[i]->GetTextProperty()->SetColor(1, 0.749, 0);
				this->orientationActor[i]->GetTextProperty()->SetFontSize(10);
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
			this->verticalAxis = vtkAxisActor2D::New();
			this->verticalAxis->SetNumberOfMinorTicks(0);
			//this->verticalAxis->RulerModeOff()
			this->verticalAxis->SetTickLength(3);
			//this->verticalAxis->SetNumberOfLabels(11);
			//this->verticalAxis->SetTitleVisibility(false);
			//this->verticalAxis->AdjustLabelsOff();
			//this->verticalAxis->LabelVisibilityOff();
			this->verticalAxis->GetProperty()->SetColor(0.2, 1, 0.2);
			this->GetAnnotationRenderer()->AddActor2D(this->verticalAxis);
			//Set axis position on display
			this->horizontalAxis = vtkAxisActor2D::New();
			this->horizontalAxis->SetNumberOfMinorTicks(0);
			this->horizontalAxis->SetTickLength(3);
			//this->horizontalAxis->SetNumberOfLabels(11);
			//this->horizontalAxis->SetTitleVisibility(false);
			//this->horizontalAxis->RulerModeOff();
			//this->horizontalAxis->AdjustLabelsOff();
			//this->horizontalAxis->LabelVisibilityOff();
			this->horizontalAxis->GetProperty()->SetColor(0.2, 1, 0.2);
			this->GetAnnotationRenderer()->AddActor2D(this->horizontalAxis);
			//Set axis position on display
			this->setUpdateAxesFlag(this->updateAxesFlag);
			QvtkPlanarViewerAxisCallback* axisCallback = QvtkPlanarViewerAxisCallback::New();
			axisCallback->viewer = this;
			this->getActiveCamera()->AddObserver(vtkCommand::ModifiedEvent, axisCallback);
			axisCallback->Delete();
			PlanarViewerScrollCallback* callback = PlanarViewerScrollCallback::New();
			callback->viewer = this;
			this->getInteractor()->AddObserver(vtkCommand::MouseWheelBackwardEvent, callback, 0.6);
			this->getInteractor()->AddObserver(vtkCommand::MouseWheelForwardEvent, callback, 0.6);
			callback->Delete();
			this->setViewPlaneNormalSyncFlag(false);
			this->setAxialViewPlaneNormal(0, 0, -1);
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
			double pdist = this->getActiveCamera()->GetParallelScale(); // Parallel scale: the height of the viewport 
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
				//this->orientationActor[i]->SetVisibility(flag);
				this->orientationActor[i]->SetVisibility(false);
			}
			ORIENTATION _orientation = static_cast<ORIENTATION>(this->getOrientation());
			char orientationText[4][2];
			orientationText[0][1] = '\0';
			orientationText[1][1] = '\0';
			orientationText[2][1] = '\0';
			orientationText[3][1] = '\0';
			if (this->orientationTextFlag) {
				switch (_orientation)
				{
				case OrthogonalViewer::SAGITTAL:
					orientationText[0][0] = 'S';
					orientationText[1][0] = 'I';
					orientationText[2][0] = 'A';
					orientationText[3][0] = 'P';
					this->orientationActor[0]->SetInput("Superior");
					this->orientationActor[1]->SetInput("Inferior");
					this->orientationActor[2]->SetInput("Anterior");
					this->orientationActor[3]->SetInput("Posterior");
					break;
				case OrthogonalViewer::CORONAL:
					orientationText[0][0] = 'S';
					orientationText[1][0] = 'I';
					orientationText[2][0] = 'R';
					orientationText[3][0] = 'L';
					this->orientationActor[0]->SetInput("Superior");
					this->orientationActor[1]->SetInput("Inferior");
					this->orientationActor[2]->SetInput("Right");
					this->orientationActor[3]->SetInput("Left");
					break;
				case OrthogonalViewer::AXIAL:
					orientationText[0][0] = 'A';
					orientationText[1][0] = 'P';
					orientationText[2][0] = 'R';
					orientationText[3][0] = 'L';
					this->orientationActor[0]->SetInput("Anterior");
					this->orientationActor[1]->SetInput("Posterior");
					this->orientationActor[2]->SetInput("Right");
					this->orientationActor[3]->SetInput("Left");
					break;
				default:
					orientationText[0][0] = '\0';
					orientationText[1][0] = '\0';
					orientationText[2][0] = '\0';
					orientationText[3][0] = '\0';
					this->orientationActor[0]->SetInput("");
					this->orientationActor[1]->SetInput("");
					this->orientationActor[2]->SetInput("");
					this->orientationActor[3]->SetInput("");
					break;
				}
			}
			else {
				orientationText[0][0] = '\0';
				orientationText[1][0] = '\0';
				orientationText[2][0] = '\0';
				orientationText[3][0] = '\0';
			}
			this->getCornerAnnotation()->SetText(vtkCornerAnnotation::UpperEdge, orientationText[0]);
			this->getCornerAnnotation()->SetText(vtkCornerAnnotation::LowerEdge, orientationText[1]);
			this->getCornerAnnotation()->SetText(vtkCornerAnnotation::LeftEdge, orientationText[2]);
			this->getCornerAnnotation()->SetText(vtkCornerAnnotation::RightEdge, orientationText[3]);
		}

		void PlanarViewer::updateCursorPosition(double x, double y, double z)
		{
			QList<Prop*> props = this->propToRenderer->keys();

			foreach(Prop* prop, props) {
				PlanarProp* planarProp = qobject_cast<PlanarProp*>(prop);
				if (planarProp) {
					planarProp->setPlanarOrigin(x, y, z);
				}
			}
			OrthogonalViewer::updateCursorPosition(x, y, z);
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
				pos[0] += (sign ? 1 * this->sliceThickness : -1 * this->sliceThickness);
				break;
			case OrthogonalViewer::ORIENTATION_XZ:
				pos[1] += (sign ? 1 * this->sliceThickness : -1 * this->sliceThickness);
				break;
			case OrthogonalViewer::ORIENTATION_XY:
				pos[2] += (sign ? 1 * this->sliceThickness : -1 * this->sliceThickness);
				break;
			case OrthogonalViewer::SAGITTAL:
			case OrthogonalViewer::CORONAL:
			case OrthogonalViewer::AXIAL: {
				double translation[3];
				std::copy(this->getCurrentPlaneNormal(), this->getCurrentPlaneNormal() + 3, translation);
				vtkMath::MultiplyScalar(translation, this->sliceThickness);
				if (sign) {
					vtkMath::Add(pos, translation, pos);
				}
				else {
					vtkMath::Subtract(pos, translation, pos);
				}
			}
			default:
				break;
			}
			this->setCursorPosition(pos[0], pos[1], pos[2]);
			this->getRenderWindow()->Render();
		}

		double* vtk::PlanarViewer::UpdateViewUp()
		{
			switch (this->getOrientation()) {
			case AXIAL:
				this->getActiveCamera()->SetViewUp(0, -1, 0);
				break;
			case CORONAL:
			case SAGITTAL:
				this->getActiveCamera()->SetViewUp(0, 0, 1);
				break;
			default: 
				return OrthogonalViewer::UpdateViewUp();
			}
			return this->getActiveCamera()->GetViewUp();
		}
	}
}