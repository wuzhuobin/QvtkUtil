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
				this->orientationMarkerWidget->SetInteractor(this->GetInteractor());
			}
			else {
				this->orientationMarkerWidget->SetInteractor(nullptr);
			}
			this->orientationMarkerWidget->SetEnabled(flag);
		}

		void NonPlanarViewer::UpdateCameraViewPlaneNormal()
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