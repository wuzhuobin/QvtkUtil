// me
#include "QvtkNonPlanarViewer.h"
#include "QvtkImageSlice.h"

//vtk
#include <vtkOpenGLCamera.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

// qt 
#include <QDebug>

//void NonPlanarViewer::SetOrientation(int orientation)
//{
//	QList<Prop*> props = this->propToRenderer->keys();
//
//	foreach(Prop* prop, props) {
//		ImageSlice* slice = qobject_cast<ImageSlice*>(prop);
//		if (slice) {
//			slice->setOrientation(orientation);
//
//			double region[6];
//			slice->getDisplayRegion(region);
//			double* pos = GetCursorPosition();
//			region[orientation * 2] = pos[orientation];
//			slice->setDisplayRegion(region);
//		}
//	}
//	OrthogonalViewer::SetOrientation(orientation);
//}
namespace Q {
namespace vtk{
NonPlanarViewer::NonPlanarViewer(QWidget * parent)
	:OrthogonalViewer(parent)
{
	//this->camera->ParallelProjectionOn();
	//vtkRenderer* ren = AddRenderer(2);
	//GetRenderers()[0]->RemoveActor(this->cursorActor);
	//ren->AddActor(this->cursorActor);
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

//void NonPlanarViewer::SetCursorPosition(double x, double y, double z)
//{
//	OrthogonalViewer::SetCursorPosition(x, y, z);
//	if (this->desyncCursorFlag) 
//	{
//		return;
//	}
//	else {
//		foreach(Viewer* viewer, GetAllViewers()) {
//			
//			NonPlanarViewer* _viewer = qobject_cast<NonPlanarViewer*>(viewer);
//			if (_viewer) {
//				QList<Prop*> props = _viewer->propToRenderer->keys();
//
//				foreach(Prop* prop, props) {
//					ImageSlice* slice = qobject_cast<ImageSlice*>(prop);
//					if (slice) {
//						double region[6];
//						slice->getDisplayRegion(region);
//
//						ORIENTATION _orientation = static_cast<ORIENTATION>(_viewer->orientation);
//						switch (_orientation)
//						{
//						case OrthogonalViewer::Sagital:
//							region[0] = x;
//							break;
//						case OrthogonalViewer::Coronal:
//							region[2] = y;
//							break;
//						case OrthogonalViewer::Axial:
//							region[4] = z;
//							break;
//						default:
//							break;
//						}
//						slice->setDisplayRegion(region);
//					}
//				}
//			}
//		}
//	}
//
//
//}

}
}