#include "QvtkOrthogonalViewer.h"
#include "ui_QvtkOrthogonalViewer.h"

//vtk
#include <vtkCamera.h>
#include <vtkOpenGLCamera.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <QVTKInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include <vtkMath.h>
//#include <vtkRenderer.h>

//qt
#include <QLayout>
#include <QDebug>

bool QvtkOrthogonalViewer::initializeFlag = false;

/* Set default value of x and y view plane normal */
double QvtkOrthogonalViewer::s_axialViewPlaneNormal[3] = { 0, 0, 1 };
double QvtkOrthogonalViewer::s_sagitalViewPlaneNormal[3] = { 1, 0, 0 };
double QvtkOrthogonalViewer::s_coronalViewPlaneNormal[3] = { 0, 1, 0 };

const struct QvtkOrthogonalViewerResourceInit
{
	QvtkOrthogonalViewerResourceInit() {
		Q_INIT_RESOURCE(AbstractViewer);
	}

} INIT;

void QvtkOrthogonalViewer::SetOrientation(int orientation)
{

    if (orientation < 0) {
        orientation = Axial;
		qWarning() << "Trying to set orientation < 0, falling back to axial";
    }
    else if (orientation > 2) {
        orientation = Sagital;
		qWarning() << "Trying to set orientation > 2, falling back to sagital.";
    }
	ORIENTATION _orientation = static_cast<ORIENTATION>(orientation);
	switch (_orientation)
	{
	case QvtkOrthogonalViewer::Axial:
		this->ui->pushBtnAxial->setChecked(true);
		break;
	case QvtkOrthogonalViewer::Coronal:
		this->ui->pushBtnCoronal->setChecked(true);
		break;
	case QvtkOrthogonalViewer::Sagital:
		this->ui->pushBtnSagital->setChecked(true);
		break;
	default:
		break;
	}
    this->orientation = _orientation;
	emit OrientationChanged(this->orientation);
    this->UpdateCameraViewPlaneNormal();
	/* Render */
	this->GetRenderWindow()->Render();
}

int QvtkOrthogonalViewer::GetOrientation()
{
    return this->orientation;
}

QvtkOrthogonalViewer::QvtkOrthogonalViewer(QWidget *parent)
    : QvtkAbstractViewer(parent)
{
    this->ui = new Ui::QvtkOrthogonalViewer;
    this->ui->setupUi(this);

	this->righthandness = true;
	this->orientation = Axial;

    Qvtk_INIT_VIEWER_MACRO();

    this->InitializeStaticVariables();

    connect(this->ui->pushBtnAxial, SIGNAL(clicked()),
            this, SLOT(SetOrientationToAxial()));
    connect(this->ui->pushBtnCoronal, SIGNAL(clicked()),
            this, SLOT(SetOrientationToCoronal()));
    connect(this->ui->pushBtnSagital, SIGNAL(clicked()),
            this, SLOT(SetOrientationToSagital()));
    connect(this->ui->pushBtnMaximize, SIGNAL(clicked()),
            this, SLOT(slotPushBtnMaximize()));
    connect(this->ui->pushBtnNormal, SIGNAL(clicked()),
            this, SLOT(slotpushBtnNormal()));
    connect(this->ui->pushBtnDropDownMenu, SIGNAL(clicked()),
            this, SLOT(slotPushBtnDropDownMenu()));

}

QvtkOrthogonalViewer::~QvtkOrthogonalViewer()
{
    // #TODO: Delete UI
	delete this->ui;
}

