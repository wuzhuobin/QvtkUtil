// me
#include "QvtkNonPlanarViewer.h"
#include "QvtkImageSlice.h"

//vtk
#include <vtkOpenGLCamera.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

// qt 
#include <QDebug>

//void QvtkNonPlanarViewer::SetOrientation(int orientation)
//{
//	QList<QvtkAbstractProp*> props = this->propToRenderer->keys();
//
//	foreach(QvtkAbstractProp* prop, props) {
//		QvtkImageSlice* slice = qobject_cast<QvtkImageSlice*>(prop);
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
//	QvtkOrthogonalViewer::SetOrientation(orientation);
//}

QvtkNonPlanarViewer::QvtkNonPlanarViewer(QWidget * parent)
	:QvtkOrthogonalViewer(parent)
{
	//this->camera->ParallelProjectionOn();
	//vtkRenderer* ren = AddRenderer(2);
	//GetRenderers()[0]->RemoveActor(this->cursorActor);
	//ren->AddActor(this->cursorActor);
}

void QvtkNonPlanarViewer::UpdateCameraViewPlaneNormal()
{
	/* Get current view plane normal */
	double *ori = this->camera->GetViewPlaneNormal();
	double targetOri[3], diff[3];

	/* Force xViewPlaneNormal and zViewPlaneNormal to be orthogonal first */
	if (abs(vtkMath::Dot(s_sagitalViewPlaneNormal, s_axialViewPlaneNormal)) > 1e-10) {
		double rotAx[3];
		vtkMath::Cross(s_axialViewPlaneNormal, s_sagitalViewPlaneNormal, rotAx);
		vtkMath::Cross(s_axialViewPlaneNormal, rotAx, s_sagitalViewPlaneNormal);
		vtkMath::Normalize(s_sagitalViewPlaneNormal);
		memcpy(targetOri, s_sagitalViewPlaneNormal, sizeof(double) * 3);
	}

	/* Find new orientaiton of camera */
	switch (this->orientation) {
	case Axial:
		targetOri[0] = -s_axialViewPlaneNormal[0];
		targetOri[1] = -s_axialViewPlaneNormal[1];
		targetOri[2] = -s_axialViewPlaneNormal[2];
		break;
	case Sagital:
		/* do not change if z is already orthogonal with x */
		if (abs(vtkMath::Dot(s_sagitalViewPlaneNormal, s_axialViewPlaneNormal)) < 1e-10) {
			memcpy(targetOri, s_sagitalViewPlaneNormal, sizeof(double) * 3);
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
			if (abs(vtkMath::Dot(s_sagitalViewPlaneNormal, s_axialViewPlaneNormal)) < 1e-10) {
				vtkMath::Cross(s_axialViewPlaneNormal, s_sagitalViewPlaneNormal, s_coronalViewPlaneNormal);
				targetOri[0] = -s_coronalViewPlaneNormal[0];
				targetOri[1] = -s_coronalViewPlaneNormal[1];
				targetOri[2] = -s_coronalViewPlaneNormal[2];
			}
			else {
				std::string errmsg = "Corrupted x view planeNormal";
				qCritical() << QString::fromStdString(errmsg);
				throw std::logic_error(errmsg);
			}
		}
		else {
			if (abs(vtkMath::Dot(s_sagitalViewPlaneNormal, s_axialViewPlaneNormal)) < 1e-10) {
				vtkMath::Cross(s_sagitalViewPlaneNormal, s_axialViewPlaneNormal, s_coronalViewPlaneNormal);
				memcpy(targetOri, s_coronalViewPlaneNormal, sizeof(double) * 3);
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

//void QvtkNonPlanarViewer::SetCursorPosition(double x, double y, double z)
//{
//	QvtkOrthogonalViewer::SetCursorPosition(x, y, z);
//	if (this->desyncCursorFlag) 
//	{
//		return;
//	}
//	else {
//		foreach(QvtkAbstractViewer* viewer, GetAllViewers()) {
//			
//			QvtkNonPlanarViewer* _viewer = qobject_cast<QvtkNonPlanarViewer*>(viewer);
//			if (_viewer) {
//				QList<QvtkAbstractProp*> props = _viewer->propToRenderer->keys();
//
//				foreach(QvtkAbstractProp* prop, props) {
//					QvtkImageSlice* slice = qobject_cast<QvtkImageSlice*>(prop);
//					if (slice) {
//						double region[6];
//						slice->getDisplayRegion(region);
//
//						ORIENTATION _orientation = static_cast<ORIENTATION>(_viewer->orientation);
//						switch (_orientation)
//						{
//						case QvtkOrthogonalViewer::Sagital:
//							region[0] = x;
//							break;
//						case QvtkOrthogonalViewer::Coronal:
//							region[2] = y;
//							break;
//						case QvtkOrthogonalViewer::Axial:
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
