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
namespace vtk{
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
		this->viewer->UpdateAxes(true);
	}
	PlanarViewer* viewer;
};

void PlanarViewer::SetOrientation(int orientation)
{
	ORIENTATION _orientation = static_cast<ORIENTATION>(orientation);
	switch (_orientation)
	{
	case OrthogonalViewer::Sagital:
		this->orientationActor[0]->SetInput("S");
		this->orientationActor[1]->SetInput("I");
		this->orientationActor[2]->SetInput("A");
		this->orientationActor[3]->SetInput("P");
		break;
	case OrthogonalViewer::Coronal:
		this->orientationActor[0]->SetInput("S");
		this->orientationActor[1]->SetInput("I");
		this->orientationActor[2]->SetInput("R");
		this->orientationActor[3]->SetInput("L");
		break;
	case OrthogonalViewer::Axial:
		this->orientationActor[0]->SetInput("A");
		this->orientationActor[1]->SetInput("P");
		this->orientationActor[2]->SetInput("R");
		this->orientationActor[3]->SetInput("L");
		break;
		break;
	default:
		break;
	}

	QList<Prop*> props = this->propToRenderer->keys();

	foreach(Prop* prop, props) {
		PlanarProp* slice = qobject_cast<PlanarProp*>(prop);
		if (slice) {
			slice->setPlanarOrientation(orientation);
		}
	}
	OrthogonalViewer::SetOrientation(orientation);
}

PlanarViewer::PlanarViewer(QWidget * parent)
	:OrthogonalViewer(parent)
{
	this->camera->ParallelProjectionOn();



	vtkRenderer* ren = AddRenderer(2);
	this->GetRenderers()[0]->RemoveActor(this->cursorActor);
	this->GetRenderers()[0]->RemoveActor(this->cornerAnnotation);
	this->GetAnnotationRenderer()->AddActor(this->cursorActor);
	this->GetAnnotationRenderer()->AddActor(this->cornerAnnotation);
	
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
	////////////////////////////////////////////////////
	this->verticalAxis = vtkAxisActor2D::New();

	this->verticalAxis->SetNumberOfMinorTicks(0);
	this->verticalAxis->RulerModeOff();
	this->verticalAxis->SetTickLength(3);
	this->verticalAxis->SetNumberOfLabels(11);
	this->verticalAxis->SetTitleVisibility(false);
	this->verticalAxis->AdjustLabelsOff();
	this->verticalAxis->LabelVisibilityOff();

	//Set axis position on display
	//this->verticalAxis->SetPoint1(0.03, 0.3);
	//this->verticalAxis->SetPoint2(0.03, 0.7);
	this->verticalAxis->GetProperty()->SetColor(0.2, 1, 0.2);

	this->GetAnnotationRenderer()->AddActor2D(this->verticalAxis);

	this->horizontalAxis = vtkAxisActor2D::New();

	this->horizontalAxis->SetNumberOfMinorTicks(0);
	this->horizontalAxis->SetTickLength(3);
	this->horizontalAxis->SetNumberOfLabels(11);
	this->horizontalAxis->SetTitleVisibility(false);
	this->horizontalAxis->RulerModeOff();
	this->horizontalAxis->AdjustLabelsOff();
	this->horizontalAxis->LabelVisibilityOff();
	//Set axis position on display
	//this->horizontalAxis->SetPoint1(0.3, 0.95);
	//this->horizontalAxis->SetPoint2(0.7, 0.95);
	this->horizontalAxis->GetProperty()->SetColor(0.2, 1, 0.2);

	this->GetAnnotationRenderer()->AddActor2D(this->horizontalAxis);
	this->UpdateAxes(false);
	
	QvtkPlanarViewerAxisCallback* axisCallback = QvtkPlanarViewerAxisCallback::New();
	axisCallback->viewer = this;
	this->GetActiveCamera()->AddObserver(vtkCommand::ModifiedEvent, axisCallback);
	axisCallback->Delete();

	PlanarViewerScrollCallback* callback = PlanarViewerScrollCallback::New();
	callback->viewer = this;
	this->GetInteractor()->AddObserver(vtkCommand::MouseWheelBackwardEvent, callback, 0.6);
	this->GetInteractor()->AddObserver(vtkCommand::MouseWheelForwardEvent, callback, 0.6);
	callback->Delete();

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

void PlanarViewer::UpdateAxes(bool flag)
{
	this->verticalAxis->SetVisibility(flag);
	this->horizontalAxis->SetVisibility(flag);
	if (!flag) {
		return;
	}
	// set the scale corresponding to world coordinate distance
	double pdist = this->GetActiveCamera()->GetParallelScale(); // Parallel scale: the height of the viewport 
	int *size = this->GetRenderWindow()->GetSize();			  // (focal point to boarder)in world-coordinate distances
	double horizontal = *(size);
	double vertical = *(size + 1);
	double h2vratio = horizontal / vertical;
	double v2hratio = vertical / horizontal;
	this->verticalAxis->SetPoint1(0.01, 0.5 - (25 * (1 / pdist)));		// Parallel scale * length of scalebar on display * 2
	this->verticalAxis->SetPoint2(0.01, 0.5 + (25 * (1 / pdist)));	// Parallel scale * length of scalebar on display * 2
	this->horizontalAxis->SetPoint1(0.5 + v2hratio * (25 * (1 / pdist)), 0.01);
	this->horizontalAxis->SetPoint2(0.5 - v2hratio * (25 * (1 / pdist)), 0.01);
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
		GetCursorPosition()[0],
		GetCursorPosition()[1],
		GetCursorPosition()[2]
	};
	ORIENTATION orientation = static_cast<ORIENTATION>(GetOrientation());
	switch (orientation)
	{
	case OrthogonalViewer::Sagital:
		pos[0] += (sign ? 1 : -1);
		break;
	case OrthogonalViewer::Coronal:
		pos[1] += (sign ? 1 : -1);
		break;
	case OrthogonalViewer::Axial:
		pos[2] += (sign ? 1 : -1);
		break;
	default:
		break;
	}
	this->SetCursorPosition(pos[0], pos[1], pos[2]);
	this->GetRenderWindow()->Render();
	//emit signalIncrementSlice();
}