void QvtkOrthogonalViewer::SetSagitalViewPlaneNormal(double e1, double e2, double e3)
{
	double invect[] = { e1, e2, e3 };
	if (vtkMath::Norm(invect) < 1e-15) {
		std::string msg = "Attempting to set yViewPlaneNormal to zero vector. Clamping it back to {0,1,0}";
		qWarning() << QString::fromStdString(msg);
		e1 = 0;
		e2 = 1;
		e3 = 0;
		invect[0] = 0;
		invect[1] = 1;
		invect[2] = 0;
	}

	/* Check if this is orthogonal with xViewPlaneNormal, if not snap it to
	the closest vect that is orthogonal with xViewPlaneNormal */
	if (abs(vtkMath::Dot(s_sagitalViewPlaneNormal, s_axialViewPlaneNormal)) > 1e-10) {
		double rotAx[3];
		vtkMath::Cross(s_axialViewPlaneNormal, s_sagitalViewPlaneNormal, rotAx);
		vtkMath::Cross(s_axialViewPlaneNormal, rotAx, s_sagitalViewPlaneNormal);
		vtkMath::Normalize(s_sagitalViewPlaneNormal);
	}
	else {
		QvtkOrthogonalViewer::s_sagitalViewPlaneNormal[0] = e1;
		QvtkOrthogonalViewer::s_sagitalViewPlaneNormal[1] = e2;
		QvtkOrthogonalViewer::s_sagitalViewPlaneNormal[2] = e3;
	}

	QvtkOrthogonalViewer dummy;
	std::string classname = typeid(dummy).name();

	for(QList<QvtkAbstractViewer*>::const_iterator cit = s_viewersList.cbegin();
			cit != s_viewersList.cend(); ++cit){
		if(classname == typeid(*cit).name())
		{
			static_cast<QvtkOrthogonalViewer*>(*cit)->UpdateCameraViewPlaneNormal();
		}
	}

//#ifdef _MSC_VER
//	for each (QvtkAbstractViewer *viewer in s_viewersList)
//	{
//		if (classname == typeid(*viewer).name())
//		{
//			((QvtkOrthogonalViewer*)viewer)->UpdateCameraViewPlaneNormal();
//
//		};
//	}
//#elif __linux
//	typedef typename std::vector<QvtkAbstractViewer *>:: IterType;
//	for (IterType iter = s_viewersList.begin(); iter != s_viewersList.end(); iter++) {
//		QvtkAbstractViewer *viewer = *iter;
//		if (classname == typeid(*viewer).name()) {
//			((QvtkOrthogonalViewer *)viewer)->UpdateCameraViewPlaneNormal();
//		};
//	}
//#endif

}

void QvtkOrthogonalViewer::SetSagitalViewPlaneNormal(double *normal)
{
	QvtkOrthogonalViewer::SetSagitalViewPlaneNormal(normal[0], normal[1], normal[2]);
}

const double *QvtkOrthogonalViewer::GetAxiaViewPlaneNormal()
{
	return s_axialViewPlaneNormal;
}

const double *QvtkOrthogonalViewer::GetSagitalViewPlaneNormal()
{
	return s_sagitalViewPlaneNormal;
}

const double* QvtkOrthogonalViewer::GetCoronalViewPlaneNormal()
{
	return s_coronalViewPlaneNormal;
}

void QvtkOrthogonalViewer::UpdateCameraViewPlaneNormal()
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
	case Axial: memcpy(targetOri, s_axialViewPlaneNormal, sizeof(double) * 3);
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
				memcpy(targetOri, s_coronalViewPlaneNormal, sizeof(double) * 3);
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

void QvtkOrthogonalViewer::UpdateViewUp()
{
	double viewUp[3];
	switch (this->orientation) {
	case Axial:
		if (this->righthandness)
		{
			vtkMath::Cross(s_sagitalViewPlaneNormal, s_axialViewPlaneNormal, viewUp);
			this->camera->SetViewUp(viewUp);
		}
		else
		{
			vtkMath::Cross(s_axialViewPlaneNormal, s_sagitalViewPlaneNormal, viewUp);
			this->camera->SetViewUp(viewUp);
		}
		break;
	case Coronal:
	case Sagital:
		this->camera->SetViewUp(s_axialViewPlaneNormal);
		break;
	default: std::string errmsg = "ORIENTATION is not between 0 to 2!";
		qCritical() << QString::fromStdString(errmsg);
		throw std::logic_error(errmsg);
		break;
	}
}

