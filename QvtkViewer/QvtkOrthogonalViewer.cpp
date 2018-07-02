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
	QvtkOrthogonalViewerResourceInit() {
		Q_INIT_RESOURCE(QvtkViewer);
	}
} INIT;
namespace Q {
	namespace vtk {
		/* Set default value of x and y view plane normal */
		bool righthandness_ = true;
		double sagitalViewPlaneNormal_[3] = { 1, 0, 0 };
		double coronalViewPlaneNormal_[3] = { 0, 1, 0 };
		double axialViewPlaneNormal_[3] = { 0, 0, 1 };
		const double OrthogonalViewer::ORIENTATION_YZ_NORMAL[3] = { 1, 0, 0 };
		const double OrthogonalViewer::ORIENTATION_XZ_NORMAL[3] = { 0, 1, 0 };
		const double OrthogonalViewer::ORIENTATION_XY_NORMAL[3] = { 0, 0, 1 };
		void OrthogonalViewer::setOrientation(int orientation)
		{
			ORIENTATION _orientation = static_cast<ORIENTATION>(orientation);
			switch (_orientation)
			{
			case OrthogonalViewer::AXIAL:
				this->ui->pushBtnAxial->setChecked(true);
				break;
			case OrthogonalViewer::CORONAL:
				this->ui->pushBtnCoronal->setChecked(true);
				break;
			case OrthogonalViewer::SAGITAL:
				this->ui->pushBtnSagital->setChecked(true);
				break;
			default:
				break;
			}
			this->orientation = _orientation;
			emit OrientationChanged(this->orientation);
			this->UpdateCameraViewPlaneNormal();
			//this->getkRenderWindow()->Render();
			this->update();
		}

		OrthogonalViewer::OrthogonalViewer(QWidget *parent)
			: Viewer(parent)
		{
			this->ui = new Ui::OrthogonalViewer;
			this->ui->setupUi(this);
			this->syncViewPlaneNormalFlag = true;
			this->righthandness = true;
			this->orientation = ORIENTATION_XY;
			std::copy(ORIENTATION_XY_NORMAL, ORIENTATION_XY_NORMAL + 3, this->currentPlaneNormal);
			std::copy(sagitalViewPlaneNormal_, sagitalViewPlaneNormal_ + 3, this->sagitalViewPlaneNormal);
			std::copy(coronalViewPlaneNormal_, coronalViewPlaneNormal_ + 3, this->coronalViewPlaneNormal);
			std::copy(axialViewPlaneNormal_, axialViewPlaneNormal_ + 3, this->axialViewPlaneNormal);
			this->setupFirstRenderer();
			// connection 
			connect(this->ui->pushBtnAxial, &QPushButton::clicked, this, &OrthogonalViewer::SetOrientationToAxial);
			connect(this->ui->pushBtnCoronal, &QPushButton::clicked, this, &OrthogonalViewer::SetOrientationToCoronal);
			connect(this->ui->pushBtnSagital, &QPushButton::clicked, this, &OrthogonalViewer::SetOrientationToSagital);
		}

		OrthogonalViewer::~OrthogonalViewer()
		{
			delete this->ui;
		}

		void OrthogonalViewer::SetSagitalViewPlaneNormal(double e1, double e2, double e3)
		{
			double normal[] = { e1, e2, e3 };
			if (vtkMath::Norm(normal) < 1e-15) {
				qCritical() << "Attempting to set sagital view plane normal to zero vector.";
				return;
			}
			vtkMath::Normalize(normal);
			if (this->syncViewPlaneNormalFlag) {
				std::copy(normal, normal + 3, sagitalViewPlaneNormal_);
			}
			else {
				std::copy(normal, normal + 3, this->sagitalViewPlaneNormal);
			}
			/* Check if this is orthogonal with xViewPlaneNormal, if not snap it to
			the closest vect that is orthogonal with xViewPlaneNormal */
			if (qAbs(vtkMath::Dot(this->GetSagitalViewPlaneNormal(), this->GetAxiaViewPlaneNormal())) > 1e-10) {
				double _axialViewPlaneNormal[3];
				if (this->GetRighthandness()) {
					vtkMath::Cross(this->GetSagitalViewPlaneNormal(), this->GetCoronalViewPlaneNormal(), _axialViewPlaneNormal);
				}
				else
				{
					vtkMath::Cross(this->GetCoronalViewPlaneNormal(), this->GetSagitalViewPlaneNormal(), _axialViewPlaneNormal);
				}
				vtkMath::Normalize(_axialViewPlaneNormal);
				this->SetAxialViewPlaneNormal(_axialViewPlaneNormal);
			}
			if (this->getSynViewPlaneNormalFlag()) {
				for (QList<Viewer*>::const_iterator cit = viewerList.cbegin();
					cit != viewerList.cend(); ++cit) {
					OrthogonalViewer *viewer = qobject_cast<OrthogonalViewer*>(*cit);
					if (viewer)
					{
						viewer->UpdateCameraViewPlaneNormal();
					}
				}
			}
			else {
				this->UpdateCameraViewPlaneNormal();
			}
		}

