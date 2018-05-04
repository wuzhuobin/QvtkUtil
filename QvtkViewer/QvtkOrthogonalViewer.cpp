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

//qt
#include <QLayout>
#include <QDebug>
const struct QvtkOrthogonalViewerResourceInit
{
	QvtkOrthogonalViewerResourceInit(){
		Q_INIT_RESOURCE(QvtkViewer);
	}
} INIT;
namespace Q {
namespace vtk{
bool OrthogonalViewer::initializeFlag = false;

/* Set default value of x and y view plane normal */
double OrthogonalViewer::axialViewPlaneNormal[3] = { 0, 0, 1 };
double OrthogonalViewer::sagitalViewPlaneNormal[3] = { 1, 0, 0 };
double OrthogonalViewer::coronalViewPlaneNormal[3] = { 0, 1, 0 };


void OrthogonalViewer::SetOrientation(int orientation)
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
	case OrthogonalViewer::Axial:
		this->ui->pushBtnAxial->setChecked(true);
		break;
	case OrthogonalViewer::Coronal:
		this->ui->pushBtnCoronal->setChecked(true);
		break;
	case OrthogonalViewer::Sagital:
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

int OrthogonalViewer::GetOrientation()
{
    return this->orientation;
}

OrthogonalViewer::OrthogonalViewer(QWidget *parent)
    : Viewer(parent)
{
    this->ui = new Ui::OrthogonalViewer;
    this->ui->setupUi(this);

	this->righthandness = true;
	this->orientation = Axial;

    QVTK_INIT_VIEWER_MACRO();

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

OrthogonalViewer::~OrthogonalViewer()
{
    // #TODO: Delete UI
	delete this->ui;
}

void OrthogonalViewer::SetSagitalViewPlaneNormal(double e1, double e2, double e3)
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
	if (abs(vtkMath::Dot(sagitalViewPlaneNormal, axialViewPlaneNormal)) > 1e-10) {
		double rotAx[3];
		vtkMath::Cross(axialViewPlaneNormal, sagitalViewPlaneNormal, rotAx);
		vtkMath::Cross(axialViewPlaneNormal, rotAx, sagitalViewPlaneNormal);
		vtkMath::Normalize(sagitalViewPlaneNormal);
	}
	else {
		OrthogonalViewer::sagitalViewPlaneNormal[0] = e1;
		OrthogonalViewer::sagitalViewPlaneNormal[1] = e2;
		OrthogonalViewer::sagitalViewPlaneNormal[2] = e3;
	}

	OrthogonalViewer dummy;
	std::string classname = typeid(dummy).name();

	for(QList<Viewer*>::const_iterator cit = s_viewersList.cbegin();
			cit != s_viewersList.cend(); ++cit){
		if(classname == typeid(*cit).name())
		{
			static_cast<OrthogonalViewer*>(*cit)->UpdateCameraViewPlaneNormal();
		}
	}

//#ifdef _MSC_VER
//	for each (Viewer *viewer in s_viewersList)
//	{
//		if (classname == typeid(*viewer).name())
//		{
//			((OrthogonalViewer*)viewer)->UpdateCameraViewPlaneNormal();
//
//		};
//	}
//#elif __linux
//	typedef typename std::vector<Viewer *>:: IterType;
//	for (IterType iter = s_viewersList.begin(); iter != s_viewersList.end(); iter++) {
//		Viewer *viewer = *iter;
//		if (classname == typeid(*viewer).name()) {
//			((OrthogonalViewer *)viewer)->UpdateCameraViewPlaneNormal();
//		};
//	}
//#endif

}

void OrthogonalViewer::SetSagitalViewPlaneNormal(double *normal)
{
	OrthogonalViewer::SetSagitalViewPlaneNormal(normal[0], normal[1], normal[2]);
}

const double *OrthogonalViewer::GetAxiaViewPlaneNormal()
{
	return axialViewPlaneNormal;
}

const double *OrthogonalViewer::GetSagitalViewPlaneNormal()
{
	return sagitalViewPlaneNormal;
}

const double* OrthogonalViewer::GetCoronalViewPlaneNormal()
{
	return coronalViewPlaneNormal;
}

void OrthogonalViewer::UpdateCameraViewPlaneNormal()
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
	case Axial: memcpy(targetOri, axialViewPlaneNormal, sizeof(double) * 3);
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
				memcpy(targetOri, coronalViewPlaneNormal, sizeof(double) * 3);
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

void OrthogonalViewer::UpdateViewUp()
{
	double viewUp[3];
	switch (this->orientation) {
	case Axial:
		if (this->righthandness)
		{
			vtkMath::Cross(sagitalViewPlaneNormal, axialViewPlaneNormal, viewUp);
			this->camera->SetViewUp(viewUp);
		}
		else
		{
			vtkMath::Cross(axialViewPlaneNormal, sagitalViewPlaneNormal, viewUp);
			this->camera->SetViewUp(viewUp);
		}
		break;
	case Coronal:
	case Sagital:
		this->camera->SetViewUp(axialViewPlaneNormal);
		break;
	default: std::string errmsg = "ORIENTATION is not between 0 to 2!";
		qCritical() << QString::fromStdString(errmsg);
		throw std::logic_error(errmsg);
		break;
	}
}

void OrthogonalViewer::SetAxialViewPlaneNormal(double *normal)
{
	OrthogonalViewer::SetAxialViewPlaneNormal(normal[0], normal[1], normal[2]);
}

void OrthogonalViewer::SetAxialViewPlaneNormal(double e1, double e2, double e3)
{
	double invect[] = { e1, e2, e3 };
	if (vtkMath::Norm(invect) < 1e-15) {
		std::string msg = "Attempting to set xViewPlaneNormal to zero vector. Clamping it back to {1,0,0}";
		qWarning() << QString::fromStdString(msg);
		e1 = 1;
		e2 = 0;
		e3 = 0;
	}

	OrthogonalViewer::axialViewPlaneNormal[0] = e1;
	OrthogonalViewer::axialViewPlaneNormal[1] = e2;
	OrthogonalViewer::axialViewPlaneNormal[2] = e3;

	OrthogonalViewer dummy;
	std::string classname = typeid(dummy).name();

	typedef QList<Viewer *>::iterator IterType;
	for (IterType iter = s_viewersList.begin(); iter != s_viewersList.end(); iter++) {
		Viewer *viewer = *iter;
		if (classname == typeid(*viewer).name()) {
			((OrthogonalViewer *)viewer)->UpdateCameraViewPlaneNormal();
		};
	}

}


vtkRenderWindow *OrthogonalViewer::GetRenderWindow()
{
    return this->ui->qvtkWidget->GetRenderWindow();
}

vtkRenderWindowInteractor *OrthogonalViewer::GetInteractor()
{
    return this->ui->qvtkWidget->GetInteractor();
}

void OrthogonalViewer::slotPushBtnMaximize()
{
    // #TODO: Implement OrthogonalViewer::slotPushBtnMaximize
//#ifdef _MSC_VER
//    qWarning() << __FUNCSIG__ << "Not implemented!";
//#elif __linux__
//    qWarning() << __FUNCTION__ << "Not implemented!";
//#endif
//
}

void OrthogonalViewer::slotpushBtnNormal()
{
    // #TODO: Implement OrthogonalViewer::slotpushBtnNormal
//#ifdef _MSC_VER
//    qWarning() << __FUNCSIG__ << "Not implemented!";
//#elif __linux__
//    qWarning() << __FUNCTION__ << "Not implemented!";
//#endif
}

void OrthogonalViewer::slotPushBtnDropDownMenu()
{
    // #TODO: Implement OrthogonalViewer::slotPushBtnDropDownMenu
//#ifdef _MSC_VER
//	qWarning() << __FUNCSIG__ << "Not implemented!";
//#elif __linux__
//	qWarning() << __FUNCTION__ << "Not implemented!";
//#endif
}

void OrthogonalViewer::InitializeStaticVariables()
{
    if (initializeFlag)
        return;
    else {
        initializeFlag = true;

    }
}
//vtkGenericOpenGLRenderWindow *OrthogonalViewer::GetGenericOpenGLRenderWindow()
//{
//    return this->ui->qvtkWidget->GetRenderWindow();
//}
//QVTKInteractor *OrthogonalViewer::GetQVTKInteractor()
//{
//    return this->ui->qvtkWidget->GetInteractor();
//}

void OrthogonalViewer::SetRighthandness(bool b)
{
	this->righthandness = b;
}

bool OrthogonalViewer::GetRighthandness()
{
	return this->righthandness;
}
}
}