void QvtkOrthogonalViewer::SetAxialViewPlaneNormal(double *normal)
{
	QvtkOrthogonalViewer::SetAxialViewPlaneNormal(normal[0], normal[1], normal[2]);
}

void QvtkOrthogonalViewer::SetAxialViewPlaneNormal(double e1, double e2, double e3)
{
	double invect[] = { e1, e2, e3 };
	if (vtkMath::Norm(invect) < 1e-15) {
		std::string msg = "Attempting to set xViewPlaneNormal to zero vector. Clamping it back to {1,0,0}";
		qWarning() << QString::fromStdString(msg);
		e1 = 1;
		e2 = 0;
		e3 = 0;
	}

	QvtkOrthogonalViewer::s_axialViewPlaneNormal[0] = e1;
	QvtkOrthogonalViewer::s_axialViewPlaneNormal[1] = e2;
	QvtkOrthogonalViewer::s_axialViewPlaneNormal[2] = e3;

	QvtkOrthogonalViewer dummy;
	std::string classname = typeid(dummy).name();

	typedef QList<QvtkAbstractViewer *>::iterator IterType;
	for (IterType iter = s_viewersList.begin(); iter != s_viewersList.end(); iter++) {
		QvtkAbstractViewer *viewer = *iter;
		if (classname == typeid(*viewer).name()) {
			((QvtkOrthogonalViewer *)viewer)->UpdateCameraViewPlaneNormal();
		};
	}

}


vtkRenderWindow *QvtkOrthogonalViewer::GetRenderWindow()
{
    return this->ui->qvtkWidget->GetRenderWindow();
}

vtkRenderWindowInteractor *QvtkOrthogonalViewer::GetInteractor()
{
    return this->ui->qvtkWidget->GetInteractor();
}

void QvtkOrthogonalViewer::slotPushBtnMaximize()
{
    // #TODO: Implement QvtkOrthogonalViewer::slotPushBtnMaximize
//#ifdef _MSC_VER
//    qWarning() << __FUNCSIG__ << "Not implemented!";
//#elif __linux__
//    qWarning() << __FUNCTION__ << "Not implemented!";
//#endif
//
}

void QvtkOrthogonalViewer::slotpushBtnNormal()
{
    // #TODO: Implement QvtkOrthogonalViewer::slotpushBtnNormal
//#ifdef _MSC_VER
//    qWarning() << __FUNCSIG__ << "Not implemented!";
//#elif __linux__
//    qWarning() << __FUNCTION__ << "Not implemented!";
//#endif
}

void QvtkOrthogonalViewer::slotPushBtnDropDownMenu()
{
    // #TODO: Implement QvtkOrthogonalViewer::slotPushBtnDropDownMenu
//#ifdef _MSC_VER
//	qWarning() << __FUNCSIG__ << "Not implemented!";
//#elif __linux__
//	qWarning() << __FUNCTION__ << "Not implemented!";
//#endif
}

void QvtkOrthogonalViewer::InitializeStaticVariables()
{
    if (initializeFlag)
        return;
    else {
        initializeFlag = true;

    }
}
//vtkGenericOpenGLRenderWindow *QvtkOrthogonalViewer::GetGenericOpenGLRenderWindow()
//{
//    return this->ui->qvtkWidget->GetRenderWindow();
//}
//QVTKInteractor *QvtkOrthogonalViewer::GetQVTKInteractor()
//{
//    return this->ui->qvtkWidget->GetInteractor();
//}

void QvtkOrthogonalViewer::SetRighthandness(bool b)
{
	this->righthandness = b;
}

bool QvtkOrthogonalViewer::GetRighthandness()
{
	return this->righthandness;
}