		void OrthogonalViewer::SetSagitalViewPlaneNormal(double *normal)
		{
			OrthogonalViewer::SetSagitalViewPlaneNormal(normal[0], normal[1], normal[2]);
		}

		const double *OrthogonalViewer::GetAxiaViewPlaneNormal()
		{
			if (this->syncViewPlaneNormalFlag) {
				return axialViewPlaneNormal_;
			}
			else {
				return this->axialViewPlaneNormal;
			}
		}

		const double *OrthogonalViewer::GetSagitalViewPlaneNormal()
		{
			if (this->syncViewPlaneNormalFlag) {
				return sagitalViewPlaneNormal_;
			}
			else {
				return this->sagitalViewPlaneNormal;
			}
		}

		const double* OrthogonalViewer::GetCoronalViewPlaneNormal()
		{
			//double dotProduct = qAbs(vtkMath::Dot(this->GetSagitalViewPlaneNormal(), this->GetAxiaViewPlaneNormal()));
			//if (dotProduct > 1e-10) {
			//	qWarning() << "SAGITAL view plane normal and axial view plane normal are almowt ";
			//	qWarning() << "The product of them is " << dotProduct;
			//}
			if (this->syncViewPlaneNormalFlag) {
				if (this->GetRighthandness()) {
					vtkMath::Cross(this->GetAxiaViewPlaneNormal(), this->GetSagitalViewPlaneNormal(), coronalViewPlaneNormal_);
				}
				else {
					vtkMath::Cross(this->GetSagitalViewPlaneNormal(), this->GetAxiaViewPlaneNormal(), coronalViewPlaneNormal_);
				}
				vtkMath::Normalize(coronalViewPlaneNormal_);
				//qDebug() << "sagitial view plane normal";
				//qDebug() << sagitalViewPlaneNormal_[0] << sagitalViewPlaneNormal_[1] << sagitalViewPlaneNormal_[2];
				//qDebug() << "CORONAL View Plane Normal ";
				//qDebug() << coronalViewPlaneNormal_[0] << coronalViewPlaneNormal_[1] << coronalViewPlaneNormal_[2];
				//qDebug() << "axial view plane normal. ";
				//qDebug() << axialViewPlaneNormal_[0] << axialViewPlaneNormal_[1] << axialViewPlaneNormal_[2];
				return coronalViewPlaneNormal_;
			}
			else {
				if (this->GetRighthandness()) {
					vtkMath::Cross(this->GetAxiaViewPlaneNormal(), this->GetSagitalViewPlaneNormal(), this->coronalViewPlaneNormal);
				}
				else {
					vtkMath::Cross(this->GetSagitalViewPlaneNormal(), this->GetAxiaViewPlaneNormal(), this->coronalViewPlaneNormal);
				}
				vtkMath::Normalize(this->coronalViewPlaneNormal);
				//qDebug() << "sagitial view plane normal";
				//qDebug() << sagitalViewPlaneNormal[0] << sagitalViewPlaneNormal[1] << sagitalViewPlaneNormal[2];
				//qDebug() << "CORONAL View Plane Normal ";
				//qDebug() << coronalViewPlaneNormal[0] << coronalViewPlaneNormal[1] << coronalViewPlaneNormal[2];
				//qDebug() << "axial view plane normal. ";
				//qDebug() << axialViewPlaneNormal[0] << axialViewPlaneNormal[1] << axialViewPlaneNormal[2];
				return this->coronalViewPlaneNormal;
			}
		}

		const double* OrthogonalViewer::UpdateCameraViewPlaneNormal()
		{
			switch (this->orientation)
			{
			case SAGITAL:
				std::copy(this->GetSagitalViewPlaneNormal(), this->GetSagitalViewPlaneNormal() + 3, this->currentPlaneNormal);
				break;
			case CORONAL:
				std::copy(this->GetCoronalViewPlaneNormal(), this->GetCoronalViewPlaneNormal() + 3, this->currentPlaneNormal);
				break;
			case AXIAL:
				std::copy(this->GetAxiaViewPlaneNormal(), this->GetAxiaViewPlaneNormal() + 3, this->currentPlaneNormal);
				break;
			case ORIENTATION_YZ:
				std::copy(ORIENTATION_XY_NORMAL, ORIENTATION_XY_NORMAL + 3, this->currentPlaneNormal);
				break;
			case ORIENTATION_XZ:
				std::copy(ORIENTATION_XZ_NORMAL, ORIENTATION_XZ_NORMAL + 3, this->currentPlaneNormal);
				break;
			case ORIENTATION_XY:
				std::copy(ORIENTATION_XY_NORMAL, ORIENTATION_XY_NORMAL + 3, this->currentPlaneNormal);
				break;
			default:
				break;
			}
			/* Set new position to camera */
			double dist = this->camera->GetDistance();
			double *fp = this->camera->GetFocalPoint();
			double newCamPos[3];
			double targetOri[3];
			std::copy(this->currentPlaneNormal, this->currentPlaneNormal + 3, targetOri);
			vtkMath::MultiplyScalar(targetOri, dist);
			vtkMath::Add(fp, targetOri, newCamPos);
			this->camera->SetPosition(newCamPos);
			this->UpdateViewUp();
			return this->currentPlaneNormal;
		}