void PlanarViewer::UpdateCameraViewPlaneNormal()
{
	/* Get current view plane normal */
	double *ori = this->camera->GetViewPlaneNormal();
	double targetOri[3], diff[3];

	/* Force xViewPlaneNormal and zViewPlaneNormal to be orthogonal first */
	if (abs(vtkMath::Dot(sagitalViewPlaneNormal, axialViewPlaneNormal)) > 1e-10) {
		double rotAx[3];
		vtkMath::Cross(axialViewPlaneNormal, sagitalViewPlaneNormal, rotAx);
		vtkMath::Cross(axialViewPlaneNormal, rotAx, sagitalViewPlaneNormal);
		vtkMath::Normalize(sagitalViewPlaneNormal);
		memcpy(targetOri, sagitalViewPlaneNormal, sizeof(double) * 3);
	}

	/* Find new orientaiton of camera */
	switch (this->orientation) {
	case Axial:
		targetOri[0] = -axialViewPlaneNormal[0];
		targetOri[1] = -axialViewPlaneNormal[1];
		targetOri[2] = -axialViewPlaneNormal[2];
		break;
	case Sagital:
		/* do not change if z is already orthogonal with x */
		if (abs(vtkMath::Dot(sagitalViewPlaneNormal, axialViewPlaneNormal)) < 1e-10) {
			memcpy(targetOri, sagitalViewPlaneNormal, sizeof(double) * 3);
		}
		else {
			std::string errmsg = "Corrupted x view planeNormal";
			qCritical() << QString::fromStdString(errmsg);
			throw std::logic_error(errmsg);
		}
		break;
	case Coronal:
		if (this->righthandness)
		{
			if (abs(vtkMath::Dot(sagitalViewPlaneNormal, axialViewPlaneNormal)) < 1e-10) {
				vtkMath::Cross(axialViewPlaneNormal, sagitalViewPlaneNormal, coronalViewPlaneNormal);
				targetOri[0] = -coronalViewPlaneNormal[0];
				targetOri[1] = -coronalViewPlaneNormal[1];
				targetOri[2] = -coronalViewPlaneNormal[2];
			}
			else {
				std::string errmsg = "Corrupted x view planeNormal";
				qCritical() << QString::fromStdString(errmsg);
				throw std::logic_error(errmsg);
			}
		}
		else {
			if (abs(vtkMath::Dot(sagitalViewPlaneNormal, axialViewPlaneNormal)) < 1e-10) {
				vtkMath::Cross(sagitalViewPlaneNormal, axialViewPlaneNormal, coronalViewPlaneNormal);
				memcpy(targetOri, coronalViewPlaneNormal, sizeof(double) * 3);
			}
			else {
				std::string errmsg = "Corrupted x view planeNormal";
				qCritical() << QString::fromStdString(errmsg);
				throw std::logic_error(errmsg);
			}
		}
		break;
	default: std::string errmsg = "ORIENTATION is not between 0 to 2!";
		qCritical() << QString::fromStdString(errmsg);
		throw std::logic_error(errmsg);
		break;
	}
	vtkMath::Normalize(targetOri);

	/* Set new position to camera */
	double dist = this->camera->GetDistance();
	double *fp = this->camera->GetFocalPoint();
	double newCamPos[3];
	vtkMath::MultiplyScalar(targetOri, dist);
	vtkMath::Add(fp, targetOri, newCamPos);
	this->camera->SetPosition(newCamPos);


	/* Specify View up */
	this->UpdateViewUp();
}
}
}