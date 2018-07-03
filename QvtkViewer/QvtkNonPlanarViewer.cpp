// me
#include "QvtkNonPlanarViewer.h"
#include "QvtkImageSlice.h"
//vtk
#include <vtkOpenGLCamera.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkAxesActor.h>
// qt 
#include <QDebug>
namespace Q {
	namespace vtk {
		NonPlanarViewer::NonPlanarViewer(QWidget * parent)
			:OrthogonalViewer(parent)
		{
			this->axesActor = vtkAxesActor::New();
			this->axesActor->SetXAxisLabelText("L");
			this->axesActor->SetYAxisLabelText("P");
			this->axesActor->SetZAxisLabelText("S");
			this->orientationMarkerWidget = vtkOrientationMarkerWidget::New();
			this->orientationMarkerWidget->SetPickingManaged(false);
			this->orientationMarkerWidget->SetOrientationMarker(this->axesActor);
			this->orientationMarkerWidget->SetViewport(0.0, 0.0, 0.2, 0.2);
			this->orientationMarkerWidgetFlag = false;
			this->setOrientationMarkerWidgetFlag(this->orientationMarkerWidgetFlag);
		}

		NonPlanarViewer::~NonPlanarViewer()
		{
			this->orientationMarkerWidget->Delete();
			this->axesActor->Delete();
		}

		void NonPlanarViewer::setOrientationMarkerWidgetFlag(bool flag)
		{
			this->orientationMarkerWidgetFlag = flag;
			if (flag) {
				this->orientationMarkerWidget->SetInteractor(this->getInteractor());
				this->orientationMarkerWidget->SetEnabled(flag);
			}
			else {
				this->orientationMarkerWidget->SetInteractor(nullptr);
			}
		}
	}
}