		double* OrthogonalViewer::UpdateViewUp()
		{
			double viewUp[3];
			switch (this->orientation) {
			case SAGITAL:
				this->camera->SetViewUp(this->GetAxiaViewPlaneNormal());
				break;
			case CORONAL:
				this->camera->SetViewUp(this->GetSagitalViewPlaneNormal());
				break;
			case AXIAL:
				this->camera->SetViewUp(this->GetCoronalViewPlaneNormal());
				break;
			case ORIENTATION_YZ:
				this->camera->SetViewUp(ORIENTATION_XY_NORMAL);
				break;
			case ORIENTATION_XZ:
				this->camera->SetViewUp(ORIENTATION_YZ_NORMAL);
				break;
			case ORIENTATION_XY:
				this->camera->SetViewUp(ORIENTATION_XZ_NORMAL);
				break;
			default:
				std::string errmsg = "ORIENTATION is not between 0 to 2!";
				qCritical() << QString::fromStdString(errmsg);
				break;
			}
			return this->camera->GetViewUp();
		}

		void OrthogonalViewer::SetAxialViewPlaneNormal(double *normal)
		{
			OrthogonalViewer::SetAxialViewPlaneNormal(normal[0], normal[1], normal[2]);
		}

		void OrthogonalViewer::SetAxialViewPlaneNormal(double e1, double e2, double e3)
		{
			double normal[] = { e1, e2, e3 };
			if (vtkMath::Norm(normal) < 1e-15) {
				qCritical() << "Attempting to set axial view plane normal to zero vector.";
				return;
			}
			vtkMath::Normalize(normal);
			if (this->syncViewPlaneNormalFlag) {
				std::copy(normal, normal + 3, axialViewPlaneNormal_);
			}
			else {
				std::copy(normal, normal + 3, this->axialViewPlaneNormal);
			}
			if (qAbs(vtkMath::Dot(this->GetSagitalViewPlaneNormal(), this->GetAxiaViewPlaneNormal())) > 1e-10) {
				double _sagitalViewPlaneNormal[3];
				if (this->GetRighthandness()) {
					vtkMath::Cross(this->GetCoronalViewPlaneNormal(), this->GetAxiaViewPlaneNormal(), _sagitalViewPlaneNormal);
				}
				else {
					vtkMath::Cross(this->GetAxiaViewPlaneNormal(), this->GetCoronalViewPlaneNormal(), _sagitalViewPlaneNormal);
				}
				vtkMath::Normalize(_sagitalViewPlaneNormal);
				this->SetSagitalViewPlaneNormal(_sagitalViewPlaneNormal);
			}
			if (this->getSynViewPlaneNormalFlag()) {
				typedef QList<Viewer *>::iterator IterType;
				for (IterType iter = viewerList.begin(); iter != viewerList.end(); iter++) {
					OrthogonalViewer *viewer = qobject_cast<OrthogonalViewer*>(*iter);
					if (viewer) {
						viewer->UpdateCameraViewPlaneNormal();
					};
				}
			}
			else {
				this->UpdateCameraViewPlaneNormal();
			}
		}

		vtkRenderWindow *OrthogonalViewer::getRenderWindow()
		{
			return this->ui->qvtkWidget->GetRenderWindow();
		}

		vtkRenderWindowInteractor *OrthogonalViewer::GetInteractor()
		{
			return this->ui->qvtkWidget->GetInteractor();
		}

		void OrthogonalViewer::SetRighthandness(bool b)
		{
			if (this->syncViewPlaneNormalFlag) {
				righthandness_ = b;
			}
			else {
				this->righthandness = b;
			}
		}

		bool OrthogonalViewer::GetRighthandness()
		{
			if (this->syncViewPlaneNormalFlag) {
				return righthandness_;
			}
			else {
				return this->righthandness;
			}
		}
		void OrthogonalViewer::setSyncViewPlaneNormalFlag(bool flag)
		{
			this->syncViewPlaneNormalFlag = flag;
			this->UpdateCameraViewPlaneNormal();
		}
	}
}

